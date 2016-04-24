//
// Created by lchavarr on 4/19/16.
//

#ifndef SOLCTRA_UTILS_H
#define SOLCTRA_UTILS_H

#include <string>

#define     PI      3.141592654
#define     miu     1.2566e-06
#define     I       -4350
const unsigned ALIGNMENT_SIZE = 64;
const unsigned TOTAL_OF_GRADES = 360;
const unsigned TOTAL_OF_COILS = 12;
const std::string PATH_TO_RESOURCES = "resources";


void* allocateGeneric(const unsigned size);
double** allocateMatrixOfDoubles(const unsigned x, const unsigned y);
double* allocateVectorOfDoubles(const unsigned size);
void deallocateGeneric(void* pointer);
void deallocate(double** matrix);
void deallocate(double* vector);


double getCurrentTime();

#endif //SOLCTRA_UTILS_H
