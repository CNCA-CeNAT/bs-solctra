//
// Created by lchavarr on 4/17/16.
//

#ifndef SOLCTRA_SOLCTRA_H
#define SOLCTRA_SOLCTRA_H

#include <string>
#include "utils.h"
#include "Coil.h"




typedef struct
{
    double x, y, z;
} cartesian;

extern struct Coil vec_e_roof[12];
extern double leng_segment[12][360];

void e_roof(Coil** coils);
Coil** load_coil_data(const std::string& path);
cartesian magnetic_field(Coil** coils, const cartesian& point);
double norm_of(const cartesian& vec);
void R_vectors(Coil** coils, const cartesian& point, const int act_coil, struct Coil& Rmi, struct Coil& Rmf);
void RK4(Coil** coils, const cartesian& start_point, const unsigned int steps, const double& step_size, const int path, const int mode);

#endif //SOLCTRA_SOLCTRA_H
