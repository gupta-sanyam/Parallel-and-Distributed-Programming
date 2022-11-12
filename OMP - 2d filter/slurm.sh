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
#SBATCH --job-name="base"
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=20
#SBATCH --time=01:00:00

iteration=(1 2 3)

for iter in ${!iteration[@]}
do
    echo "** iteration : ${iteration[$iter]} **"
    threads=(2 4 6 8 10)
    factor=${threads[$iter]}
    m=$((factor * 10000))
    n=10000
    echo "size = $n * $m"
    echo "without omp"
    ./withoutomp $n $m
    ./withoutomp $n $m
    ./withoutomp $n $m
    ./withoutomp $n $m
    ./withoutomp $n $m

    echo "With OMP"
    for element in ${threads[@]}
    do
            echo "threads = ${element}"
            OMP_NUM_THREADS=${element} ./a0 $n $m
            OMP_NUM_THREADS=${element} ./a0 $n $m
            OMP_NUM_THREADS=${element} ./a0 $n $m
            OMP_NUM_THREADS=${element} ./a0 $n $m
            OMP_NUM_THREADS=${element} ./a0 $n $m
    done
    echo "done"
done
