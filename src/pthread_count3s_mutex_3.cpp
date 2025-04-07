#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>

using namespace std;

//#define N 16
#define N 1000000
/*Processador Intel® Core™ i7-10510U quad-core com 2 threads por core: 
  Núcleo é um termo de hardware que descreve o número de unidades de processamento central independentes em um único componente de 
  computação (matriz ou chip).
  Um thread, ou thread de execução, é um termo de software para a sequência básica ordenada de instruções que pode ser passada
  ou processada por um único núcleo de CPU.
*/
#define T 8 //máximo para a CPU em questão

int v[N], count = 0;

pthread_mutex_t lock;
int private_count[T]{0};

void* count3s_thread(void* id) {
    int my_id =  *(int*)id;
    //cout << "criei thread " << my_id;
    pthread_t thId = pthread_self();
    //printf(" with ID: %d\n", thId);
    int length_per_thread = N/T;
    int start = my_id*length_per_thread;
    
    for (int i=start; i<start+length_per_thread; i++) {
        if (v[i] == 3) {
            private_count[my_id]++;
        }
    }
    pthread_mutex_lock(&lock);
        count += private_count[my_id];
    pthread_mutex_unlock(&lock);

    return NULL; //evita warning
}

int main()  {
    pthread_t tid[T]; //array de threads
    int err;
    srand(time(NULL));
    for (int i=0; i<N; i++) {
        v[i] = rand() % 4;
        //cout << v[i] << ",";
    }
    //init na thread principal
    pthread_mutex_init(&lock, NULL); // o segundo parâmetro pthread_mutexattr_t *attr = NULL significa que usará atributos padrão
    /*
        o atributo padrão é PTHREAD_MUTEX_NORMAL ou PTHREAD_MUTEX_DEFAULT: causa deadlock se thread tenta acessar lock sem antes ter havido unlock
        o atributo PTHREAD_MUTEX_RECURSIVE permite usar à thread usar o mesmo mutex "lockado". Ao ser criado o atríbuto é NORMAL. Depois muda
        com settype. Exemplo:
        pthread_mutexattr_t    attr;
        pthread_mutex_t         mutex;
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&mutex, &attr);
    */ 
    
    clock_t tstart, tend;
    tstart = clock();
    for (int i=0; i<T; i++) {
        err = pthread_create(&tid[i], NULL, count3s_thread, &i);
    }
    for (int i=0; i<T; i++) {
        err  = pthread_join(tid[i], NULL);
    }
    
    pthread_mutex_destroy(&lock);

    tend = clock();
    double trun;
    trun = (double)(tend - tstart)/(double)CLOCKS_PER_SEC;
    cout.precision(12);
    cout << fixed << "Time: " << trun << endl;
    cout << endl << count << " found";
    return 0;
}
