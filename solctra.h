//
// Created by lchavarr on 4/17/16.
//

#ifndef SOLCTRA_SOLCTRA_H
#define SOLCTRA_SOLCTRA_H

#include <string>
#include <cmath>
#include "utils.h"
#include "GlobalData.h"

void load_coil_data(Coil**& coils, const std::string& path);
void e_roof(GlobalData& data);
void R_vectors(Coil* coil, const cartesian& point, Coil& Rmi, Coil& Rmf);
cartesian magnetic_field(const GlobalData& data, const cartesian& point);
inline double norm_of(const cartesian& vec)
{
    return sqrt(( vec.x * vec.x ) + ( vec.y * vec.y ) + ( vec.z * vec.z ));
}
void RK4(const GlobalData& data, const cartesian& start_point, const unsigned int steps, const double& step_size, const int path, const int mode);

#endif //SOLCTRA_SOLCTRA_H
