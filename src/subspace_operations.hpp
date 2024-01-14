
// initialisation/reset of subspace container
std::vector<std::vector<std::vector<int> > > initialiseSubspace(
        int subspacingNumber, int expectedAgentNumberPerSubspace)
{
    // create empty subspace container
    std::vector < std::vector < std::vector < int > > > subspaceAllocation(
            subspacingNumber, std::vector < std::vector < int > > (
                    subspacingNumber, std::vector<int>() //expectedAgentNumberPerSubspace * 5)
            )
    );
    return subspaceAllocation;
}



// allocate agent_indices to subspaces
void allocateToSubspace(
        std::vector<std::vector<std::vector<int> > > &subspaceAllocation, // pass by ref
        double neighbourhoodRadius, int agentNumber,
        std::vector<std::vector<double> > positions, bool debug)
{
  // allocate all agents to subboxes
  for (int agentIndex = 0; agentIndex < agentNumber; agentIndex++)
  {
      // subbox pos = integer division of position by subbox spacing (nbhood radius)
      // Caution: dim = 2 hard-coded here
      int subspaceXCoordinate = static_cast<int> (floor(positions[agentIndex][0] / neighbourhoodRadius) );
      int subspaceYCoordinate = static_cast<int> (floor(positions[agentIndex][1] / neighbourhoodRadius) );

      subspaceAllocation[subspaceXCoordinate][subspaceYCoordinate].push_back(agentIndex);
  }
}



// determine which subspace cells neighbor a certain cell
// first 2 dim: coord. of subspace cell, 3rd dim: neighbor index, 4th dim: array of (x,y) information of neighbors
std::vector < std::vector < std::vector < std::vector<int> > > > getSubspaceCellNeighbours(
        bool periodicBoundaryConditions, int subspacingNumber, int dim, bool debug)
{
    // create empty subspace container
    std::vector < std::vector < std::vector < std::vector < int > > > > subspaceCellNeighbours(
            subspacingNumber, std::vector < std::vector < std::vector < int > > >(
                    subspacingNumber, std::vector< std::vector< int> >(
                            std::pow(dim+1, 2), std::vector <int>(dim)
                    )
            )
    );

    // iterate through subspace grid
    for (int subspaceXCoordinate = 0; subspaceXCoordinate < subspacingNumber; subspaceXCoordinate++)
    {
        for (int subspaceYCoordinate = 0; subspaceYCoordinate < subspacingNumber; subspaceYCoordinate++)
        {
            if (debug){std::cout<< "\n******CELL_NB\nx: " << subspaceXCoordinate << "\ty: " << subspaceYCoordinate << std::endl;}

            // store for each cell of subspace itself and its neighbors for neighborhood search
            // in 2D: +/-1 in x and y directions
            int neighbourIndex = 0; // we have 9 neighbors (including cell itself) in 2D

            for (int neighbourCellXIndex = subspaceXCoordinate - 1;
                 neighbourCellXIndex < subspaceXCoordinate + 2;
                 neighbourCellXIndex++)
            {
                for (int neighbourCellYIndex = subspaceYCoordinate - 1;
                     neighbourCellYIndex < subspaceYCoordinate + 2;
                     neighbourCellYIndex++)
                {
                    //std::cout << "was x: " << neighbourCellXIndex <<
                    //"\ty: " << neighbourCellYIndex << std::endl;
                    // treat periodic boundary conditions for each dimension (hardcoded!)
                    // x
                    if (neighbourCellXIndex < 0 && periodicBoundaryConditions)
                    {
                        subspaceCellNeighbours[subspaceXCoordinate][subspaceYCoordinate][neighbourIndex][0] =
                                neighbourCellXIndex + subspacingNumber;
                    }
                    else if (neighbourCellXIndex > subspacingNumber-1 && periodicBoundaryConditions)
                    {
                        subspaceCellNeighbours[subspaceXCoordinate][subspaceYCoordinate][neighbourIndex][0] =
                                neighbourCellXIndex - subspacingNumber;
                    }
                    else if (neighbourCellXIndex <= subspacingNumber-1 && neighbourCellXIndex >= 0)
                    {
                        subspaceCellNeighbours[subspaceXCoordinate][subspaceYCoordinate][neighbourIndex][0] =
                                neighbourCellXIndex;
                    }
                    // y
                    if (neighbourCellYIndex < 0 && periodicBoundaryConditions)
                    {
                        subspaceCellNeighbours[subspaceXCoordinate][subspaceYCoordinate][neighbourIndex][1] =
                                neighbourCellYIndex + subspacingNumber;
                    }
                    else if (neighbourCellYIndex > subspacingNumber-1 && periodicBoundaryConditions)
                    {
                        subspaceCellNeighbours[subspaceXCoordinate][subspaceYCoordinate][neighbourIndex][1] =
                                neighbourCellYIndex - subspacingNumber;
                    }
                    else if (neighbourCellYIndex <= subspacingNumber-1 && neighbourCellYIndex >= 0)
                    {
                        subspaceCellNeighbours[subspaceXCoordinate][subspaceYCoordinate][neighbourIndex][1] =
                                neighbourCellYIndex;
                    }

                    if (debug){std::cout << "nb_x: " <<
                        subspaceCellNeighbours[subspaceXCoordinate][subspaceYCoordinate][neighbourIndex][0] << "\tnb_y: " <<
                        subspaceCellNeighbours[subspaceXCoordinate][subspaceYCoordinate][neighbourIndex][1] << std::endl;}

                    neighbourIndex++;
                }
            }
        }
    }
    return subspaceCellNeighbours;
}



// determine actual interactions between agent in subspace and its neighbors
std::vector<std::vector<int> > getInteractingNeighbours(
        std::vector < std::vector < std::vector < std::vector<int> > > > subspaceCellNeighbours,
        std::vector<std::vector<std::vector<int> > > subspaceAllocation,
        int expectedAgentNumberPerSubspace, int subspacingNumber, int dim,
        double neighbourhoodRadius, std::vector<std::vector<double> > positions,
        int agentNumber, double boxSize, bool periodicBoundaryConditions, bool debug
        )
{
    // initialize interaction container
    std::vector<std::vector<int> > interactingNeighbours(agentNumber,
            std::vector<int>() ); //expectedAgentNumberPerSubspace * 5) );

    // iterate through subspace grid
    for (int subspaceXCoordinate = 0; subspaceXCoordinate < subspacingNumber; subspaceXCoordinate++)
    {
        for (int subspaceYCoordinate = 0; subspaceYCoordinate < subspacingNumber; subspaceYCoordinate++)
        {
            if (debug){std::cout << "\n=====BASE_CELL=====\nx: " << subspaceXCoordinate << "\ty: " << subspaceYCoordinate << std::endl;}

            // iterate through all agent_inds in one subspace cell
            for (int subspaceAgentIndex = 0; subspaceAgentIndex < subspaceAllocation[subspaceXCoordinate][subspaceYCoordinate].size();
                subspaceAgentIndex++)
            {
                // retrieve agent index of agent in subspace cell
                int agentIndex = subspaceAllocation[subspaceXCoordinate][subspaceYCoordinate][subspaceAgentIndex];

                if (debug){std::cout << "\n>>> treating subsp_agent " << subspaceAgentIndex << "\tof " <<
                    subspaceAllocation[subspaceXCoordinate][subspaceYCoordinate].size() <<
                    " in this cell\n with agentIndex "<< agentIndex << std::endl;}

                // iterate through all neighboring cells to this cell
                for (int neighbourCellIndex = 0; neighbourCellIndex < static_cast<int>(std::pow(dim+1, 2) ); neighbourCellIndex++)
                {
                    // retrieve x and y coords of current neighboring subspace cell
                    int subspaceNeighbourXCoordinate = subspaceCellNeighbours[subspaceXCoordinate][subspaceYCoordinate][neighbourCellIndex][0];
                    int subspaceNeighbourYCoordinate = subspaceCellNeighbours[subspaceXCoordinate][subspaceYCoordinate][neighbourCellIndex][1];

                    if (debug){std::cout << "\n********INTERACTING_CELL\nneighborcell_x: " << subspaceNeighbourXCoordinate <<
                    "\tneighborcell_y: " << subspaceNeighbourYCoordinate << std::endl;}


                    // access agent indices in neighboring cells
                    for (int neighbourIndex = 0;
                        neighbourIndex < subspaceAllocation[subspaceNeighbourXCoordinate][subspaceNeighbourYCoordinate].size();
                        neighbourIndex++)
                    {
                        // retrieve agent index of neighboring agent
                        int neighbourAgentIndex = subspaceAllocation[subspaceNeighbourXCoordinate][subspaceNeighbourYCoordinate][neighbourIndex];

                        if (debug){std::cout << ">>> neighbor  " << neighbourIndex << "\tof " <<
                        subspaceAllocation[subspaceNeighbourXCoordinate][subspaceNeighbourYCoordinate].size() <<
                        "\t with agentIndex "<< neighbourAgentIndex << std::endl;}


                        // compute distance between subsp_agent and subsp_neighbor_agent (2D hardcoded)
                        double xDistance = fabs(positions[neighbourAgentIndex][0] - positions[agentIndex][0]);
                        double yDistance = fabs(positions[neighbourAgentIndex][1] - positions[agentIndex][1]);

                        // treat pbcs (hardcoded)
                        if (xDistance > (boxSize/2) && periodicBoundaryConditions)
                        {
                            xDistance = boxSize - xDistance;
                        }
                        if (yDistance > (boxSize/2) && periodicBoundaryConditions)
                        {
                            yDistance = boxSize - yDistance;
                        }

                        // compute actual distance
                        double distance = sqrt( std::pow( xDistance, 2 ) + std::pow( yDistance, 2 ) );

                        // decide on interaction between these two agents based on neighbourhoodRadius
                        if (distance <= neighbourhoodRadius && agentIndex != neighbourAgentIndex)
                        {
                            interactingNeighbours[agentIndex].push_back(neighbourAgentIndex);

                            if (debug){std::cout << "\t dist: " << distance << "\t interact: true" << std::endl;}
                        }
                        else
                        {
                            if (debug){std::cout << "\t dist: " << distance << "\t interact: false" << std::endl;}
                        }
                    }
                    if (debug){std::cout << "********" << std::endl;}
                }
            }
        if (debug){std::cout << "===================\n" << std::endl;}
        }
    }
    return interactingNeighbours;
}




