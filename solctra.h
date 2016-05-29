//
// Created by lchavarr on 4/17/16.
//

#ifndef SOLCTRA_SOLCTRA_H
#define SOLCTRA_SOLCTRA_H

#include <string>
#include <cmath>
#include "utils.h"

void load_coil_data(double* x, double* y, double* z, const std::string& path);
void e_roof(GlobalData& data);
void R_vectors(double* x, double* y, double* z, const cartesian& point, Coil& Rmi, Coil& Rmf);
cartesian magnetic_field(const GlobalData& data, const cartesian& point);
void RK4(const GlobalData& data, const cartesian& start_point, const int steps, const double& step_size, const int particle, const int mode);
inline double norm_of(const cartesian& vec)
{
    return sqrt(( vec.x * vec.x ) + ( vec.y * vec.y ) + ( vec.z * vec.z ));
}
void runParticles(const GlobalData& data, const int particles, const int steps, const double& step_size, const int mode);

#endif //SOLCTRA_SOLCTRA_H
