#include <iostream>
#include <cstdlib>
#include <ctime>

#define MAX 50
int v[MAX];
int length = MAX;
int count;

using namespace std;

int count3s();
int main() {
    srand(time(NULL));
    for (int i=0; i<length; i++) {
        v[i] = rand() % 4;
        cout << v[i] << ",";
    }
    cout << endl << count3s() << " found";
    return 0;
}

int count3s() {
    int i;
    count=0;
    for(i=0;i<length;i++) {
        if(v[i]==3) count++;
    }
    return count;
}