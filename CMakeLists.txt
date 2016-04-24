cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_COMPILER "/opt/intel/compilers_and_libraries_2016.1.150/linux/bin/intel64/icc")
set(CMAKE_CXX_COMPILER "/opt/intel/compilers_and_libraries_2016.1.150/linux/bin/intel64/icpc")
project(solctra)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -qopenmp")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -MMD -MP -MF ${PROJECT_NAME}.d")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DPRINT_FOR_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/media/lchavarr/3635-3935/Dropbox/TEC/Tesis/code sources/solctra")
set(SOURCE_FILES main.cpp solctra.cpp solctra.h utils.h utils.cpp FileHandler.h FileHandler.cpp Coil.cpp Coil.h)
add_executable(solctra ${SOURCE_FILES})
