//
// Created by lchavarr on 4/19/16.
//


#include "utils.h"
#include <sys/time.h>



double getCurrentTime()
{
    struct timeval tod;
    gettimeofday(&tod, nullptr);
    return static_cast<double>(tod.tv_sec) + static_cast<double>(tod.tv_usec) * 1.0e-6;
}

