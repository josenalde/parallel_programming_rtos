#include <omp.h>
#include <iostream>

int main() {
    #pragma omp parallel // diretiva do compilador
    {
        std::cout << "Hello world parallel" << std::endl;
        //as exibições coincidem com o número default de threads
        //pode-se mudar no linux com export OMP_NUM_THREADS=<num>
        //e no windows terminal com set OMP_NUM_THREADS=<num> para o user
        //e com opção /m também para o system (geral)
    }
    return 0;
}