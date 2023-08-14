#include <iostream>
#include <cstdlib>

int fcomp (const void * a, const void * b) //what is it returning?
{
   if (*(double*)a > *(double*)b) return -1;
   else return 1;
}

int main() {
    int n = 3;
    
    /* O void* é um ponteiro para algo indeterminado. Quando você for usá-lo para algo, 
    vai precisar de um cast para o tipo real. O tamanho do void* é o tamanho de um ponteiro normal (ou do maior ponteiro possível, 
    no caso de arquiteturas onde existem ponteiros de tamanhos diferentes). O void* ocupa espaço na memória, já o void não.*/

    void *gp = malloc(n*sizeof(int)); //malloc retorna void*
    int *ip = (int *) gp;
    
    int v[] = {1,10,20};
    ip = v;
    
    for (int i = 0; i < 3; i++) {
        std::cout << *(ip + i) << ", "; //equivalente a ip[i]
    }
    std::cout << "\n";
    // outro exemplo
    double notOrdered[] = {1,-2,10,20,0,-5,1,100,4};
    int sizeNotOrdered = sizeof(notOrdered)/sizeof(notOrdered[0]);
    qsort(notOrdered, sizeNotOrdered, sizeof(double), fcomp);
    for (int i = 0; i < sizeNotOrdered; i++) {
        std::cout << *(notOrdered + i) << ", "; //equivalente a ip[i]
    }
}
