
import os

text = ('''#!/bin/bash
# execution cue
#SBATCH -p batch
# one node:
#SBATCH -N 1
# maximum number of tasks per node:
#SBATCH -n 1
#SBATCH -t 48:00:00
#SBATCH --mem-per-cpu=1000
# file for error messages
#SBATCH -e output=Slurmjob_%N_%j.stderr
# file for normal output
#SBATCH --output=Slurmjob_%N_%j.out
cd /scratch/fermi/horton/Paper/main1/src/
''')

# delay = (3,5)

# systemSizes = 32
delay = 3
size  = 256

# for delay in delay:
# pathDir = "data/observables/"+str(size)+"/delay_"+str(delay)+"/"
# os.mkdir(pathDir)

noise = 0.665

for i in range(5):
    with open('../Slurm/slurmJob_delay_'+str(delay)+'_size_'+str(size)+'_noise_'+str('{0:.4f}'.format(noise)), 'w') as f:           
        f.write(text+'./main.out '+str(2*size*size)+' '+str('{0:.4f}'.format(noise))+' '+str(delay)+' ../data7/observables/delay_'+str(delay)+'/ ../data7/snapshot/delay_'+str(delay)+'/')
        noise += 0.0002










