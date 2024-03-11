#include <omp.h>
#include <iostream>
using namespace std;

static long num_steps = 1000000;

#define NUM_THREADS 2

int main() {
    int i, nthreads;
    double pi = 0.0;
    double step = 1.0 / (double) num_steps;
    double tstart = omp_get_wtime();
    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel
    {
        int i, id, nthrds;
        double x, sum; //each thread has its copy of sum
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads(); // round robin distribution (deck of cards)
        if (id == 0) nthreads = nthrds; //visible after the parallel region, because OS may give ou fewer number of threads
        for (i=id, sum=0.0; i<num_steps; i+=nthreads) {
            x = (i + 0.5)*step;
            sum += 4.0/(1.0+x*x); 
        }
        #pragma omp critical // critical region (mutual exclusion)
            pi += sum * step; //shared value pi
    }
    double tend = omp_get_wtime();
    cout << "pi omp critical: " << fixed << pi << " in " << tend-tstart << " seconds " << endl;
    return 0;
}

    
