#include "mpi.h"
#include "Coordonator.h"
#include <fstream>
#include <string>
#include <iostream>
#include "helpers.h"
#define min(a,b) (((a)<(b))?(a):(b))

Coordonator::Coordonator(){};
Coordonator::~Coordonator(){};

void Coordonator::readWorkers(){

    string fisier("cluster");
    fisier += to_string(this->rank);
    fisier += ".txt";

    ifstream myfile(fisier);

    string mystring;
    if ( myfile.is_open() ) {     
        myfile >> mystring;
        this->number_of_workers = stoi(mystring);
        this->workers = (int*)calloc(number_of_workers, sizeof(int));

        for(int i = 0; i < number_of_workers; i++){
            myfile >> mystring;
            this->workers[i] = stoi(mystring);
        }
    }
}

void Coordonator::sendWorker(){
    //int MPI_Send(void* data, int count, MPI_DsendWorkersToClusters(int source, int destination, int tag, int *vect, int size)atatype datatype, int destination, int tag, MPI_Comm communicator)
    for(int i = 0; i < this->number_of_workers; i++){
        MPI_Send(&this->rank, 1, MPI_INT, this->workers[i], 0, MPI_COMM_WORLD);
        log_send(this->rank,this->workers[i]);
    }
}

void Coordonator::sendWorkersToClusters(int source, int destination, int tag, int *vect, int size){
    MPI_Send(vect, size, MPI_INT, destination, tag, MPI_COMM_WORLD);
    log_send(source, destination);
}

void Coordonator::receiveWorkersFromCluster(int tag, int source){
    int amount;
    MPI_Status status;
    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &amount);

    // cout << "Amount: " << amount << endl;
    // cout << "Tag: " << tag << endl;
    // cout << "Source: " << source << endl;
    
    this->sizes[tag] = amount;
    this->topology[tag] = (int*)calloc(amount, sizeof(int));
    MPI_Recv(this->topology[tag], amount, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // cout << "Proces: " << this->rank << endl; 
    // for(int i = 0; i < amount; i++){
    //     cout << this->topology[tag][i] << " ";
    // }
    // cout << endl;
}

void Coordonator::sendTopologyToWorkers(){
    for(int i = 0; i < number_of_workers; i++){
        for(int j = 0; j < 4; j++){
            MPI_Send(this->topology[j], this->sizes[j], MPI_INT, this->workers[i], this->workers[i], MPI_COMM_WORLD);
            //log_send(this->rank, this->workers[i]);
        }
    }
}

void Coordonator::equalDistribution(int vectorSize){
    int totalWorkers = 0;
    for(int i = 0; i < 4; i++){
        totalWorkers += this->sizes[i];
    }
    //cout << "workeri: " << totalWorkers << " size: " << vectorSize << endl;
    //cout << "Size:";

    this->clusterDistribution[0] = ceil(vectorSize * (float)this -> sizes[0] / (float)totalWorkers);
    this->clusterDistribution[2] = ceil(vectorSize * (float)this -> sizes[2] / (float)totalWorkers);
    this->clusterDistribution[3] = ceil(vectorSize * (float)this -> sizes[3] / (float)totalWorkers);
    this->clusterDistribution[1] = vectorSize - (this->clusterDistribution[0] + this->clusterDistribution[2] + this->clusterDistribution[3]);
}

void Coordonator::generateVector(int *vector, int size){
    for(int i = 0; i < size; i++){
        vector[i] = size - i - 1;
    }
}

void Coordonator::sendVector(int* vect, int size, int dest, int tag){
    MPI_Send(vect, size, MPI_INT, dest, tag, MPI_COMM_WORLD);
    log_send(this->rank, dest);
}

void Coordonator::recieveVector(int tag, int source){
    int amount;
    MPI_Status status;
    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &amount);

    this->vectorCluster = (int*)calloc(amount, sizeof(int));
    MPI_Recv(this->vectorCluster, amount, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

void Coordonator::recieveVectorTemp(int tag, int source){
    int amount;
    MPI_Status status;
    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &amount);

    this->temp = (int*)calloc(amount, sizeof(int));
    this->gather_amount = amount;
    MPI_Recv(this->temp, amount, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

void copy_from_start_to_end(int *vect1, int *vect2, int start, int end){
    for(int i = start, contor = 0; i < end; i++, contor++){
        vect1[contor] = vect2[i];
    }
}

void Coordonator::sendVectorToEachWorker(){
    int start;
    int end;
    int nr_elem = this->clusterDistribution[this->rank];

    int **distributionToWorkers = (int**)calloc(this->number_of_workers, sizeof(int));

    for(int i = 0; i < this->number_of_workers; i++){
        start = i * ceil((double)nr_elem / this->number_of_workers);
        end = min((i + 1) * ceil((double)nr_elem / this->number_of_workers), nr_elem);
        int size = end - start;

        distributionToWorkers[i] = (int*)calloc(size, sizeof(int));

        copy_from_start_to_end(distributionToWorkers[i], this->vectorCluster, start, end);
        MPI_Send(distributionToWorkers[i], size, MPI_INT, this->workers[i], this->rank, MPI_COMM_WORLD);
        log_send(this->rank, this->workers[i]);
    }
}

void Coordonator::receiveVectorFromEachWorker(){
    int amount;
    MPI_Status status;
    for(int i = 0; i < this->number_of_workers; i++){
        MPI_Probe(this->workers[i], this->workers[i], MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &amount);
        
        int* temp = (int*)calloc(amount, sizeof(int));
        MPI_Recv(temp, amount, MPI_INT, this->workers[i], this->workers[i], MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for(int i = 0; i < amount; i++){
            this->gather.push_back(temp[i]);
        }
    }
}

void Coordonator::sendVectorGather(int dest, int tag){
    int* tmp = (int*)calloc(this->gather.size(), sizeof(int));
    for(int i = 0; i < this->gather.size(); i++){
        tmp[i] = this->gather.at(i);
    }
    this->sendVector(tmp, this->gather.size(), dest, tag);
    log_send(this->rank, dest);
}

void Coordonator::recieveVectorGather(int tag, int source){
    int amount;
    MPI_Status status;
    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &amount);

    this->temp = (int*)calloc(amount, sizeof(int));
    this->gather_amount = amount;
    MPI_Recv(this->temp, amount, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}