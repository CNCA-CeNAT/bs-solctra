
#include "solctra.h"
#include <iostream>
#include <mpi.h>

const unsigned DEFAULT_STEPS = 500000;
const double DEFAULT_STEP_SIZE = 0.001;
const unsigned DEFAULT_PRECISION = 5;
const unsigned DEFAULT_PARTICLES= 1;
const unsigned DEFAULT_MODE= 1;

unsigned getPrintPrecisionFromArgs(const int& argc, char** argv)
{
    for(unsigned i = 1 ; i < argc - 1 ; ++i)
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
    for(unsigned i = 1 ; i < argc - 1 ; ++i)
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
    for(unsigned i = 1 ; i < argc - 1 ; ++i)
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
    for(unsigned i = 1 ; i < argc - 1 ; ++i)
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
    for(unsigned i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-mode")
        {
            return static_cast<unsigned>(atoi(argv[i+1]));
        }
    }
    return DEFAULT_MODE;
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    int myRank;
    int commSize;
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    unsigned steps;
    double stepSize;
    unsigned precision;
    unsigned int particles;
    unsigned int mode;
    if(0 == myRank)
    {
        std::cout << "Communicator Size=[" << commSize << "]." << std::endl;
        steps = getStepsFromArgs(argc, argv);
        stepSize = getStepSizeFromArgs(argc, argv);
        precision = getPrintPrecisionFromArgs(argc, argv);
        particles = getParticlesFromArgs(argc, argv);
        mode = getModeFromArgs(argc, argv);
        std::cout.precision(precision);
        std::cout << "Running with:" << std::endl;
        std::cout << "Steps=[" << steps << "]." << std::endl;
        std::cout << "Steps size=[" << stepSize << "]." << std::endl;
        std::cout << "Particles=[" << particles << "]." << std::endl;
        std::cout << "Mode=[" << mode << "]." << std::endl;
    }
    MPI_Bcast(&steps, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&stepSize, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&precision, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&particles, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&mode, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);


    cartesian A={0,0,0};          // A:start point of field line
    //double x[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    double y[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    double z[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    //const size_t sizeToAllocate = sizeof(double) * TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS;
    //double* x = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    //double* y = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    //double* z = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    if(0 == myRank)
    {
        load_coil_data(x, y, z, PATH_TO_RESOURCES);
        //std::cout << x[TOTAL_OF_GRADES_PADDED] << "|" << y[TOTAL_OF_GRADES_PADDED] << "|" << z[TOTAL_OF_GRADES_PADDED] << std::endl;
        //std::cout << x[TOTAL_OF_GRADES_PADDED*2] << "|" << y[TOTAL_OF_GRADES_PADDED*2] << "|" << z[TOTAL_OF_GRADES_PADDED*2] << std::endl;
    }
    MPI_Bcast(&x, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&y, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&z, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    /*for(unsigned i = 0 ; i < TOTAL_OF_COILS ; ++i)
    {
        std::cout << i << std::endl;
        std::cout << coils[i]->x[359] << "\t" << coils[i]->y[359] << "\t" << coils[i]->z[359] << "\t" << std::endl;
        std::cout << coils[i]->x[360] << "\t" << coils[i]->y[360] << "\t" << coils[i]->z[360] << "\t" << std::endl;
        std::cout << coils[i]->x[361] << "\t" << coils[i]->y[361] << "\t" << coils[i]->z[361] << "\t" << std::endl;
    }*/
    GlobalData data;
    data.coils.x = x;
    data.coils.y = y;
    data.coils.z = z;
    double eRoofX[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    double eRoofY[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    double eRoofZ[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    //double* eRoofX = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    //double* eRoofY = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    //double* eRoofZ = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    double leng_segment[TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS] __attribute__((aligned(64)));
    data.e_roof.x = eRoofX;
    data.e_roof.y = eRoofY;
    data.e_roof.z = eRoofZ;
    data.leng_segment = leng_segment;
    if(0 == myRank)
    {
        e_roof(data);
    }
    MPI_Bcast(&eRoofX, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&eRoofY, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&eRoofZ, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&leng_segment, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    A.x=2.284e-01;
    A.z=-0.0295;

    const double startTime = getCurrentTime();
    if(particles > 1)
    {
        runParticles(data, particles, steps, stepSize, mode);
    }
    else if(0 == myRank)
    {
        RK4(data, A, steps, stepSize, 5, mode);
    }
    std::cout << myRank << " before finalize barrier!" << std::endl;
    MPI_Barrier(MPI_COMM_WORLD);
    const double endTime = getCurrentTime();
    delete x;
    delete y;
    delete z;
    MPI_Finalize();
    std::cout << "Total execution time=[" << (endTime - startTime) << "]." << std::endl;
    return (5);
}
