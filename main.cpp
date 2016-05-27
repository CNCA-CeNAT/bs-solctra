
#include "solctra.h"
#include <iostream>
#include <omp.h>

const unsigned DEFAULT_STEPS = 500000;
const double DEFAULT_STEP_SIZE = 0.001;
const unsigned DEFAULT_PRECISION = 5;
const unsigned DEFAULT_PARTICLES= 10;
const unsigned DEFAULT_MODE= 1;

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

int main(int argc, char** argv)
{
    const int ompSize = omp_get_max_threads();
    int micOmpSize = 0;
#pragma offload target(mic) out(micOmpSize)
    micOmpSize = omp_get_max_threads();
    const int devices = _Offload_number_of_devices();
    unsigned steps;
    double stepSize;
    unsigned precision;
    unsigned int particles;
    unsigned int mode;
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
    std::cout << "OpenMP local size=[" << ompSize << "]." << std::endl;
    std::cout << "OpenMP mic size=[" << micOmpSize << "]." << std::endl;
    std::cout << "Number of devices=[" << devices << "]." << std::endl;
    cartesian A={0,0,0};          // A:start point of field line
    const size_t sizeToAllocate = sizeof(double) * TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS;
    GlobalData data;
    data.coils.x = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.coils.y = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.coils.z = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    load_coil_data(data.coils.x, data.coils.y, data.coils.z, PATH_TO_RESOURCES);
    /*for(unsigned i = 0 ; i < TOTAL_OF_COILS ; ++i)
    {
        std::cout << i << std::endl;
        std::cout << coils[i]->x[359] << "\t" << coils[i]->y[359] << "\t" << coils[i]->z[359] << "\t" << std::endl;
        std::cout << coils[i]->x[360] << "\t" << coils[i]->y[360] << "\t" << coils[i]->z[360] << "\t" << std::endl;
        std::cout << coils[i]->x[361] << "\t" << coils[i]->y[361] << "\t" << coils[i]->z[361] << "\t" << std::endl;
    }*/
    data.e_roof.x = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.e_roof.y = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.e_roof.z = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.leng_segment = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    e_roof(data);

    const double startTime = getCurrentTime();
    runParticles(data, particles, steps, stepSize, mode);
    const double endTime = getCurrentTime();
    _mm_free(data.coils.x);
    _mm_free(data.coils.y);
    _mm_free(data.coils.z);
    _mm_free(data.e_roof.x);
    _mm_free(data.e_roof.y);
    _mm_free(data.e_roof.z);
    _mm_free(data.leng_segment);
    std::cout << "Total execution time=[" << (endTime - startTime) << "]." << std::endl;
    return (5);
}