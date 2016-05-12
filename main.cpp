
#include "solctra.h"
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
    if(particles > 1)
    {
        runParticles(data, particles, steps, stepSize, mode);
    }
    else if(0 == myRank)
    {
        A.x=2.284e-01;
        A.z=-0.0295;
        RK4(data, A, steps, stepSize, 5, mode);
    }
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
    std::cout << "Total execution time=[" << (endTime - startTime) << "]." << std::endl;
    return (5);
}
