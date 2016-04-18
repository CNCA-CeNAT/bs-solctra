//
// Created by lchavarr on 4/17/16.
//

#ifndef SOLCTRA_SOLCTRA_H
#define SOLCTRA_SOLCTRA_H

#define     PI      3.141592654
#define     miu     1.2566e-06
#define     I       -4350

typedef     struct{

    float x, y, z;

}cartesian;

struct  coil{

    float x[361], y[361], z[361];

};
extern struct coil num_coil[12],vec_e_roof[12],Rmi,Rmf;
extern float   leng_segment[12][360];

void    e_roof(void);
void    load_coil_data(void);
cartesian load_path(int point);
cartesian     magnetic_field(cartesian point);
float norm_of(cartesian vec);
void    R_vectors(float xx, float yy, float zz, int act_coil);
void    RK4(cartesian start_point,int steps,float step_size, int path,int mode);
int save_data(int phat_num,float x,float y, float z);

#endif //SOLCTRA_SOLCTRA_H
