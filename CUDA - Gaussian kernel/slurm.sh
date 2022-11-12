#!/bin/bash

####### set memory that nodes provide (check CCR documentation, e.g., 32GB)
#SBATCH --mem=32000

####### make sure no other jobs are assigned to your nodes
#SBATCH --exclusive

####### further customizations
#SBATCH --job-name="a3"
#SBATCH --output=%j.stdout
#SBATCH --error=%j.stderr
#SBATCH --time=01:00:00
#SBATCH --nodes=1
#SBATCH --gres=gpu:tesla_v100-pcie-16gb:2

module load cuda/11.0
module load gcc/8.3.0

./a3 1024000 .005
./a3 2048000 .005
./a3 3072000 .005