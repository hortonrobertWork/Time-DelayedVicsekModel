// command line parsing handle
int parse_input(int argc, char* argv[], int& agent_number, std::string& output_path, std::string& output_snapshot,
        double& noise_strength, int& delayTime)

{
    if (argc < 2 || argc > 14) {
        std::cerr << "\n\t\t---ERROR---\nCheck number of input arguments.\n\n"
         "Usage:\n"
         "\tArg  1: <AGNTNO>\tNumber of agents in the Vicsek model (integer)\n"
         "\tArg  2: <NOISE>\tNoise Strenght\n"
         "\tArg  3: <OUTPUT>\tOutput path (string, default: \"../data/\")\n"
         "\tArg  4: <SNAPSHOT>\tSnapshot Output Path (string, default: \"..data/snapshot\")"
         << std::endl;
        return 0;

    } else {
        if (argc >= 2) {
            agent_number = atoi(argv[1]);
        }
        if (argc >= 3) {
            noise_strength = atof(argv[2]);
        }
        if (argc >= 4) {
            delayTime = atoi(argv[3]);
        }
        if (argc >= 5) {
            output_path = argv[4];
        }
        if (argc >= 6) {
            output_snapshot = argv[5];
        } 

    }
    return 1;
}

// create output file handle
void output_init(std::ofstream& outputfile, int agent_number, std::string output_path,
                 double noise_strength, int delayTime)
{

    std::string bs = std::string(output_path)
        + "out_N_" + std::to_string(agent_number)
        + "_delayTime_" + std::to_string(delayTime) 
        + "_eta_" + std::to_string(noise_strength)       
        + ".txt";
    const char* filename = bs.c_str();

    outputfile.open(filename, std::ofstream::trunc);  
    outputfile << "#parameter" << std::endl;
    outputfile << "noise_strength=" << noise_strength << std::endl;
    outputfile << "delayTime=" << delayTime << std::endl;
    outputfile << "\ntime\tOrder\tOrder2\tOrder4" << std::endl;
}

void output_init_snapshot(std::ofstream& outputfile_snapshot, std::string output_path,
                 double noise_strength, int delayTime){

    std::string bs = std::string(output_path)
        + "snapshot_eta_" + std::to_string(noise_strength)   
        + "_delay_" + std::to_string(delayTime)   
        + ".txt";
    const char* filename = bs.c_str();

    outputfile_snapshot.open(filename, std::ofstream::trunc);  
}
