// defines the signum function
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


// updates the positions of all agents from time t to t+1
void updatePositions(int agentNumber, int dim, std::vector<std::vector<double> > &positions,
	std::vector<std::vector<double> > angles, double velocity, double timeStep, double boxSize)
{
    // 2D hard-coded: consider only one angular dimension
    int dimensionAngleIndex = 0;

	for (int agentIndex = 0; agentIndex < agentNumber; agentIndex++)
	{
	    // iterate over all spatial dimensions (dimensionPositionIndex)
		for (int dimensionPositionIndex = 0; dimensionPositionIndex < dim; dimensionPositionIndex++)
		{
			if (dimensionPositionIndex == 0)
			{
				//differential equation
				positions[agentIndex][dimensionPositionIndex] += velocity * cos(angles[agentIndex][dimensionAngleIndex]) * timeStep;

				//periodic boundary conditions
				if (positions[agentIndex][dimensionPositionIndex] >= boxSize)
				{
					positions[agentIndex][dimensionPositionIndex] -= boxSize;
				}
				if (positions[agentIndex][dimensionPositionIndex] < 0)
				{
					positions[agentIndex][dimensionPositionIndex] += boxSize;
				}
			}
			if (dimensionPositionIndex == 1)
			{
                //differential equation
				positions[agentIndex][dimensionPositionIndex] += velocity * sin(angles[agentIndex][dimensionAngleIndex]) * timeStep;

                //periodic boundary condition
				if (positions[agentIndex][dimensionPositionIndex] >= boxSize)
				{
					positions[agentIndex][dimensionPositionIndex] -= boxSize;
				}
				if (positions[agentIndex][dimensionPositionIndex] < 0)
				{
					positions[agentIndex][dimensionPositionIndex] += boxSize;
				}
			}
		}
	}
}


// updates the angles of all agents from time t to t+1
std::vector<std::vector<double> > sumNeighbourAngles(int agentNumber, int dim, 
												std::vector<std::vector<double> > savedAngles,
												double noiseStrength, std::vector<std::vector<int> > interactingNeighbours, double angleIntervalLow,
												double angleIntervalHigh)
{

	

	// Create temporary vector
	std::vector<std::vector<double> > tempAngles(agentNumber, 
        std::vector<double>(dim-1));

	for (int agentIndex = 0; agentIndex < agentNumber; agentIndex++)
	{

	    // treat all angular dimensions
		for (int dimensionIndex = 0; dimensionIndex < dim - 1; dimensionIndex++)
		{
			if (dimensionIndex == 0)
			{
			    // summation of complex exponentials exp(i*theta) (take mean of circular quantity)
				const std::complex<double> i(0,1);
			    std::complex<double> exp_sum = 0;

				// integration of the stochastic differential equations
				// angular update: following the original Vicsek paper
				for (int interact_neighbor_ind = 0; interact_neighbor_ind < interactingNeighbours[agentIndex].size();
				    interact_neighbor_ind++)
				{
					int selected_agent_ind = interactingNeighbours[agentIndex][interact_neighbor_ind];
                    std::complex<double> agent_angle = std::polar(1.0, savedAngles[selected_agent_ind][dimensionIndex]);
					exp_sum += agent_angle;
                    
					
				}
			
				exp_sum = std::norm(exp_sum);
				// obtain argument of complex exponential and add noise
                tempAngles[agentIndex][dimensionIndex] = static_cast<double>(std::arg(exp_sum));

                // pbc, put angles into [-pi,pi) by subtracting or adding 2pi
				if (tempAngles[agentIndex][dimensionIndex] > angleIntervalHigh)
				{ 
					tempAngles[agentIndex][dimensionIndex] -= 2 * atan(1) * 4;
				}
				else if (tempAngles[agentIndex][dimensionIndex] <= angleIntervalLow)
				{
					tempAngles[agentIndex][dimensionIndex] += 2 * atan(1) * 4;
				}
			}
		}
	}
	
	for (int agentIndex = 0; agentIndex < agentNumber; agentIndex++)
        {
            for (int dimensionIndex = 0; dimensionIndex < dim-1; dimensionIndex++)
            {
                savedAngles[agentIndex][dimensionIndex] = tempAngles[agentIndex][dimensionIndex];
            }
        }

	return savedAngles;
}



// updates the angles of all agents from time t to t+1
std::vector<std::vector<double> > updateAngles(int agentNumber, int dim, std::vector<std::vector<double> > angles,
	std::vector<std::vector<double> > savedAngles,
	double noiseStrength, std::vector<std::vector<int> > interactingNeighbours, double angleIntervalLow,
	double angleIntervalHigh, std::mt19937& gen, double polar_interact_prob)
{

	// RNG: draw from [-pi;pi) (angular noise) and [0,1) (polar or nematic interactions)
    std::uniform_real_distribution<> dis(angleIntervalLow, angleIntervalHigh);
    std::uniform_real_distribution<> dis2(0, 1);
	// Create temporary vector
	std::vector<std::vector<double> > tempAngles(agentNumber, 
        std::vector<double>(dim-1));

	for (int agentIndex = 0; agentIndex < agentNumber; agentIndex++)
	{

	    // treat all angular dimensions
		for (int dimensionIndex = 0; dimensionIndex < dim - 1; dimensionIndex++)
		{
			if (dimensionIndex == 0)
			{
			    // summation of complex exponentials exp(i*theta) (take mean of circular quantity)
				const std::complex<double> i(0,1);
			    std::complex<double> exp_sum = 0;
				std::complex<double> neighbourhoodSum = 0;
				// integration of the stochastic differential equations
				// angular update: following the original Vicsek paper

                std::complex<double> agentAngle = std::polar(1.0, angles[agentIndex][dimensionIndex]);

				for (int interact_neighbor_ind = 0; interact_neighbor_ind < interactingNeighbours[agentIndex].size();
				    interact_neighbor_ind++)
				{
					int selected_agent_ind = interactingNeighbours[agentIndex][interact_neighbor_ind];
                    std::complex<double> agent_angle = std::polar(1.0, savedAngles[selected_agent_ind][dimensionIndex]);
					neighbourhoodSum += agent_angle;
                    
				}
		
				exp_sum = agentAngle + neighbourhoodSum;
			
				// obtain argument of complex exponential and add noise
                tempAngles[agentIndex][dimensionIndex] = static_cast<double>(std::arg(exp_sum)) + noiseStrength*dis(gen);


                // pbc, put angles into [-pi,pi) by subtracting or adding 2pi
				if (tempAngles[agentIndex][dimensionIndex] > angleIntervalHigh)
				{ 
					tempAngles[agentIndex][dimensionIndex] -= 2 * atan(1) * 4;
				}
				else if (tempAngles[agentIndex][dimensionIndex] <= angleIntervalLow)
				{
					tempAngles[agentIndex][dimensionIndex] += 2 * atan(1) * 4;
				}
			}
		}
	}
	
	for (int agentIndex = 0; agentIndex < agentNumber; agentIndex++)
        {
            for (int dimensionIndex = 0; dimensionIndex < dim-1; dimensionIndex++)
            {	

                angles[agentIndex][dimensionIndex] = tempAngles[agentIndex][dimensionIndex];
            }
        }

	return angles;
}