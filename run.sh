#!/bin/bash

build_type=Debug 

#Generate the Makefile using CMake
cmake -S . -B build/$build_type -G "Ninja" -DCMAKE_BUILD_TYPE=$build_type
cmake --build build/$build_type --target MPI_Raytracer


#Check if the compilation was successful
if [ $? -eq 0 ]; then
	echo "Compilation successful. Running the program..."
	#Execute the ParticleSystem binary
	mpirun -np 10 ./build/*/MPI_Raytracer
else
	echo "Compilation failed. Please check the error messages."
fi

read