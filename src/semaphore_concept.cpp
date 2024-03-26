#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define THREAD_NUM 4
sem_t sm;

void* routine(void* args) {
    long cast_arg = (long)args;
    sem_wait(&sm);
        usleep(1000000);
        std::cout << "Oi, estou na thread " << cast_arg << std::endl;
    sem_post(&sm);
    return NULL;
}

int main() {
    pthread_t th[THREAD_NUM];
    //inicializar semaforo
    sem_init(&sm, 0, 1); //ultimo parametro valor inicial
    int i;
    for (i=0;i<THREAD_NUM; i++) {
        // int* a = malloc(sizeof(int));
        //*a = i; // neste caso passar a como parâmetro e na routine o conteúdo é *(int*)args
        if(pthread_create(&th[i], NULL, &routine, (void *) i) != 0) {
            perror("Erro ao criar thread");
        }
    }
    for (i=0;i<THREAD_NUM; i++) {
        if(pthread_join(th[i], NULL) != 0) {
            perror("Erro ao finalizar thread");
        }
    }
    sem_destroy(&sm);
    return 0;
}
