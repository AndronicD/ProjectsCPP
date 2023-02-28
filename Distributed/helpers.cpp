#include <string>
#include <iostream>
#include "helpers.h"
using namespace std;

void log_send(int source, int dest){
    string mesaj("M(");
    mesaj += to_string(source);
    mesaj += ",";
    mesaj += to_string(dest);
    mesaj += ")";
    cout << mesaj << endl;
}

void copy_array(int *array1, int *array2, int size){
    for(int i = 0; i < size; i++){
        array1[i] = array2[i];
    }
}

void print_topology(int** topology, int* sizes, int proces){
    cout << proces << " -> ";
    for(int i = 0; i < 4; i++){
        cout << i << ":";
        for(int j = 0; j < sizes[i]; j++){
            cout << topology[i][j];
            if(sizes[i] == j + 1)
                cout << " ";
            else{
                cout << ",";
            }
        }
    }
    cout << endl;
}