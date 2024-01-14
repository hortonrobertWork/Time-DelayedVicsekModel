// command line parsing handle
int parseInput(int argc, char* argv[], int& agentNumber, std::string& outputPath, std::string& outputSnapshot,
        std::string& outputAnimation, double& noiseStrength, int& delayTime)

{
    if (argc < 2 || argc > 14) {
        std::cerr << "\n\t\t---ERROR---\nCheck number of input arguments.\n\n"
         "Usage:\n"
         "\tArg  1: <AGNTNO>\tNumber of agents in the Vicsek model (integer)\n"
         "\tArg  2: <NOISE>\tNoise Strenght\n"
         "\tArg  3: <NOISE>\tDelay Time\n"
         "\tArg  4: <OUTPUT>\tOutput path)\n"
         "\tArg  5: <SNAPSHOT>\tSnapshot Output Path"
         "\tArg  6: <SNAPSHOT>\tAnimation Output Path"
         << std::endl;
        return 0;

    } else {
        if (argc >= 2) {
            agentNumber = atoi(argv[1]);
        }
        if (argc >= 3) {
            noiseStrength = atof(argv[2]);
        }
        if (argc >= 4) {
            delayTime = atoi(argv[3]);
        }
        if (argc >= 5) {
            outputPath = argv[4];
        }
        if (argc >= 6) {
            outputSnapshot = argv[5];
        }
        if (argc >=7) {
            outputAnimation = argv[6];
        } 

    }
    return 1;
}

// create output file handle
void initialiseOutputFile(std::ofstream& outputfile, int agentNumber, std::string outputPath,
                 double noiseStrength, int delayTime)
{

    std::string bs = std::string(outputPath)
        + "out_N_" + std::to_string(agentNumber)
        + "_delayTime_" + std::to_string(delayTime) 
        + "_eta_" + std::to_string(noiseStrength)       
        + ".txt";
    const char* filename = bs.c_str();

    outputfile.open(filename, std::ofstream::trunc);  
    outputfile << "#parameter" << std::endl;
    outputfile << "noiseStrength=" << noiseStrength << std::endl;
    outputfile << "delayTime=" << delayTime << std::endl;
    outputfile << "\ntime\tOrder\tOrder2\tOrder4" << std::endl;
}

void initialiseOutputFileSnapshot(std::ofstream& outputfile_snapshot, std::string outputSnapshot,
                 double noiseStrength, int delayTime){

    std::string bs = std::string(outputSnapshot)
        + "snapshot_delay_" + std::to_string(delayTime)   
        + "_eta_" + std::to_string(noiseStrength)   
        + ".txt";
    const char* filename = bs.c_str();

    outputfile_snapshot.open(filename, std::ofstream::trunc);  
}

void initialiseOutputFileAnimation(std::ofstream& outputfileAnimation, std::string outputAnimation,
                 double noiseStrength, int delayTime){

    std::string bs = std::string(outputAnimation)
        + "animationData_delay_" + std::to_string(delayTime)   
        + "_eta_" + std::to_string(noiseStrength)   
        + ".txt";
    const char* filename = bs.c_str();

    outputfileAnimation.open(filename, std::ofstream::trunc);  
}
