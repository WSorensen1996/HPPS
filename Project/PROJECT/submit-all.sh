################################################################################################
# Modi guide: 

# Make sure to change the "MODI_TEST" value to '1' in the 'benchmark.c' file to get the right tests
# Open your modi server and upload the files to run the tests. 
# Run the following commands to compile the handed-in files. 
# The 'slurm-12345.out' file can now be cleaned with this python script and plotted via excel. 

#cc -c matlib.c -Wextra -Wall -pedantic -std=c99 -fopenmp -O1
#cc -c benchmark.c -Wextra -Wall -pedantic -std=c99 -fopenmp -O1
#cc -o benchmark benchmark.c matlib.o -lm -fopenmp
#sbatch --exclusive submit-all.sh

################################################################################################


#!/bin/bash


for n in {1..2}; do
OMP_NUM_THREADS=${n} ./benchmark ${n}
done
