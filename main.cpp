
#include "solctra.h"
#include "utils.h"
#include <cstdlib>
#include <string>
#include <iostream>

const unsigned DEFAULT_STEPS = 500000;
const double DEFAULT_STEP_SIZE = 0.001;
const unsigned DEFAULT_PRECISION = 5;

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

int main(int argc, char** argv)
{
    cartesian A={0,0,0};          // A:start point of field line
    GlobalData data;
    load_coil_data(data.coils, PATH_TO_RESOURCES);
    /*for(unsigned i = 0 ; i < TOTAL_OF_COILS ; ++i)
    {
        std::cout << i << std::endl;
        std::cout << coils[i]->x[359] << "\t" << coils[i]->y[359] << "\t" << coils[i]->z[359] << "\t" << std::endl;
        std::cout << coils[i]->x[360] << "\t" << coils[i]->y[360] << "\t" << coils[i]->z[360] << "\t" << std::endl;
        std::cout << coils[i]->x[361] << "\t" << coils[i]->y[361] << "\t" << coils[i]->z[361] << "\t" << std::endl;
    }*/
    e_roof(data);
    A.x=2.284e-01;
    A.z=-0.0295;
    const unsigned steps = getStepsFromArgs(argc, argv);
    const double stepSize = getStepSizeFromArgs(argc, argv);
    const unsigned precision = getPrintPrecisionFromArgs(argc, argv);
    std::cout.precision(precision);
    std::cout << "Running with steps=[" << steps << "] and step size=[" << stepSize << "]." << std::endl;
    const double startTime = getCurrentTime();
    RK4(data, A, steps, stepSize, 5, 1);
    const double endTime = getCurrentTime();
    std::cout << "Total execution time=[" << (endTime - startTime) << "]." << std::endl;
    return (5);
}