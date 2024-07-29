#include <omp.h>
#include <iostream>

int main() {
  std::cout << omp_get_max_threads() << std::endl;
  #pragma omp parallel num_threads(8) // diretiva do compilador
    {
        std::cout << "Hello world parallel from thread_id: " << omp_get_thread_num() << std::endl;
        //as exibições coincidem com o número default de threads
        //pode-se mudar no linux com export OMP_NUM_THREADS=<num>
        //e no windows terminal com set OMP_NUM_THREADS=<num> para o user
        //e com opção /m também para o system (geral)
    }
    return 0;
}
