//
// Created by lchavarr on 4/19/16.
//

#ifndef SOLCTRA_UTILS_H
#define SOLCTRA_UTILS_H


#define     PI      3.141592654
#define     miu     1.2566e-06
#define     I       -4350
#define ALIGNMENT_SIZE 64
#define GRADES_PER_PAGE ALIGNMENT_SIZE / sizeof(double)
#define TOTAL_OF_GRADES 360
#define TOTAL_OF_GRADES_PADDED 384
#define TOTAL_OF_COILS 12
#define PATH_TO_RESOURCES "resources"


#include <cstdio>
struct cartesian
{
    double x, y, z;
    void print()
    {
        printf("X=[%e]. Y=[%e]. Z=[%e].\n", x, y, z);
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

void loadFile(double* x, double* y, double* z, const int length, const char* path);
double getCurrentTime();
void createDirectoryIfNotExists(const char* path);
bool directoryExists(const char* path);

#endif //SOLCTRA_UTILS_H
