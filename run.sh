#!/bin/bash

OS="$(uname -s)"
NB_PROCESS=5

build_type=Debug 

#Generate the Makefile using CMake
cmake -S . -B build/$build_type -G "Ninja" -DCMAKE_BUILD_TYPE=$build_type
cmake --build build/$build_type --target MPI_Raytracer


#Check if the compilation was successful
if [ $? -eq 0 ]; then
	echo "Compilation successful. Running the program..."
	
	if [ "$OS" = "Linux" ]; then
        mpirun --oversubscribe --report-bindings -np ${NB_PROCESS} ./build/*/MPI_Raytracer
    elif [[ "$OS" == *"MINGW"* || "$OS" == *"CYGWIN"* || "$OS" == *"MSYS"* || "$OS" == *"MINGW32"* ]]; then
        mpiexec --oversubscribe --report-bindings -n ${NB_PROCESS} ./build/*/MPI_Raytracer
    else
        echo "Unsupported OS: $OS"
        exit 1
    fi
else
	echo "Compilation failed. Please check the error messages."
fi
