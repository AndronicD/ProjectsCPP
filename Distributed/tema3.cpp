#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "Worker.cpp"
#include "Coordonator.cpp"
#include "helpers.h"
#include <unistd.h>

#define NUMBER_OF_CLUSTERS 4
using namespace std;

int main(int argc, char *argv[])
{
	int rank, nProcesses;
    int vectorSize;
    vectorSize = atoi(argv[1]);
    int *vectorPrelucrare;
    int **vectorFinal;
    vector<int> gather_all;

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;
    Coordonator coord;
    Worker worker;


	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    if(rank < 4){
        coord = Coordonator();
        coord.rank = rank;
        coord.readWorkers();
        // cout << "Procesul:" << coord.rank << " cu workerii:";
        // for(int i = 0; i < coord.number_of_workers; i++){
        //     //cout << " " << coord.workers[i];
        // }
        // cout << endl;
        coord.sizes[rank] = coord.number_of_workers;
        coord.sendWorker();
        coord.topology = (int**)malloc(NUMBER_OF_CLUSTERS*sizeof(int*));

        coord.topology[rank] = (int*)malloc(coord.number_of_workers * sizeof(int));
        copy_array(coord.topology[rank], coord.workers, coord.number_of_workers);
    }
    else{
        worker = Worker();
        worker.rank = rank;
        //printf("Proces %d\n", worker.rank);
        worker.recieveCoordonator();
        worker.topology = (int**)malloc(NUMBER_OF_CLUSTERS*sizeof(int*));
        //cout << "Worker:" << worker.rank << " cu coordonatorul: " << worker.coordonator << endl;
    }

    // 0 -> 2 -> 3 -> 1
    if(rank == 0){
        coord.sendWorkersToClusters(0, 2, 0, coord.workers, coord.number_of_workers);
        cout << endl;
    }
    if(rank == 2){
        //sleep(5);
        coord.receiveWorkersFromCluster(0, 0);
        //cout << endl;
        coord.sendWorkersToClusters(2, 3, 0, coord.topology[0], coord.sizes[0]);
        //cout << endl;
    }
    if(rank == 3){
        //sleep(5);
        coord.receiveWorkersFromCluster(0, 2);
        coord.sendWorkersToClusters(3, 1, 0, coord.topology[0], coord.sizes[0]);
    }
    if(rank == 1){
        coord.receiveWorkersFromCluster(0, 3);
    }

    // 2 -> 0 apoi 2 -> 3 -> 1
    if(rank == 2){
        coord.sendWorkersToClusters(2, 0, 2, coord.workers, coord.number_of_workers);
        coord.sendWorkersToClusters(2, 3, 2, coord.workers, coord.number_of_workers);
    }

    if(rank == 0){
        coord.receiveWorkersFromCluster(2, 2);
    }

    if(rank == 3){
        coord.receiveWorkersFromCluster(2, 2);
        coord.sendWorkersToClusters(3, 1, 2, coord.topology[2], coord.sizes[2]);
    }

    if(rank == 1){
        coord.receiveWorkersFromCluster(2, 3);
    }

    // 3 -> 2 -> 0 apoi 3 -> 1
    if(rank == 3){
        coord.sendWorkersToClusters(3, 1, 3, coord.workers, coord.number_of_workers);
        coord.sendWorkersToClusters(3, 2, 3, coord.workers, coord.number_of_workers);
    }

    if(rank == 1){
        coord.receiveWorkersFromCluster(3, 3);
    }

    if(rank == 2){
        coord.receiveWorkersFromCluster(3, 3);
        coord.sendWorkersToClusters(2, 0, 3, coord.topology[3], coord.sizes[3]);
    }

    if(rank == 0){
        coord.receiveWorkersFromCluster(3, 2);
    }
    
    // 1 -> 3 -> 2 -> 0 
    if(rank == 1){
        coord.sendWorkersToClusters(1, 3, 1, coord.workers, coord.number_of_workers);
    }

    if(rank == 3){
        coord.receiveWorkersFromCluster(1, 1);
        coord.sendWorkersToClusters(3, 2, 1, coord.topology[1], coord.sizes[1]);
    }

    if(rank == 2){
        coord.receiveWorkersFromCluster(1, 3);
        coord.sendWorkersToClusters(2, 0, 1, coord.topology[1], coord.sizes[1]);
    }

    if(rank == 0){
        coord.receiveWorkersFromCluster(1, 2);
    }

    if(rank < 4){
        print_topology(coord.topology, coord.sizes, coord.rank);
        coord.sendTopologyToWorkers();
    }
    else{
        worker.recieveTopology();
        print_topology(worker.topology, worker.sizes, worker.rank);
    }

    ///////////////////////////////////////////////

    if(rank < 4){
        coord.equalDistribution(vectorSize);
    }

    if(rank == 0){
        vectorPrelucrare = (int*)calloc(vectorSize, sizeof(int));
        coord.generateVector(vectorPrelucrare, vectorSize);

        vectorFinal = (int**)malloc(NUMBER_OF_CLUSTERS *  sizeof(int*));
        
        for(int i = 0; i < 4; i++){
            vectorFinal[i] = (int*)calloc(coord.clusterDistribution[i], sizeof(int));
        }
    }

    int counter = 0;
    if(rank == 0){
        for(int i = 0; i < NUMBER_OF_CLUSTERS; i++){
            for(int j = 0; j < coord.clusterDistribution[i]; j++){
                vectorFinal[i][j] = vectorPrelucrare[counter];
                counter++;
            }
        }
    }

    //Trimite insusi
    if(rank == 0){
        coord.sendVector(vectorFinal[0], coord.clusterDistribution[0], 0, 0);
        coord.recieveVector(0, 0);
    }

    //Trimite la doi
    if(rank == 0){
        coord.sendVector(vectorFinal[2], coord.clusterDistribution[2], 2, 2);
    }

    if(rank == 2){
        coord.recieveVector(2, 0);
    }

    //Trimite la trei
    if(rank == 0){
        coord.sendVector(vectorFinal[3], coord.clusterDistribution[3], 2, 3);
    }

    if(rank == 2){
        coord.recieveVectorTemp(3, 0);
        coord.sendVector(coord.temp, coord.clusterDistribution[3], 3, 3);
        free(coord.temp);
    }

    if(rank == 3){
        coord.recieveVector(3, 2);
    }

    //Trimite la unu
    if(rank == 0){
        coord.sendVector(vectorFinal[1], coord.clusterDistribution[1], 2, 1);
    }

    if(rank == 2){
        coord.recieveVectorTemp(1, 0);
        coord.sendVector(coord.temp, coord.clusterDistribution[1], 3, 1);
        free(coord.temp);
    }

    if(rank == 3){
        coord.recieveVectorTemp(1, 2);
        coord.sendVector(coord.temp, coord.clusterDistribution[1], 1, 1);
        free(coord.temp);
    }

    if(rank == 1){
        coord.recieveVector(1, 3);
    }

    //////////////////////////////////////////////////////////////////////
    if(rank < 4){
        coord.sendVectorToEachWorker();
    }

    if(rank >= 4){
        worker.recieveVector();
    }

    if(rank >= 4){
        worker.changeVector();
    }

    if(rank >= 4){
        worker.sendVector();
    }

    if(rank < 4){
        coord.receiveVectorFromEachWorker();
    }
    /////////////////////////////////////////////////////////////////////////////

    //Trimite insusi
    if(rank == 0){
        coord.sendVectorGather(0, 0);
        coord.recieveVectorGather(0, 0);

        for(int i = 0; i < coord.gather_amount; i++){
            gather_all.push_back(coord.temp[i]);
        }
    }

    //Trimit de la 1 -> 3 -> 2 -> 0
    if(rank == 1){
        coord.sendVectorGather(3, 1);
    }

    if(rank == 3){
        coord.recieveVectorGather(1, 1);
        coord.sendVector(coord.temp, coord.gather_amount, 2, 1);
    }

    if(rank == 2){
        coord.recieveVectorTemp(1, 3);
        coord.sendVector(coord.temp, coord.gather_amount, 0, 1);
    }

    if(rank == 0){
        coord.recieveVectorTemp(1, 2);
        for(int i = 0; i < coord.gather_amount; i++){
            gather_all.push_back(coord.temp[i]);
        }
    }

    //Trimit de la 2 -> 0
    if(rank == 2){
        coord.sendVectorGather(0, 2);
    }

    if(rank == 0){
        coord.recieveVectorGather(2, 2);

        for(int i = 0; i < coord.gather_amount; i++){
            gather_all.push_back(coord.temp[i]);
        }
    }

    //Trimit de la 3 -> 2 -> 0
    if(rank == 3){
        coord.sendVectorGather(2, 3);
    }

    if(rank == 2){
        coord.recieveVectorGather(3, 3);
        coord.sendVector(coord.temp, coord.gather_amount, 0, 3);
    }

    if(rank == 0){
        coord.recieveVectorTemp(3, 2);
        for(int i = 0; i < coord.gather_amount; i++){
            gather_all.push_back(coord.temp[i]);
        }
    }

    if(rank == 0){
        cout << "Rezultat: "; 
        for(int i = 0; i < gather_all.size(); i++){
            cout << gather_all.at(i) << " ";
        }
        cout << endl;
    }


    MPI_Finalize();
	return 0;
}