//
// Created by lchavarr on 4/19/16.
//


#include "utils.h"
#include <sys/time.h>

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


double getCurrentTime()
{
    struct timeval tod;
    gettimeofday(&tod, nullptr);
    return static_cast<double>(tod.tv_sec) + static_cast<double>(tod.tv_usec) * 1.0e-6;
}


void* allocateGeneric(const int size)
{
    return _mm_malloc(size, ALIGNMENT_SIZE);
}

double** allocateMatrixOfDoubles(const int x, const int y)
{
    double** matrix = static_cast<double**>(_mm_malloc(sizeof(double*) * x, ALIGNMENT_SIZE));
    for(int i = 0 ; i < x; ++i)
    {
        matrix[i] = static_cast<double*>(_mm_malloc(sizeof(double) * y, ALIGNMENT_SIZE));
    }
    return matrix;
}

double* allocateVectorOfDoubles(const int size)
{
    return static_cast<double*>(_mm_malloc(sizeof(double) * size, ALIGNMENT_SIZE));
}

void deallocateGeneric(void* pointer)
{
    _mm_free(pointer);
}

void deallocate(double** matrix)
{
    _mm_free(matrix);
}

void deallocate(double* vector)
{
    _mm_free(vector);
}













