#include <omp.h>
#include <iostream>
#include <pthread.h>

int main() {
    std::cout << "Outside paralell region\n";
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        std::cout << "I am thread " << id << " fom " << nt << std::endl;
    }
    std::cout << "Outside paralell region\n";
    return 0;
}