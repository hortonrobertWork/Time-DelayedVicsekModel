void record_snapshot(std::ofstream& outputfile_snapshot,int dim, int agent_number,
    double noise_strenght,
    std::vector<std::vector<double> > positions,
    std::vector<std::vector<double> > angles)
{

        outputfile_snapshot << "Noise:"<< noise_strenght << '\n';
        for (int agent_ind = 0; agent_ind < agent_number; agent_ind ++)
        {
            
            outputfile_snapshot << agent_ind << "\t";
            
            // write all position coordinates into file
            for (int dim_ind = 0; dim_ind < dim; dim_ind++)
            {
                outputfile_snapshot << positions[agent_ind][dim_ind] << "\t";
            }
            
            // write all angular components into file
            for (int dim_ind = 0; dim_ind < dim-1; dim_ind++)
            {
                outputfile_snapshot << angles[agent_ind][dim_ind] << "\t";
            }
            
            // go to next line and treat next agent
            outputfile_snapshot << std::endl;
        }
}


void recordFrameSteadyState(std::ofstream& outputfile, int agent_number,
    double time_step, double timerecord_step, double time,  int dim,
    std::vector<std::vector<double> > positions,
    std::vector<std::vector<double> > angles)
{
    {
        for (int agent_ind = 0; agent_ind < agent_number; agent_ind ++)
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




void distantDependentCorrelationFunction(std::ofstream& outputCorrelation, std::vector<std::vector<double> > positions, 
    std::vector<std::vector<double> > angles, 
    double time, int agent_number, double velocity)
    {
    const std::complex<double> i(0,1);
    std::complex<double> averageVelocity = 0;
    std::vector<std::vector<double> > fluctuationAngles(agent_number, std::vector<double>(1));
    double x=0; 
    double y=0;
    double orderParameter = 0;
    double orderParameter1 = 0.0;
    double orderParameter2 = 0.0;
    double orderParameter4 = 0.0;

    for (int j = 0; j < agent_number; j++)
    {   
        
        averageVelocity += std::polar(velocity, angles[j][0]);

    }

    orderParameter = std::abs(averageVelocity)/static_cast<double>(agent_number);
    orderParameter += orderParameter;
    orderParameter2 += orderParameter * orderParameter;
    orderParameter4 += orderParameter * orderParameter * orderParameter * orderParameter;


    
    outputCorrelation << time << '\t' << orderParameter << '\t'  << orderParameter2 << '\t' << orderParameter4;
    outputCorrelation << std::endl;
    }

float orderParameterCalculation(std::vector<std::vector<double> > positions, 
    std::vector<std::vector<double> > angles, int agent_number, double velocity)
    {
    const std::complex<double> i(0,1);
    std::complex<double> averageVelocity = 0;
    double x=0; 
    double y=0;
    double orderParameter = 0;
    double orderParameter1 = 0.0;
    double orderParameter2 = 0.0;
    double orderParameter4 = 0.0;

    for (int j = 0; j < agent_number; j++)
    {   
        
        averageVelocity += std::polar(1.0, angles[j][0]);

    }

    orderParameter = std::abs(averageVelocity)/static_cast<double>(agent_number);
    
    return orderParameter;
    }

