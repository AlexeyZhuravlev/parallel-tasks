#!/bin/bash
#SBATCH --time=30
mpirun --map-by ppr:1:node ./life
