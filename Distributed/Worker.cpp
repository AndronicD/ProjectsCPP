#include "mpi.h"
#include "Worker.h"
#include "helpers.cpp"

Worker::Worker(){};
Worker::~Worker(){};

void Worker::recieveCoordonator(){
    //int MPI_Recv(void* data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)
    MPI_Recv(&this->coordonator, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

void Worker::recieveTopology(){
    int amount;
    MPI_Status status;
    for(int j = 0; j < 4; j++){
        MPI_Probe(this->coordonator, this->rank, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &amount);
        this->sizes[j] = amount;
        this->topology[j] = (int*)calloc(amount, sizeof(int));
        
        MPI_Recv(this->topology[j], amount, MPI_INT, this->coordonator, this->rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

void Worker::recieveVector(){
    int amount;
    MPI_Status status;
    MPI_Probe(this->coordonator, this->coordonator, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &amount);

    this->vectorPrimit = (int*)calloc(amount, sizeof(int));
    this->size_received = amount;
    MPI_Recv(this->vectorPrimit, amount, MPI_INT, this->coordonator, this->coordonator, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

void Worker::changeVector(){
    this->vectorTrimis = (int*)calloc(this->size_received, sizeof(int));
    for(int i = 0; i < this->size_received; i++){
        this->vectorTrimis[i] = 5 * this->vectorPrimit[i];
    }
}

void Worker::sendVector(){
    MPI_Send(this->vectorTrimis, this->size_received, MPI_INT, this->coordonator, this->rank, MPI_COMM_WORLD);
    log_send(this->rank, this->coordonator);
}