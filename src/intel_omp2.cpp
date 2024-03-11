#include <omp.h>
#include <iostream>
using namespace std;

static long num_steps = 1000000;
double step;

#define NUM_THREADS 2

int main() {
    int i;
    double x, pi, sum = 0.0;

    step = 1.0 / (double) num_steps;

    double tstart = omp_get_wtime();
    for (i=0; i<num_steps; i++) {
        x = (i + 0.5)*step;
        sum += 4.0/(1.0+x*x);
    }
    double tend = omp_get_wtime();
    pi = step * sum;
    cout.precision(12);
    cout << "pi serial: " << fixed << pi << " in " << tend-tstart << " seconds " << endl;

    // ----------- pi with open mp ----------------

    int nthreads, sum[NUM_THREADS];
    double tstart = omp_get_wtime();
    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    {
        int i, id, nthrds;
        double x_omp;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads(); // round robin distribution (deck of cards)
        cout << "num of threads of this machine: " << nthrds << endl;
        // each thread gets a subset of iterations to compute
        if (id == 0) nthreads = nthrds; //visible after the parallel region, because OS may give ou fewer number of threads
        for (i=id, sum[id]=0.0; i<num_steps; i+=nthrds) {
            x_omp = (i + 0.5)*step;
            sum[id] += 4.0/(1.0+x_omp*x_omp); // false cache sharing occurs. Not scalable!
        }
    }
    double tend = omp_get_wtime();
    for (i=0, pi=0.0; i<nthreads;i++) pi += sum[i]*step;
    cout << "pi omp: " << fixed << pi << " in " << tend-tstart << " seconds " << endl;
    return 0;
}

    
