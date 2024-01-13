unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}


std::vector<std::vector<double> > positions_init(int agent_number, 
    double box_size, int dim, std::mt19937& gen, std::string init_mode, int delayTime, double noise_strength)
{
    std::vector<std::vector<double> > positions(agent_number, 
        std::vector<double>(dim));
    // saves current positions for each agent
    
    if (init_mode == "random_uniform")
    {
        // RNG: draw from [0;box_size)
        std::uniform_real_distribution<> dis(0.0, box_size);

        // draw for each agent, for each dimension one random number
        for (int agent_ind = 0; agent_ind < agent_number; agent_ind++)
        {
            for (int dim_ind = 0; dim_ind < dim; dim_ind++)
            {
                positions[agent_ind][dim_ind] = dis(gen);
            }
        }
    }

    if (init_mode == "continue")
    { 
        std::ifstream inputFile("../dataContinue/snapshot/delay_"+std::to_string(delayTime)+"/snapshot_eta_"+std::to_string(noise_strength)+"_delay_"+std::to_string(delayTime)+".txt");
        
        std::string line;


        while (std::getline(inputFile, line)) 
            {
            std::istringstream iss(line);
            double a, b, c, d;
            if(!(iss >> a >> b >> c >> d)) {break;}
            positions[a][0] = b;
            positions[a][1] = c;
            }
    }
                      
    return positions;
}

std::vector<std::vector<double> > angles_init(int agent_number, 
    double box_size, int dim, double angle_interval_low, double angle_interval_high,
    std::mt19937& gen, std::string init_mode, int delayTime, double noise_strength)
{
    std::vector<std::vector<double> > angles(agent_number, 
        std::vector<double>(dim-1));
    // saves current angles
 
    if (init_mode == "random_uniform")
    {
        // RNG: draw from [0;box_size)
        std::uniform_real_distribution<> dis(angle_interval_low, angle_interval_high);
        // CAUTION: for generalization to 3D adapt random interval
        
        // draw for each agent, for each dimension one random number
        for (int agent_ind = 0; agent_ind < agent_number; agent_ind++)
        {
            for (int dim_ind = 0; dim_ind < dim-1; dim_ind++)
            {
                angles[agent_ind][dim_ind] = dis(gen);
            }
        }
    }

    if (init_mode == "continue")
    {
        std::ifstream inputFile("../dataContinue/snapshot/delay_"+std::to_string(delayTime)+"/snapshot_eta_"+std::to_string(noise_strength)+"_delay_"+std::to_string(delayTime)+".txt");
        
        std::string line;


        while (std::getline(inputFile, line)) 
            {
            std::istringstream iss(line);
            double a, b, c, d;
            if(!(iss >> a >> b >> c >> d)) {break;}
            {
            angles[a][0] = d;
            }

            }
    } return angles;
}
