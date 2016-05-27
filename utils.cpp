//
// Created by lchavarr on 4/19/16.
//


#include "utils.h"
#include <sys/time.h>

#ifndef __INTEL_COMPILER

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

void freeGlobalDataInMic(const GlobalData& data)
{
//#ifdef __MIC__
    const int devices = _Offload_number_of_devices();
    for(int i = 0 ; i < devices ; ++i)
    {
#pragma offload_transfer target(mic:i) in(data.coils.x: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) FREE) \
                                     in(data.coils.y: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) FREE) \
                                     in(data.coils.z: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) FREE) \
                                     in(data.e_roof.x: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) FREE) \
                                     in(data.e_roof.y: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) FREE) \
                                     in(data.e_roof.z: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) FREE) \
                                     in(data.leng_segment: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) FREE)
    }
//#endif
}

void allocGlobaDataInMic(const GlobalData& data)
{
//#ifdef __MIC_
    const int devices = _Offload_number_of_devices();
    int flags[devices];
    flags[0:devices] = 0;
    for(int i = 0 ; i < devices ; ++i)
    {
        printf("Moving data for device=[%d]\n", i);
//#pragma offload_transfer target(mic:i) signal(&flags[i]) \

#pragma offload_transfer target(mic:i) \
                                     in(data.coils.x: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) ALLOC) \
                                     in(data.coils.y: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) ALLOC) \
                                     in(data.coils.z: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) ALLOC) \
                                     in(data.e_roof.x: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) ALLOC) \
                                     in(data.e_roof.y: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) ALLOC) \
                                     in(data.e_roof.z: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) ALLOC) \
                                     in(data.leng_segment: length(TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS) ALLOC)
        printf("Moved data for device=[%d]\n", i);
    }
    for(int i = 0 ; i < devices ; ++i)
    {
//#pragma offload_wait target(mic:i) wait(&flags[i])
    }
//#endif
}

