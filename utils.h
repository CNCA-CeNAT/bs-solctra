//
// Created by lchavarr on 4/19/16.
//

#ifndef SOLCTRA_UTILS_H
#define SOLCTRA_UTILS_H

#include <string>
#include <iostream>

#define     PI      3.141592654
#define     miu     1.2566e-06
#define     I       -4350
#define ALIGNMENT_SIZE 64
#define TOTAL_OF_GRADES 360
#define TOTAL_OF_GRADES_PADDED 384
#define TOTAL_OF_COILS 12
#define PATH_TO_RESOURCES "resources"


struct cartesian
{
    double x, y, z;
    void print()
    {
        std::cout << "X=[" << x << "]. Y=[" << y << "]. Z=[" << z << "]." << std::endl;
    }
};


struct Coil
{
    double* x;
    double* y;
    double* z;

};
struct GlobalData
{
    Coil coils;
    Coil e_roof;
    double* leng_segment;
};

#ifndef __INTEL_COMPILER

void* _mm_malloc(size_t size, size_t alignment);
void _mm_free(void* pointer);

#define nullptr NULL

#endif


void* allocateGeneric(const int size);
double** allocateMatrixOfDoubles(const int x, const int y);
double* allocateVectorOfDoubles(const int size);
void deallocateGeneric(void* pointer);
void deallocate(double** matrix);
void deallocate(double* vector);


double getCurrentTime();

#endif //SOLCTRA_UTILS_H
