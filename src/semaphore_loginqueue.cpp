#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <ctime>
#include <cstdlib>

#define THREAD_NUM 16 //16 pessoas tentando simultaneamente conectar ao servidor
sem_t sm;

void* routine(void* args) {
    long cast_arg = (long)args;
    std::cout << "(" << cast_arg << ")" << " Esperando na fila...\n";
    sem_wait(&sm);
        std::cout << "(" << cast_arg << ")" << "Logado...\n";
        Sleep(rand() % 5 + 1); //dormir por tempo randômico (tempo logado)
        std::cout << "(" << cast_arg << ")" << "Saindo...\n";
    sem_post(&sm);
    return NULL;
}

int main() {
    pthread_t th[THREAD_NUM];
    //inicializar semaforo
    sem_init(&sm, 0, 12); //12 pessoas na fila de login no máximo (cenário gamer, webserver etc.)
    int i;
    for (i=0;i<THREAD_NUM; i++) {
        // int* a = malloc(sizeof(int));
        //*a = i; // neste caso passar a como parâmetro e na routine o conteúdo é *(int*)args e depois free(args)
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
