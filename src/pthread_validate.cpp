#include <iostream>
#include <pthread.h>
using namespace std;

int thread_count; //global compartilhada entre as threads

void* test(void* rank)
{
    long my_rank = (long) rank;
    std::cout << "test from thread " << my_rank << " of " << thread_count;
    pthread_t thId = pthread_self(); //obter id da thread
    printf(" with ID: %d\n", thId);
    //std::cout << "Thread Id from thread function : " << thId << std::endl; //cout não imprime pthread_t
    return NULL;
}

int main()
{
    long thread;
    pthread_t* thread_handles;
    thread_count = 4;

    thread_handles = (pthread_t* ) malloc(thread_count * sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, test, (void*) thread);
    }
    cout << "test from main thread ..." << endl;

    for (thread = 0; thread < thread_count; thread++) 
        pthread_join(thread_handles[thread], NULL);
    
    free(thread_handles);
    return 0;
}