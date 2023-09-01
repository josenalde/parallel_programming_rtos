#include <cstdio>
#include <pthread.h>
#include <cstdlib>

//limite de threads em Linux definda em /proc/sys/kernel/threads-max
 
/*Rotina que será executada pelos dois threads*/
void * routine(void *arg);
 
int main (int argc, char *argv[])
{
    pthread_t thread_idA;
    pthread_t thread_idB;
    void * thread_res;
    int rstatus;
    
    /*tenta iniciar o thread, indicando a função 'routine' e passando como argumento a string "Thread A"*/
    rstatus = pthread_create (&thread_idA, NULL, routine, (void*)("Thread A"));
 
    /*verificar se ocorreu algum erro na chamada de pthread_create*/
    if(rstatus != 0)
    {
        printf ("Erro ao criar o thread A.\n");
        exit(EXIT_FAILURE);
    }
 
    printf ("Thread A criado com sucesso!\n");
 
    /*tenta iniciar o thread, indicando novamente a função 'routine' e passando como argumento a string "Thread B"*/
    rstatus = pthread_create (&thread_idB, NULL, routine, (void*)("Thread B"));
 
    /*verificar se ocorreu algum erro na chamada de pthread_create*/
    if(rstatus != 0)
    {
        printf ("Erro ao criar o thread B.\n");
        exit(EXIT_FAILURE);
    }
 
    printf ("Thread B criado com sucesso!\n");
 
 
    /*aguarda finalização do thread A identificado por thread_idA. O retorno é passado pelo ponteiro thread_res*/
    rstatus = pthread_join (thread_idA, &thread_res);
 
    if(rstatus != 0)
    {
        printf ("Erro ao aguardar finalização do thread A.\n");
        exit(EXIT_FAILURE);
    }
 
    printf ("Thread A finalizado! Retorno = %s\n", (char *)thread_res);
 
 
    /*aguarda finalização do thread B identificado por thread_idB. O retorno é passado pelo ponteiro thread_res*/
    rstatus = pthread_join (thread_idB, &thread_res);
 
    if(rstatus != 0)
    {
        printf ("Erro ao aguardar finalização do thread B.\n");
        exit(EXIT_FAILURE);
    }
 
    printf ("Thread B finalizado! Retorno = %s\n", (char *)thread_res);
 
    return 0;
}
 
void * routine(void *arg)
{
    int contador = 10;
 
    /*procedimento para decrementar um contador e exibir o seu valor*/
    while(contador--)
    {
        printf("%s: %i\n", (char *)arg, contador);
        //sched_yield();
    }
 
    /*finaliza a função retornando o argumento que foi recebido*/
    pthread_exit(arg);
    return NULL;
}