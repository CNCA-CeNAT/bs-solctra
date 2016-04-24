//
// Created by lchavarr on 4/24/16.
//

#include "Coil.h"
#include "utils.h"

Coil::Coil()
{
    x = allocateVectorOfDoubles(TOTAL_OF_GRADES + 1);
    y = allocateVectorOfDoubles(TOTAL_OF_GRADES + 1);
    z = allocateVectorOfDoubles(TOTAL_OF_GRADES + 1);
}

Coil::~Coil()
{
    deallocate(x);
    deallocate(y);
    deallocate(z);
}

Coil** Coil::allocateCoils()
{
    Coil** coils = (Coil**)allocateGeneric(sizeof(Coil*) * TOTAL_OF_COILS);
    for(unsigned i = 0 ; i < TOTAL_OF_COILS ; ++i)
    {
        coils[i] = new Coil();
    }
    return coils;
}

void Coil::destroyVector(Coil** coils)
{
    for(unsigned i = 0 ; i < TOTAL_OF_COILS ; ++i)
    {
        delete coils[i];
    }
    deallocateGeneric(coils);
}









