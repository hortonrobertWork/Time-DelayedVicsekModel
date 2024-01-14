void recordSnapshot(std::ofstream& outputfileSnapshot,int dim, int agentNumber,
    double noiseStrength,
    std::vector<std::vector<double> > positions,
    std::vector<std::vector<double> > angles)
{

        outputfileSnapshot << "Noise:"<< noiseStrength << '\n';
        for (int agent_ind = 0; agent_ind < agentNumber; agent_ind ++)
        {
            
            outputfileSnapshot << agent_ind << "\t";
            
            // write all position coordinates into file
            for (int dim_ind = 0; dim_ind < dim; dim_ind++)
            {
                outputfileSnapshot << positions[agent_ind][dim_ind] << "\t";
            }
            
            // write all angular components into file
            for (int dim_ind = 0; dim_ind < dim-1; dim_ind++)
            {
                outputfileSnapshot << angles[agent_ind][dim_ind] << "\t";
            }
            
            // go to next line and treat next agent
            outputfileSnapshot << std::endl;
        }
}

void recordFrameSteadyState(std::ofstream& outputfile, int agentNumber,
    double timeStep, double timerecordStep, double time,  int dim,
    std::vector<std::vector<double> > positions,
    std::vector<std::vector<double> > angles)
{
    {
        for (int agent_ind = 0; agent_ind < agentNumber; agent_ind ++)
        {
            outputfile << time << "\t" << agent_ind << "\t";
            
            // write all position coordinates into file
            for (int dim_ind = 0; dim_ind < dim; dim_ind++)
            {
                outputfile << positions[agent_ind][dim_ind] << "\t";
            }
            
            // write all angular components into file
            for (int dim_ind = 0; dim_ind < dim-1; dim_ind++)
            {
                outputfile << angles[agent_ind][dim_ind] << "\t";
            }
            
            // go to next line and treat next agent
            outputfile << std::endl;
        }
    }
}
void recordAnimaion(std::ofstream& outputfile, int agentNumber,  int dim,
    std::vector<std::vector<double> > positions,
    std::vector<std::vector<double> > angles)
{
    {
        for (int agent_ind = 0; agent_ind < agentNumber/8; agent_ind ++)
        {
            outputfile << agent_ind << "\t";
            
            // write all position coordinates into file
            for (int dim_ind = 0; dim_ind < dim; dim_ind++)
            {
                outputfile << positions[agent_ind][dim_ind] << "\t";
            }
            
            // write all angular components into file
            for (int dim_ind = 0; dim_ind < dim-1; dim_ind++)
            {
                outputfile << angles[agent_ind][dim_ind] << "\t";
            }
            
            // go to next line and treat next agent
            outputfile << std::endl;
        }
    }
}


float orderParameterCalculation(std::vector<std::vector<double> > positions, 
    std::vector<std::vector<double> > angles, int agentNumber, double velocity)
    {
    const std::complex<double> i(0,1);
    std::complex<double> averageVelocity = 0;
    double x=0; 
    double y=0;
    double orderParameter = 0;
    double orderParameter1 = 0.0;
    double orderParameter2 = 0.0;
    double orderParameter4 = 0.0;

    for (int j = 0; j < agentNumber; j++)
    {   
        
        averageVelocity += std::polar(1.0, angles[j][0]);

    }

    orderParameter = std::abs(averageVelocity)/static_cast<double>(agentNumber);
    
    return orderParameter;
    }

