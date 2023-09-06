#include <iostream>
#include <cstdlib>
#include <ctime>

#define MAX 1000000
int v[MAX];
int length = MAX;
int count;

using namespace std;

int count3s();
int main() {
    srand(time(NULL));
    for (int i=0; i<length; i++) {
        v[i] = rand() % 4;
        //cout << v[i] << ",";
    }
    cout << endl << count3s() << " found";
    return 0;
}

int count3s() {
    int i;
    count=0;
    clock_t tstart, tend;
    tstart = clock(); //tempo de cpu (cpu time), não inclui delays nem espera de IO
    for(i=0;i<length;i++) {
        if(v[i]==3) count++;
    }
    tend = clock();
    double trun;
    trun = (double)(tend - tstart)/(double)CLOCKS_PER_SEC;
    cout.precision(12);
    cout << fixed << "Time: " << trun << endl;
    return count;
}