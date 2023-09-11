#include <iostream>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mt;

void* routine(void* arg) {
    for (int i=0; i<10e6; i++) {
        pthread_mutex_lock(&mt);
        mails++;
        pthread_mutex_unlock(&mt);
    }
    return NULL;
}

int main() {
    pthread_t th[4];
    int i;
    pthread_mutex_init(&mt, NULL);
    for (i=0;i<4;i++) {
        if (pthread_create(th+i, NULL, &routine, NULL) != 0) {
            perror("Erro ao criar thread...\n");
            return 1;
        }
        std::cout << "thread " << i << " iniciada...\n";
        // aqui serializa código, pois criar thread i e espera ela finalizar com join, no mesmo loop
        /*
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
        std::cout << "thread " << i << " finalizada...\n";
        */
    }
    for (i=0; i<4; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
        std::cout << "thread " << i << " finalizada...\n";
    }
    pthread_mutex_destroy(&mt);
    std::cout << "quandos e-mails: " << mails << std::endl;
    return 0;
}