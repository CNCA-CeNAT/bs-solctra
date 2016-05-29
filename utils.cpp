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
