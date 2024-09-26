cmake_minimum_required(VERSION 3.9)
project(MPI_Raytracer LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wfatal-errors -Werror -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(MPI REQUIRED)

file(GLOB_RECURSE RAYTRACER_SOURCES "src/*")

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/SDL EXCLUDE_FROM_ALL)

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glm/glm)
include_directories(${PROJECT_SOURCE_DIR}/libs/glm/glm)

add_executable(MPI_Raytracer ${RAYTRACER_SOURCES})

target_link_libraries(MPI_Raytracer PRIVATE SDL2::SDL2 MPI::MPI_CXX glm::glm)