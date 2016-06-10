//
// Created by lchavarr on 4/19/16.
//


#include "utils.h"
#include <cstdlib>
#include <sys/time.h>
#include <sys/stat.h>

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

void loadFile(double* x, double* y, double* z, const int length, const char* path)
{
    FILE* file_buff;
    //Open file
    file_buff = fopen(path, "r");
    if (file_buff == nullptr)
    {
        printf("Error al abrir archivo \n");
    }
    else
    {
        double localX, localY, localZ;
        printf("Loading %s with length=%d\n", path, length);
        for (int point = 0; point < length; point++)
        {
            fscanf(file_buff, "%le %le %le", &localX, &localY, &localZ);
            x[point] = localX;
            y[point] = localY;
            z[point] = localZ;
        }
        fclose(file_buff);
    }

}

double getCurrentTime()
{
    struct timeval tod;
    gettimeofday(&tod, nullptr);
    return static_cast<double>(tod.tv_sec) + static_cast<double>(tod.tv_usec) * 1.0e-6;
}

void createDirectoryIfNotExists(const char* path)
{
    if(!directoryExists(path))
    {
        const int error = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (0 > error)
        {
            printf("Error creating directory!n");
            exit(1);
        }
    }
}

bool directoryExists(const char* path)
{
    struct stat info;
    if (stat(path, &info) != 0)
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