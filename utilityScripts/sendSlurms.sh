#!/bin/bash
for job in Slurm/*;
	do sbatch $job;
done
