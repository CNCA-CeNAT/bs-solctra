
#include "solctra.h"
#include <mpi.h>
#include <omp.h>
#include <fstream>
#include <iostream>
#include <cstring>

const unsigned DEFAULT_STRING_BUFFER = 30;
const unsigned DEFAULT_STEPS = 500000;
const double DEFAULT_STEP_SIZE = 0.001;
const unsigned DEFAULT_PRECISION = 5;
const unsigned DEFAULT_PARTICLES= 1;
const unsigned DEFAULT_MODE= 1;
const char* DEFAULT_OUTPUT = "results";

unsigned getPrintPrecisionFromArgs(const int& argc, char** argv)
{
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-precision")
        {
            return static_cast<unsigned>(atoi(argv[i+1]));
        }
    }
    return DEFAULT_PRECISION;
}
unsigned getStepsFromArgs(const int& argc, char** argv)
{
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-steps")
        {
            return static_cast<unsigned>(atoi(argv[i+1]));
        }
    }
    return DEFAULT_STEPS;
}
double getStepSizeFromArgs(const int& argc, char** argv)
{
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-stepSize")
        {
            return strtod(argv[i+1], nullptr);
        }
    }
    return DEFAULT_STEP_SIZE;
}
unsigned getParticlesFromArgs(const int& argc, char** argv)
{
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-particles")
        {
            return static_cast<unsigned>(atoi(argv[i+1]));
        }
    }
    return DEFAULT_PARTICLES;
}
unsigned getModeFromArgs(const int& argc, char** argv)
{
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-mode")
        {
            return static_cast<unsigned>(atoi(argv[i+1]));
        }
    }
    return DEFAULT_MODE;
}

void getResultsPath(const int& argc, char** argv, char* &path)
{
    bool found = false;
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-out")
        {
            found = true;
            sprintf(path,"results_%s", argv[i+1]);
        }
    }
    if(!found)
    {
        strcpy(path, "results");
    }
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    int myRank;
    int commSize;
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    const int ompSize = omp_get_max_threads();
    unsigned steps;
    double stepSize;
    unsigned precision;
    unsigned int particles;
    unsigned int mode;
    char* output = new char[DEFAULT_STRING_BUFFER];
    std::ofstream handler;
    if(0 == myRank)
    {
        std::cout << "Communicator Size=[" << commSize << "]." << std::endl;
        steps = getStepsFromArgs(argc, argv);
        stepSize = getStepSizeFromArgs(argc, argv);
        precision = getPrintPrecisionFromArgs(argc, argv);
        particles = getParticlesFromArgs(argc, argv);
        mode = getModeFromArgs(argc, argv);
        getResultsPath(argc, argv, output);
        createDirectoryIfNotExists(output);
        std::cout.precision(precision);
        std::cout << "Running with:" << std::endl;
        std::cout << "Steps=[" << steps << "]." << std::endl;
        std::cout << "Steps size=[" << stepSize << "]." << std::endl;
        std::cout << "Particles=[" << particles << "]." << std::endl;
        std::cout << "Mode=[" << mode << "]." << std::endl;
        std::cout << "Output path=[" << output << "]." << std::endl;
        std::cout << "MPI size=[" << commSize << "]." << std::endl;
        std::cout << "OpenMP size=[" << ompSize << "]." << std::endl;
        char file_name[30];
        sprintf(file_name,"%s/stdout.log", output);
        handler.open(file_name);
        if(!handler.is_open())
        {
            std::cerr << "Unable to open stdout.log for appending. Nothing to do." << std::endl;
            exit(0);
        }

        handler << "Running with:" << std::endl;
        handler << "Steps=[" << steps << "]." << std::endl;
        handler << "Steps size=[" << stepSize << "]." << std::endl;
        handler << "Particles=[" << particles << "]." << std::endl;
        handler << "Mode=[" << mode << "]." << std::endl;
        handler << "Output path=[" << output << "]." << std::endl;
        handler << "MPI size=[" << commSize << "]." << std::endl;
        handler << "OpenMP size=[" << ompSize << "]." << std::endl;
    }
    MPI_Bcast(&steps, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&stepSize, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&precision, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&particles, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&mode, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(output, DEFAULT_STRING_BUFFER, MPI_CHAR, 0, MPI_COMM_WORLD);
    std::cout << "Rank=[" << myRank << "] => Running with:" << std::endl;
    std::cout << "Rank=[" << myRank << "] => Steps=[" << steps << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => Steps size=[" << stepSize << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => Particles=[" << particles << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => Mode=[" << mode << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => Output path=[" << output << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => MPI size=[" << commSize << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => OpenMP size=[" << ompSize << "]." << std::endl;


    //double x[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    //double y[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    //double z[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    const size_t sizeToAllocate = sizeof(double) * TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS;
    GlobalData data;
    data.coils.x = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.coils.y = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.coils.z = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    if(0 == myRank)
    {
        load_coil_data(data.coils.x, data.coils.y, data.coils.z, PATH_TO_RESOURCES);
        //std::cout << x[TOTAL_OF_GRADES_PADDED] << "|" << y[TOTAL_OF_GRADES_PADDED] << "|" << z[TOTAL_OF_GRADES_PADDED] << std::endl;
        //std::cout << x[TOTAL_OF_GRADES_PADDED*2] << "|" << y[TOTAL_OF_GRADES_PADDED*2] << "|" << z[TOTAL_OF_GRADES_PADDED*2] << std::endl;
    }
    MPI_Bcast(data.coils.x, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(data.coils.y, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(data.coils.z, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    //double eRoofX[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    //double eRoofY[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    //double eRoofZ[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    //double leng_segment[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    //data.e_roof.x = eRoofX;
    //data.e_roof.y = eRoofY;
    //data.e_roof.z = eRoofZ;
    //data.leng_segment = leng_segment;
    data.e_roof.x = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.e_roof.y = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.e_roof.z = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.leng_segment = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    if(0 == myRank)
    {
        e_roof(data);
    }
    MPI_Bcast(data.e_roof.x, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(data.e_roof.y, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(data.e_roof.z, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(data.leng_segment, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    const double startTime = getCurrentTime();
    runParticles(data, output, particles, steps, stepSize, mode);
    std::cout << "Rank=" << myRank << " before finalize barrier!" << std::endl;
    MPI_Barrier(MPI_COMM_WORLD);
    const double endTime = getCurrentTime();
    _mm_free(data.coils.x);
    _mm_free(data.coils.y);
    _mm_free(data.coils.z);
    _mm_free(data.e_roof.x);
    _mm_free(data.e_roof.y);
    _mm_free(data.e_roof.z);
    _mm_free(data.leng_segment);
    MPI_Finalize();

    if(0 == myRank)
    {
        handler << "Total execution time=[" << (endTime - startTime) << "]." << std::endl;
	    handler.close();
    }
    std::cout << "Total execution time=[" << (endTime - startTime) << "]." << std::endl;
    return (5);
}
