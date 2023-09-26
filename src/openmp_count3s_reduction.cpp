//Por Josenalde Oliveira, setembro 2023
#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define N 16
/*Processador Intel® Core™ i7-10510U quad-core com 2 threads por core: 
  Núcleo é um termo de hardware que descreve o número de unidades de processamento central independentes em um único componente de 
  computação (matriz ou chip).
  Um thread, ou thread de execução, é um termo de software para a sequência básica ordenada de instruções que pode ser passada
  ou processada por um único núcleo de CPU.
*/
#define T 8 // número de threads máximo

int v[N], count;
int length = N;

int count3s() {
    int i;
    count = 0; //compartilhada 
    // aqui só tem uma thread
    int my_rank = omp_get_thread_num();
    cout << "thread " << my_rank << " de " << omp_get_num_threads() << endl;
    // aqui vai abrir o time de threads para o for
    #pragma omp parallel for reduction(+: count) private(i) num_threads(T)
    for (i = 0; i < length; i++) {
        count += (v[i]==3)?1:0;
        my_rank = omp_get_thread_num();
        cout << "thread " << my_rank << " de " << omp_get_num_threads() << endl;
    }
    return count;
}


int main()  {
    srand(time(NULL));
    for (int i=0; i<N; i++) {
        v[i] = rand() % 4;
        cout << v[i] << ",";
    }
    clock_t tstart, tend;
    tstart = clock();
      int c = count3s();
    tend = clock();
    
    double trun;
    trun = (double)(tend - tstart)/(double)CLOCKS_PER_SEC;
    
    cout.precision(12);
    cout << fixed << "Time: " << trun << endl;
    cout << endl << c << " found";
    return 0;
}