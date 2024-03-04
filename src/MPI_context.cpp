#include "MPI_context.h"

MPI_context::MPI_context(int argc, char *argv[]) {
    initMPI(argc, argv);
}

void MPI_context::initMPI(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    MPI_Get_processor_name(hostname, &len);
}

void MPI_context::endMPI() {
    MPI_Finalize();
}

int MPI_context::getRank() { return rank; }

int MPI_context::getNumTasks() { return num_tasks; }

MPI_Request MPI_context::getRequest() { return request; }

MPI_Status MPI_context::getStatus() { return status; }