//
// Created by lchavarr on 4/19/16.
//


#include "utils.h"
#include <cstdlib>
#include <sys/time.h>
#include <sys/stat.h>
#include <fstream>

#ifndef __INTEL_COMPILER
#include <cstdlib>

void* _mm_malloc(size_t size, size_t /*alignment*/)
{
    return malloc(size);
}
void _mm_free(void* pointer)
{
    free(pointer);
}
#endif

void loadFile(double* x, double* y, double* z, const int length, const std::string& path)
{
    std::ifstream handler(path);
    //FILE* file_buff;
    //Open file
    //file_buff = fopen(path.c_str(), "r");
    //if (file_buff == nullptr)
    if(!handler.is_open())
    {
        printf("Error al abrir archivo=[%s] \n", path.c_str());
    }
    else
    {
        double localX, localY, localZ;
        printf("Loading %s with length=%d\n", path.c_str(), length);

        int point = 0;
        std::string line;
        while ( std::getline(handler, line) && point < length)
        {
            //printf("Line[%d]=[%s]\n", point, line.c_str());
            sscanf(line.c_str(), "%le %le %le", &localX, &localY, &localZ);
            x[point] = localX;
            y[point] = localY;
            z[point] = localZ;
            ++point;
        }
        handler.close();
    }

}

double getCurrentTime()
{
    struct timeval tod;
    gettimeofday(&tod, nullptr);
    return static_cast<double>(tod.tv_sec) + static_cast<double>(tod.tv_usec) * 1.0e-6;
}

void createDirectoryIfNotExists(const std::string& path)
{
    if(!directoryExists(path))
    {
        const int error = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (0 > error)
        {
            printf("Error creating directory!n");
            exit(1);
        }
    }
}

bool directoryExists(const std::string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    else if (info.st_mode & S_IFDIR)
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::string getZeroPadded(const int num)
{
    std::string value = std::to_string(num);
    const size_t numSize = value.size();
    for(size_t i = 0 ; i < 3 - numSize ; ++i)
    {
        value = "0" + value;
    }
    return value;
}
