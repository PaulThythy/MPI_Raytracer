cmake_minimum_required(VERSION 3.9)
project(MPI_Raytracer LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

include_directories(${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE RAYTRACER_SOURCES "src/*")
file(GLOB_RECURSE RAYTRACER_SOURCES "src/*")

if(WIN32)
    if(DEFINED ENV{MSMPI_INC} AND DEFINED ENV{MSMPI_LIB64})
        include_directories($ENV{MSMPI_INC})
        link_directories($ENV{MSMPI_LIB64})
    else()
        message(FATAL_ERROR "MS-MPI environment variables not found. Please make sure MS-MPI is installed and configured correctly.")
    endif()

    set(SDL2_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL")
    add_subdirectory(${SDL2_SOURCE_DIR})

    add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glm/glm)
    include_directories(${PROJECT_SOURCE_DIR}/libs/glm/glm)

    add_executable(MPI_Raytracer ${RAYTRACER_SOURCES})

    target_link_libraries(MPI_Raytracer PRIVATE SDL2::SDL2 SDL2::SDL2main msmpi glm::glm)
else()
    find_package(MPI REQUIRED)

    add_subdirectory(${PROJECT_SOURCE_DIR}/libs/SDL EXCLUDE_FROM_ALL)

    add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glm/glm)
    include_directories(${PROJECT_SOURCE_DIR}/libs/glm/glm)

    add_executable(MPI_Raytracer ${RAYTRACER_SOURCES})

    target_link_libraries(MPI_Raytracer PRIVATE SDL2::SDL2 MPI::MPI_CXX glm::glm)
endif()