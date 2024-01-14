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

  std::string outputPath; // for generation of output
  std::string outputSnapshot; // for generation of output
  std::string outputAnimation; // for generation of output
  int agentNumber; // number of microscopic agents in the simulation         
  double noiseStrength;      // tunes stochasticity of interaction
  int delayTime;

  // non parsable parameters
  double neighbourhoodRadius = 1.0;   // interaction radius around every agent
  bool periodicBoundaryConditions = true;                    // sets periodic boundary conditions
  double totalTime = 100;            // total runtime of the simulation
  double equilibriumTime = 0;         // time to allow system to equilibriate
  double timeStep = 1;               // smallest time step for integration of ODEs
  double polarInteractProbability = 1;     // probability of polar versus nematic behaviour
  double velocity = 0.5;              // constant velocity of agents
  double boxSize = 256;              // system size
  double timerecordStep = 1.0;       // time step which is recorded
  int dim = 2;                        // dimension of the system, hard-coded, do not change!

  std::string initialisationMode = "random_uniform"; // whether system starts from random initial conditions or continues from a previous simulation (random_uniform or continue)
  bool recordAnimations = true;
  int startRecordingTime = 1;

  // debugging options
  bool debug = false;                                      // debugging option for more verbose output

  unsigned long seed = mix(clock(), time(NULL), getpid()); // seed for random number generator

  // input / output handling
  std::ofstream outputfile;
  std::ofstream outputfileSnapshot;
  std::ofstream outputfileAnimation;

  parseInput(argc, argv, agentNumber, outputPath, outputSnapshot, outputAnimation, noiseStrength, delayTime);
  initialiseOutputFile(outputfile, agentNumber, outputPath, noiseStrength, delayTime);
  initialiseOutputFileSnapshot(outputfileSnapshot, outputSnapshot, noiseStrength, delayTime);
  initialiseOutputFileAnimation(outputfileAnimation, outputAnimation, noiseStrength, delayTime);

  // initialize random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  gen.seed(seed);

  // define angle interval, here: [-pi, pi)
  // changing this might cause issues in the updateAngles function
  double angleIntervalLow = -atan(1) * 4;
  double angleIntervalHigh = atan(1) * 4;

  // allocate random positions and angles
  std::vector<std::vector<double>> positions = initialisePosition(
      agentNumber, boxSize, dim, gen, initialisationMode, delayTime, noiseStrength);

  std::vector<std::vector<double>> angles = initialiseAngles(
      agentNumber, boxSize, dim, angleIntervalLow, angleIntervalHigh, gen, initialisationMode, delayTime, noiseStrength);

  // initialise arrays to save angles and neighbours for delayed angles
  std::vector<std::vector<std::vector<double>>> savedAngles; // array to save past angles
  std::vector<std::vector<std::vector<int>>> savedInteractingNeighbourlist;

  // create subspace with M x M fields
  // M is the box size divided by the specified neighbourhoodRadius
  int subspaceNumber = static_cast<int>(floor(boxSize / neighbourhoodRadius) + 1); // per dimension
  int expectedAgenNumberPerSubspace =
      static_cast<int>(ceil(agentNumber / std::pow(subspaceNumber, 2)));

  // determine neighboring cells in subspace
  std::vector<std::vector<std::vector<std::vector<int>>>> subspaceCellNeighbours =
      getSubspaceCellNeighbours(periodicBoundaryConditions, subspaceNumber, dim, debug);

  // Initialise Order Parameter
  float orderParameter = 0.0;
  float orderParameter2 = 0.0;
  float orderParameter4 = 0.0;

  for (double time = 1; time <= totalTime; time += timeStep)
  {
    // initialize/reset subspace
    std::vector<std::vector<std::vector<int>>> subspaceAllocation = initialiseSubspace(
        subspaceNumber, expectedAgenNumberPerSubspace);

    // allocate agents to subspaces
    allocateToSubspace(
        subspaceAllocation, neighbourhoodRadius, agentNumber, positions, debug);

    // determine which agents interact with each other
    std::vector<std::vector<int>> interactingNeighbours = getInteractingNeighbours(
        subspaceCellNeighbours, subspaceAllocation,
        expectedAgenNumberPerSubspace, subspaceNumber, dim,
        neighbourhoodRadius, positions, agentNumber, boxSize, periodicBoundaryConditions, debug);

    savedAngles.push_back(angles);
    savedInteractingNeighbourlist.push_back(interactingNeighbours);

    angles = updateAngles(agentNumber, dim, angles, savedAngles[0], noiseStrength, savedInteractingNeighbourlist[0], angleIntervalLow,
                           angleIntervalHigh, gen, polarInteractProbability);

    updatePositions(agentNumber, dim, positions, angles, velocity, timeStep, boxSize);

    if (savedAngles.size() > delayTime)
    {
      savedAngles.erase(savedAngles.begin());
    }

    if (savedInteractingNeighbourlist.size() > delayTime)
    {
      savedInteractingNeighbourlist.erase(savedInteractingNeighbourlist.begin());
    }
    orderParameter = 0.0;
    orderParameter2 = 0.0;
    orderParameter4 = 0.0;
    orderParameter = orderParameterCalculation(positions, angles, agentNumber, velocity);

    orderParameter2 = orderParameter * orderParameter;
    orderParameter4 = orderParameter * orderParameter * orderParameter * orderParameter;

    outputfile << time << ' ' << orderParameter << ' ' << orderParameter2 << ' ' << orderParameter4 << '\n';

    while ((recordAnimations) && (time>startRecordingTime))
    {
    recordAnimaion(outputfileAnimation, agentNumber, dim, positions, angles);
    break;
    }
    if (static_cast<int>(time) == totalTime)
    {
      recordSnapshot(outputfileSnapshot, dim, agentNumber, noiseStrength, positions, angles);
    }
  }
  // Record Observables
  outputfile.close();
  outputfileAnimation.close();
  outputfileSnapshot.close();

  auto end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsedTime = end - start;
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "finished computation at " << std::ctime(&end_time)
            << "elapsed time: " << elapsedTime.count() << "s"
            << std::endl;
}
