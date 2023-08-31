/* adaptado de:
   GARCIA, F.D. Threads POSIX. 2017. Disponível em: <https://embarcados.com.br/threads-posix/>
                Acesso em: 31/08/2023
*/

#include <iostream>
#include <pthread.h>

/*Assinatura da rotina que será executada por cada thread*/
void * routine(void *arg);
 
int main (int argc, char *argv[]) {
    pthread_t thread_id;
    void * thread_res; //para guardar o resultado passado por routine 
                       //no exit, usado no JOIN
    int rstatus; // guardar status do create e do join
    
    /*tenta iniciar o thread, indicando a função 'routine' e passando como argumento a string "Embarcados é no TADS"*/
    rstatus = pthread_create (&thread_id, NULL, routine, (void*)("Embarcados é no TADS"));
 
    /*verificar se ocorreu algum erro na chamada de pthread_create*/
    if(rstatus != 0) {
        std::cout << "Erro ao criar o thread.\n";
        exit(EXIT_FAILURE);
    }
 
    std::cout << "Thread criado com sucesso!\n";
 
    /*aguarda finalização do thread identificado por thread_id. O retorno é passado pelo ponteiro thread_res*/
    rstatus = pthread_join (thread_id, &thread_res);
 
    /*verificar se ocorreu algum erro na chamada de pthread_join*/
    if(rstatus != 0) {
        std::cout << "Erro ao aguardar finalização do thread.\n";
        exit(EXIT_FAILURE);
    }
 
    /*exibe o valor de retorno da função 'routine'*/
    std::cout << "Thread finalizado! Retorno = " << (char *)thread_res << std::endl;
 
    return 0;
}
 
void * routine(void *arg) {
    /*exibe o argumento recebido*/
    std::cout << "Argumento: " << (char *)arg << std::endl;
    /*finaliza a função retornando o argumento que foi recebido que será lido por thread_res*/
    pthread_exit(arg);
    return NULL;
}