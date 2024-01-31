cmake_minimum_required(VERSION 3.9)
project(MPI_Raytracer LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wfatal-errors -Werror -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

file(GLOB_RECURSE RAYTRACER_SOURCES "src/*.cpp")

add_executable(MPI_Raytracer ${RAYTRACER_SOURCES})

find_package(MPI REQUIRED)

if(WIN32)
    #set(MPI_CXX_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/libs/MPI/Include")
    #set(MPI_CXX_LIBRARIES "${PROJECT_SOURCE_DIR}/libs/MPI/Lib")

    #include_directories(MPI_CXX_INCLUDE_PATH)
    #link_directories(MPI_CXX_LIBRARIES)

    target_link_libraries(MPI_Raytracer ${MPI_LIBRARIES})
else()
    include_directories(${MPI_INCLUDE_PATH})

    target_link_libraries(MPI_Raytracer ${MPI_CXX_LIBRARIES})
endif()
