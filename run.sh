#!/bin/bash

OS="$(uname -s)"

build_type=Debug 

#Generate the Makefile using CMake
cmake -S . -B build/$build_type -G "Ninja" -DCMAKE_BUILD_TYPE=$build_type
cmake --build build/$build_type --target MPI_Raytracer


#Check if the compilation was successful
if [ $? -eq 0 ]; then
	echo "Compilation successful. Running the program..."
	
	#Execute the ParticleSystem binary
	case "$OS" in
		Linux*)	mpirun -np 10 ./build/*/MPI_Raytracer;;
		CYGWIN*|MINGW32*|MSYS*|MINGW*)	mpiexec -np 10 ./build/*/MPI_Raytracer;;
	esac
else
	echo "Compilation failed. Please check the error messages."
fi

read