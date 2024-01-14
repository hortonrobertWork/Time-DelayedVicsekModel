// This function is a useful for producing random seeds
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

std::vector<std::vector<double> > initialisePosition(int agentNumber, 
    double boxSize, int dim, std::mt19937& gen, std::string initialisationMode, int delayTime, double noiseStrength)
{
    std::vector<std::vector<double> > positions(agentNumber, 
        std::vector<double>(dim));
    // saves current positions for each agent
    
    if (initialisationMode == "random_uniform")
    {
        // RNG: draw from [0;boxSize)
        std::uniform_real_distribution<> dis(0.0, boxSize);

        // draw for each agent, for each dimension one random number
        for (int agentIndex = 0; agentIndex < agentNumber; agentIndex++)
        {
            for (int dimensionIndex = 0; dimensionIndex < dim; dimensionIndex++)
            {
                positions[agentIndex][dimensionIndex] = dis(gen);
            }
        }
    }

    if (initialisationMode == "continue")
    { 
        std::ifstream inputFile("../dataContinue/snapshot/delay_"+std::to_string(delayTime)+"/snapshot_eta_"+std::to_string(noiseStrength)+"_delay_"+std::to_string(delayTime)+".txt");
        
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

std::vector<std::vector<double> > initialiseAngles(int agentNumber, 
    double boxSize, int dim, double angleIntervalHigh, double angleIntervalLow,
    std::mt19937& gen, std::string initialisationMode, int delayTime, double noiseStrength)
{
    std::vector<std::vector<double> > angles(agentNumber, 
        std::vector<double>(dim-1));
    // saves current angles
 
    if (initialisationMode == "random_uniform")
    {
        // RNG: draw from [0;boxSize)
        std::uniform_real_distribution<> dis(angleIntervalHigh, angleIntervalLow);
        // CAUTION: for generalization to 3D adapt random interval
        
        // draw for each agent, for each dimension one random number
        for (int agentIndex = 0; agentIndex < agentNumber; agentIndex++)
        {
            for (int dimensionIndex = 0; dimensionIndex < dim-1; dimensionIndex++)
            {
                angles[agentIndex][dimensionIndex] = dis(gen);
            }
        }
    }

    if (initialisationMode == "continue")
    {
        std::ifstream inputFile("../dataContinue/snapshot/delay_"+std::to_string(delayTime)+"/snapshot_eta_"+std::to_string(noiseStrength)+"_delay_"+std::to_string(delayTime)+".txt");
        
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
