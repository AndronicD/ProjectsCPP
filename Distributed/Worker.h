#include<iostream>
using namespace std;

class Worker{
    public: 
        int rank;
        int coordonator;
        int **topology;
        int sizes[4];

        int *vectorPrimit;
        int size_received;
        int *vectorTrimis;

        Worker();
        ~Worker();
        void recieveCoordonator();
        void recieveTopology();

        void recieveVector();
        void changeVector();
        void sendVector();
};