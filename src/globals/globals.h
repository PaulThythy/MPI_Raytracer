#ifndef GLOBALS_H
#define GLOBALS_H

#include <mpi.h>

namespace Config {

    inline const double PI = 3.1415926535897932384626433832795028841971693993751058209;

    //TODO maybe make it a uint32 ?
    inline const int WINDOW_WIDTH = 1200;
    inline const int WINDOW_HEIGHT = 900;

    // number of rays per pixel
    inline const int SAMPLES = 3;

    inline const int BOUNCES = 2;

    //TODO use chunks for rendering
    inline const int BATCH_SIZE = 1;

    inline int NB_WORKERS;

    /*namespace MPI_Data {

        struct PixelData {
            int x, y;
            int r, g, b;
        };

        //number of fields in the structure
        inline const int nitems_pixel_data = 5;

        //only 1 value per types (no arrays)
        inline int block_lengths[5] = {1,1,1,1,1};

        //1 MPI_INT per fields
        inline MPI_Datatype types[nitems_pixel_data] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
        inline MPI_Datatype mpi_pixelData_type;
        inline MPI_Aint offsets[nitems_pixel_data];
        //maybe I can keep only MPI_Datatypes in public and the rest in the function ?

        inline void INIT_MPI_DATA_STRUCTS() {
            //for pixel datas
            offsets[0] = offsetof(PixelData, x);
            offsets[1] = offsetof(PixelData, y);
            offsets[2] = offsetof(PixelData, r);
            offsets[3] = offsetof(PixelData, g);
            offsets[4] = offsetof(PixelData, b);
            MPI_Type_create_struct(nitems_pixel_data, block_lengths, offsets, types, &mpi_pixelData_type);
            MPI_Type_commit(&mpi_pixelData_type);
        }

    }*/

}

#endif