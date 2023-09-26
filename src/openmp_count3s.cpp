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
int length = N / T;

int count3s() {
    int i, count_p, start;
    count = 0; //compartilhada
        
    #pragma omp parallel num_threads(T) \
     shared(v, count, length) private(count_p, start)
    {
        count_p = 0;
        int my_rank = omp_get_thread_num();
        start = my_rank*length;
        //int thread_count = omp_get_num_threads(); // replit default 6
        //cout << thread_count << ":" << endl;
        cout << "thread: " << my_rank << "from index " << start << " to " << start + length << endl;
        #pragma omp parallel for private(i)
          for (i = start; i < start + length; i++) {
               if (v[i] == 3) count_p++;
          }
        //equivale a mutex ou semáforo binário
        #pragma omp critical
        count += count_p;
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