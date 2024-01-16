# Time-Delayed Vicsek Model
---
This project contains the code required to simulate and perform some analysis on the time delayed Vicsek model. The initial simulation code used was adopted from [this](
https://gitlab.physik.uni-muenchen.de/activemattersim19/vicsek-model/-/tree/master) gitlab project. This code was adapted and revised to *accurately* describe the system.
The results produced from the simulation match that of the literature.
---
The project has the following directories
- data, contains the results produced from simulation
- dataContinue, contains the results from a simulation that was begun from user defined starting configuration
- evaluation, contains jupyter notebooks for the evaluation of the simulation output
- Plots, folder containing saved plots for use in reports
- Slurm, contains slurm files to submit to the cluster
- src, contains all the source code for the simulation
- utilityScripts, folder containing a few useful scripts
---
To begin, the source code can be compiled `g++ main.cpp -o main.out -O2`

To run, `./main.out [args]`

> Arg  1: Number of agents
> 
> Arg  2: Noise Strength
> 
> Arg  3: Delay Time
> 
> Arg  4: Output path
> 
> Arg  5: Snapshot Output Path
> 
> Arg  6: Animation Output Path
---
The algorithm make use of a subspacing mechanism. This mechanism segments the system into boxes, the same size as the interaction radius. When searching for neighbours, each agent will 
only look inside an area where it is possible to find an interacting neighbour and no further thus reducing the computation complexity from $N^2$ to order N. This significantly speeds up
the compuational time and allows us to simulate very large systems within a reasonable time.
