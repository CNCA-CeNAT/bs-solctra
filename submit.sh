#!/bin/bash
#SBATCH -J solctra           # job name
#SBATCH -o solctra.o%j       # output and error file name (%j expands to jobID)
#SBATCH -n 2              # total number of mpi tasks requested
#SBATCH -N 1              # total number of mpi tasks requested
#SBATCH -p normal     # queue (partition) -- normal, development, etc.
#SBATCH -t 01:00:00        # run time (hh:mm:ss) - 1.5 hours

####SBATCH --mail-user=ldcl289@gmail.com
####SBATCH --mail-type=begin  # email me when the job starts
####SBATCH --mail-type=end    # email me when the job finishes

export RESULTS_DIR=$SCRATCH
export OMP_NESTED=true
export OMP_NUM_THREADS=4
export KMP_SETTINGS=TRUE
#export OMP_PROC_BIND=spread
#export OMP_PLACES=cores
#export KMP_AFFINITY=verbose,norespect,compact


ibrun tacc_affinity ./solctra -length 16 -particles particles.txt -id ${SLURM_JOBID}
#ibrun tacc_affinity ./solctra.serial -length 1 -particles particles.txt -id serial_${SLURM_JOBID}

mv results_${SLURM_JOBID} $RESULTS_DIR/solctra/results/.
mv stdout_${SLURM_JOBID}.log $RESULTS_DIR/solctra/results/.
mv solctra.o${SLURM_JOBID} $RESULTS_DIR/solctra/results/.
