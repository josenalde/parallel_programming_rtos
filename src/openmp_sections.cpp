#include <iostream>
#include <omp.h>
using namespace std;

void taskA() {
  cout << "na taskA com " << omp_get_thread_num() << "\n";
  int x = 10, s = 0;
  for (int i = 0; i < x; i++) {
     s+= x;
  }
  cout << "Finalizei o calculo na taskA com x = " << x << endl;
}

void taskB() {
  cout << "na taskB com " << omp_get_thread_num() << "\n";
  int x = 20, s = 0;
  for (int i = 0; i < x; i++) {
     s += x;
  }
  cout << "Finalizei o calculo na taskB com x = " << x << endl;
}

void taskC() {
  cout << "na taskC com " << omp_get_thread_num() << "\n";
}

void taskD() {
  cout << "na taskD com " << omp_get_thread_num() << "\n";
}

void taskAll() {
  cout << "na taskAll com " << omp_get_thread_num() << "\n";
}

int main(void) {

/* no caso tarefas independentes pode ser assim */
// cria time (team) de threads
#pragma omp parallel 
{
  #pragma omp sections
  {
    #pragma omp section
    {
      taskA();  // apenas uma thread
      taskB();  // apenas uma thread, a mesma de taskA
    }
    #pragma omp section
    taskC();  // apenas uma thread
    #pragma omp section
    taskD();  // apenas uma thread
  }
  taskAll();  // todas as threads
}
  // sem nowait (barreira implícita)
  cout << "na thread main()....\n";
  return 0;
}