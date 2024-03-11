#include <omp.h>
#include <iostream>
using namespace std;

void f(int id, double *v) {
    cout << "id: " << id << endl;
    for (int i = 0; i < 8; i++) cout << "id: " << id << ", " << v[i] << endl;
    
}

int main() {
    double A[8] = {1,2,3,4,5,6,7,8};
    omp_set_num_threads(4);
    cout << "before parallelization, serial region\n";
    #pragma omp parallel
    {
        #pragma omp critical
        {
            int tid = omp_get_thread_num(); // local to the thread stack
            f(tid, A);
        }
    }
    cout << "joining after parallelization\n";
    return 0;
}