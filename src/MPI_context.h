#ifndef MPI_CONTEXT_H
#define MPI_CONTEXT_H

#ifdef _WIN32
	#include "../libs/MPI/Include/mpi.h"
#elif __linux__
	#include <mpi.h>
#endif

class MPI_context {

private:
    int len, num_tasks, rank;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Request request;
    MPI_Status status;

public:
    MPI_context(int argc, char *argv[]);
    MPI_context();

    void initMPI(int argc, char *argv[]);
    void endMPI();

    int getRank();
    int getNumTasks();

    MPI_Request getRequest();
    MPI_Status getStatus();
};

#endif