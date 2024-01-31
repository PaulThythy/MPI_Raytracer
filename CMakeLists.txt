cmake_minimum_required(VERSION 3.9)
project(MPI_Raytracer LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wfatal-errors -Werror -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

file(GLOB_RECURSE RAYTRACER_SOURCES "src/*.cpp")

add_executable(MPI_Raytracer ${RAYTRACER_SOURCES})

find_package(MPI REQUIRED)

set(SDL2_DIR ${CMAKE_SOURCE_DIR}/libs/SDL2/cmake)

if(WIN32)
    #set(MPI_CXX_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/libs/MPI/Include")
    #set(MPI_CXX_LIBRARIES "${PROJECT_SOURCE_DIR}/libs/MPI/Lib")

    #include_directories(MPI_CXX_INCLUDE_PATH)
    #link_directories(MPI_CXX_LIBRARIES)

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SDL2_LIB_DIR ${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64)
    else()
        set(SDL2_LIB_DIR ${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x86)
    endif()

    # Add the SDL2.dll file to the binary directory for Windows
    file(COPY ${SDL2_LIB_DIR}/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR})

    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})

    target_link_libraries(MPI_Raytracer ${MPI_LIBRARIES} ${SDL2_LIBRARIES})
else()
    include_directories(${MPI_INCLUDE_PATH})

    set(SDL2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/SDL2/include)

    include_directories(SDL2_INCLUDE_DIR)
    link_directories(SDL2_LIB_DIR)

    target_link_libraries(MPI_Raytracer ${MPI_CXX_LIBRARIES} SDL2main SDL2)
endif()
