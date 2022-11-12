#!/bin/bash

####### PLANEX SPECIFIC - DO NOT EDIT THIS SECTION
#SBATCH --clusters=faculty
#SBATCH --partition=planex
#SBATCH --qos=planex
#SBATCH --account=cse570f21
#SBATCH --exclusive
#SBATCH --mem=64000
#SBATCH --output=%j.stdout
#SBATCH --error=%j.stderr

####### CUSTOMIZE THIS SECTION FOR YOUR JOB
#SBATCH --job-name="report"
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=1
#SBATCH --time=00:10:00

module load intel-mpi/2020.2
# if using Intel MPI add need this
export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so

for size in ${!gbs[@]}
do
    echo "** dataSize : ${gbs[$size]} GBs**"
    temp=$((10**5))
    n=$((gbs[$size] * temp))


    for element in ${threads[@]}
    do
            echo "threads = ${element}"
            srun --mpi=pmi2 ./a1 $n
            srun --mpi=pmi2 ./a1 $n
            srun --mpi=pmi2 ./a1 $n
            srun --mpi=pmi2 ./a1 $n
            srun --mpi=pmi2 ./a1 $n

    done
    echo "done"
done
