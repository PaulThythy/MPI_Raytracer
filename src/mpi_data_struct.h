#ifndef MPI_DATA_STRUCT_H
#define MPI_DATA_STRUCT_H

#ifdef _WIN32
	#include "../libs/MPI/Include/mpi.h"
#elif __linux__
	#include <mpi.h>
#endif

struct PixelData {
    //coordinates of the pixel in the image
    int x, y;
    //r g b values of the color of the pixel
    int r, g, b;
};
//number of fields in the structure
const int nitems_pixel_data = 5;
//only 1 value per types (no arrays)
int block_lengths[5] = {1,1,1,1,1};
//1 MPI_INT per fields
MPI_Datatype types[nitems_pixel_data] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
MPI_Datatype mpi_pixelData_type;
MPI_Aint offsets[nitems_pixel_data];
//maybe I can keep only MPI_Datatypes in public and the rest in the function ?


inline void init_mpi_data_structs() {

    /* for pixel datas */
    offsets[0] = offsetof(PixelData, x);
    offsets[1] = offsetof(PixelData, y);
    offsets[2] = offsetof(PixelData, r);
    offsets[3] = offsetof(PixelData, g);
    offsets[4] = offsetof(PixelData, b);
    MPI_Type_create_struct(nitems_pixel_data, block_lengths, offsets, types, &mpi_pixelData_type);
    MPI_Type_commit(&mpi_pixelData_type);

}

#endif