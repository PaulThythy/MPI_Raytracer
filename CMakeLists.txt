cmake_minimum_required(VERSION 3.12)

project(MPI_Raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wfatal-errors -Werror -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

file(GLOB_RECURSE RAYTRACER_SOURCES "src/*.cpp")

add_executable(MPI_Raytracer ${RAYTRACER_SOURCES})

target_link_libraries(MPI_Raytracer ${MPI_CXX_LIBRARIES})