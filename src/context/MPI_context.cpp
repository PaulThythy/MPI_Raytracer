#include "MPI_context.h"

MPI::MPI_context::MPI_context() {}

MPI::MPI_context::MPI_context(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    MPI_Get_processor_name(hostname, &len);
}

void MPI::MPI_context::endMPI() {
    MPI_Finalize();
}

int MPI::MPI_context::getRank() { return rank; }

int MPI::MPI_context::getNumTasks() { return num_tasks; }

MPI_Request MPI::MPI_context::getRequest() { return request; }

MPI_Status MPI::MPI_context::getStatus() { return status; }