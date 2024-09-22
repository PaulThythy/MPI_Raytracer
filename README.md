# MPI_Raytracer

## On linux, type the following command to install dependencies :

```sh
sudo apt install openmpi-bin openmpi-common libopenmpi-dev ninja-build cmake
```

## On windows, install Microsoft MPI from their website

## compile a simple MPI program :
```sh
mpicxx src/main.cpp -o bin/main
```

## run the program :
```sh
mpirun -np 4 ./main
```

## TODO :
find out more informations on flags compilation with GCC, for SIMD instructions 
