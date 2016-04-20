//
// Created by lchavarr on 4/17/16.
//

#ifndef SOLCTRA_SOLCTRA_H
#define SOLCTRA_SOLCTRA_H

#define     PI      3.141592654f
#define     miu     1.2566e-06f
#define     I       -4350.0f

typedef struct
{
    float x, y, z;
} cartesian;

struct coil
{
    float x[361], y[361], z[361];
};
extern struct coil num_coil[12], vec_e_roof[12], Rmi, Rmf;
extern float leng_segment[12][360];

void e_roof();
void load_coil_data();
cartesian magnetic_field(const cartesian& point);
float norm_of(const cartesian& vec);
void R_vectors(const float& xx, const float& yy, const float& zz, const int act_coil);
void RK4(const cartesian& start_point, const unsigned int steps, const float& step_size, const int path, const int mode);
int save_data(const int phat_num, const float& x, const float& y, const float& z);

#endif //SOLCTRA_SOLCTRA_H
