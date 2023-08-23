#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define N 64
#define T 4

int v[N], count;

void* count3s_thread(void* id) {
    int my_id = (int) id;
    cout << "criei thread " << my_id;
    pthread_t thId = pthread_self();
    printf(" with ID: %d\n", thId);
    int length_per_thread = N/T;
    int start = my_id*length_per_thread;
    for (int i=start; i<start+length_per_thread;i++) {
        if (v[i] == 3) count++;
    }
    return NULL; //evita warning
}

int main()  {
    pthread_t tid[T]; //array de threads
    int err;
    srand(time(NULL));
    for (int i=0; i<N; i++) {
        v[i] = rand() % 4;
        cout << v[i] << ",";
    }

    for (int i=0; i<T; i++) {
        err = pthread_create(&tid[i], NULL, count3s_thread, (void*) i);
    }
    for (int i=0; i<T; i++) {
        err  = pthread_join(tid[i], NULL);
    }
    
    cout << endl << count << " found";
    return 0;
}