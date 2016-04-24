//
// Created by lchavarr on 4/24/16.
//

#ifndef SOLCTRA_COIL_H
#define SOLCTRA_COIL_H


class Coil
{
public:
    Coil();
    ~Coil();
    static Coil** allocateCoils();
    static void destroyVector(Coil** coils);


    double* x;
    double* y;
    double* z;
};


#endif //SOLCTRA_COIL_H
