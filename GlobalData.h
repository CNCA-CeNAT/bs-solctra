//
// Created by lchavarr on 4/24/16.
//

#ifndef SOLCTRA_GLOBALDATA_H
#define SOLCTRA_GLOBALDATA_H

#include "Coil.h"
#include "utils.h"

class GlobalData
{
public:
    GlobalData();
    ~GlobalData();
    Coil** coils;
    Coil** e_roof;
    double** leng_segment;
};


#endif //SOLCTRA_GLOBALDATA_H
