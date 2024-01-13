#include <vector>

#include <iostream>

#include <fstream>

#include <string>

#include <cmath>

#include <cstring>

#include <random>

#include <sstream>

#include <algorithm>

#include <ctime>

#include <complex>

#include <unistd.h>

#include "record_frame.hpp"

#include "update_params.hpp"

#include "system_init.hpp"

#include "debugging_tools.hpp"

#include "subspace_operations.hpp"

#include "io_init.hpp"

#include "chrono"






int main(int argc, char *argv[])

{
  auto start = std::chrono::system_clock::now();
  // parameters which may be modified through parsing

  int agent_number; // number of microscopic agents in the simulation

  std::string output_path = "../data/positions/";               // for generation of output

  std::string output_snapshot = "../data/snapshot/";        // for generation of output


  double velocity = 0.5;            // constant velocity of agents
  double box_size = 256;            // system size
  double noise_strength = 0.3;      // tunes stochasticity of interaction
  double neighborhood_radius = 1.0; // interaction radius around every agent
  bool pbc = true;                  // sets periodic boundary conditions
  double time_total = 100;         // total runtime of the simulation
  double equilibriumTime = 0;     // time to allow system to equilibriate
  double time_step = 1;             // smallest time step for integration of ODEs
  double polar_interact_prob = 1;   // probability of polar versus nematic behaviour
  std::string init_mode = "continue";
  int delayTime = 5;
  // not parsable parameters
  double timerecord_step = 1.0; // time step which is recorded
  int dim = 2;                  // dimension of the system, hard-coded, do not change!

  // debugging options
  bool debug = false;                                      // debugging option for more verbose output
  unsigned long seed = mix(clock(), time(NULL), getpid()); // seed for random number generator

  // input / output handling
  std::ofstream outputfile;
  std::ofstream outputfile_snapshot;


  parse_input(argc, argv, agent_number, output_path, output_snapshot , noise_strength, delayTime);

  // output_init(dim, outputfile, agent_number, output_path, velocity, box_size, noise_strength, neighborhood_radius,
  // pbc, time_total, time_step, polar_interact_prob, delayTime);
  output_init(outputfile, agent_number, output_path, noise_strength, delayTime);

  output_init_snapshot(outputfile_snapshot, output_snapshot, noise_strength, delayTime);

  
  // initialize random number generator
  std::random_device rd;

  std::mt19937 gen(rd());

  gen.seed(seed);

  // define angle interval, here: [-pi, pi)
  // changing this might cause issues in the update_angles function
  double angle_interval_low = -atan(1) * 4;

  double angle_interval_high = atan(1) * 4;

  // loop over time interval

  // allocate random positions and angles
  std::vector<std::vector<double>> positions = positions_init(
      agent_number, box_size, dim, gen, init_mode, delayTime, noise_strength);

  std::vector<std::vector<double>> angles = angles_init(
      agent_number, box_size, dim, angle_interval_low, angle_interval_high, gen, init_mode, delayTime, noise_strength);

  std::vector<std::vector<std::vector<double>>> savedAngles; // array to save past angles

  std::vector<std::vector<std::vector<int>>> savedInteracting_neighbors;
  // create subspace with M x M fields
  // M is the box size divided by the specified neighborhood_radius
  int subspacing_number = static_cast<int>(floor(box_size / neighborhood_radius)+1); // per dimension

  int expected_agentnumber_per_subspace =
      static_cast<int>(ceil(agent_number / std::pow(subspacing_number, 2)));

  // determine neighboring cells in subspace
  std::vector<std::vector<std::vector<std::vector<int>>>> subspace_cell_neighbors =
      get_subspace_cell_neighbors(pbc, subspacing_number, dim, debug);
  
  // Initialise Order Parameter
  float orderParameter = 0.0;
  float orderParameter2 = 0.0;
  float orderParameter4 = 0.0;

  for (double time = 1; time <= time_total; time += time_step)
  {


    // initialize/reset subspace

    std::vector<std::vector<std::vector<int>>> subspace_allocation = subspace_init(
        subspacing_number, expected_agentnumber_per_subspace);

    // allocate agents to subspaces
    allocate_to_subspace(
        subspace_allocation, neighborhood_radius, agent_number, positions, debug);

    // determine which agents interact with each other
    std::vector<std::vector<int>> interacting_neighbors = get_interacting_neighbors(
        subspace_cell_neighbors, subspace_allocation,
        expected_agentnumber_per_subspace, subspacing_number, dim,
        neighborhood_radius, positions, agent_number, box_size, pbc, debug);

    // update direction, velocity and position

    savedAngles.push_back(angles);

    savedInteracting_neighbors.push_back(interacting_neighbors);

    angles = update_angles(agent_number, dim, angles, savedAngles[0], noise_strength, savedInteracting_neighbors[0], angle_interval_low,
                           angle_interval_high, gen, polar_interact_prob);

    update_positions(agent_number, dim, positions, angles, velocity, time_step, box_size);

    if (savedAngles.size() > delayTime)
    {
      savedAngles.erase(savedAngles.begin());
    }

    if (savedInteracting_neighbors.size() > delayTime)
    {
      savedInteracting_neighbors.erase(savedInteracting_neighbors.begin());
    }
    orderParameter = 0.0;
    orderParameter2 = 0.0;
    orderParameter4 = 0.0;
    orderParameter = orderParameterCalculation(positions, angles, agent_number, velocity);
    
    orderParameter2 = orderParameter*orderParameter;
    orderParameter4 = orderParameter*orderParameter*orderParameter*orderParameter;

    outputfile << time << ' ' << orderParameter << ' ' << orderParameter2 << ' ' <<orderParameter4 << '\n';

    if (static_cast<int>(time) == time_total)
    {
     record_snapshot(outputfile_snapshot,dim, agent_number, noise_strength, positions, angles);
    }

  
  }
  // Record Observables
  outputfile.close();

  auto end = std::chrono::system_clock::now();
 
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s"
              << std::endl;
}
