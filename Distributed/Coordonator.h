#include<iostream>
#include<vector>
using namespace std;

class Coordonator{
    public: 
        int rank;
        int number_of_workers;
        int *workers;
        int **topology;
        
        int sizes[4];
        int clusterDistribution[4];
        
        int *vectorCluster;
        int *temp;

        int **distributionToWorkers;
        vector<int> gather;
        int gather_amount;

        Coordonator();
        ~Coordonator();
        void readWorkers();
        void sendWorker();

        void sendWorkersToClusters(int source, int destination, int tag, int *vect, int size);
        void receiveWorkersFromCluster(int tag, int source);
        void sendTopologyToWorkers();

        void equalDistribution(int vectorSize);
        void generateVector(int *vector, int size);
        
        void sendVector(int* vect, int size, int dest, int tag);
        void recieveVector(int tag, int source);
        void recieveVectorTemp(int tag, int source);

        void sendVectorToEachWorker();
        void receiveVectorFromEachWorker();
        
        void sendVectorGather(int dest, int tag);
        void recieveVectorGather(int tag, int source);
};