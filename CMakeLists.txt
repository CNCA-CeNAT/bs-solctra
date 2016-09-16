cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_COMPILER "/opt/intel/compilers_and_libraries/linux/bin/intel64/mpiicc")
set(CMAKE_CXX_COMPILER "/opt/intel/compilers_and_libraries/linux/bin/intel64/mpiicpc")
project(solctra)

find_package(MPI)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -qopenmp")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -MMD -MP -MF ${PROJECT_NAME}.d")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DPRINT_FOR_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/home/ldcl289/tesis/code sources/cadejos")
set(SOURCE_FILES main.cpp solctra.cpp solctra.h utils.h utils.cpp)
add_executable(solctra ${SOURCE_FILES})
