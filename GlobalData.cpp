//
// Created by lchavarr on 4/24/16.
//

#include "GlobalData.h"


GlobalData::GlobalData()
{
    e_roof = Coil::allocateCoils();
    leng_segment = allocateMatrixOfDoubles(TOTAL_OF_COILS, TOTAL_OF_GRADES);
    coils = Coil::allocateCoils();
}

GlobalData::~GlobalData()
{
    deallocate(leng_segment);
    Coil::destroyVector(e_roof);
    Coil::destroyVector(coils);
}