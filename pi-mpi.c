#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUMSTEPS 1000000

int main() {
    
    int i;
    double x, pi, sum = 0.0;
    struct timespec start, end;
    
    MPI_Init(NULL,NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    double step = 1.0/(double) NUMSTEPS;
    double local_sum = 0.0;

    for (i=world_rank;i<= NUMSTEPS; i+=world_size){
        x = (i *0.5) * step;
        local_sum += 4.0/(1.0+x*x);
    }
    
    MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   
    clock_gettime(CLOCK_MONOTONIC, &end);
    u_int64_t diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    
    if (world_rank == 0) {
        pi = step * sum;
        printf("PI is %.20f\n",pi);
        printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);
    }

    
    MPI_Finalize();

        return 0;
}

