//
// Created by lchavarr on 4/17/16.
//


#include "solctra.h"
#include "FileHandler.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <iostream>

struct Coil vec_e_roof[12];
double leng_segment[12][360];

void e_roof(Coil** coils)
{
    cartesian segment;
    for (int j = 0; j < TOTAL_OF_COILS; j++)
    {
        for (unsigned i = 0; i < TOTAL_OF_GRADES; i++)
        {
            segment.x = ( coils[j]->x[i + 1] ) - ( coils[j]->x[i] );
            segment.y = ( coils[j]->y[i + 1] ) - ( coils[j]->y[i] );
            segment.z = ( coils[j]->z[i + 1] ) - ( coils[j]->z[i] );
            leng_segment[j][i] = norm_of(segment);
            vec_e_roof[j].x[i] = segment.x / leng_segment[j][i];
            vec_e_roof[j].y[i] = segment.y / leng_segment[j][i];
            vec_e_roof[j].z[i] = segment.z / leng_segment[j][i];
        }
    }
}

Coil** load_coil_data(const std::string& path)
{
    char coil_file[30];
    int num, point;
    double x,y,z;
    Coil** coils = Coil::allocateCoils();
    for (num = 0; num < TOTAL_OF_COILS; num++)
    {
        std::string tmp = path + "/Bobina%dm.txt";
        //Set Coil files location
        sprintf(coil_file, tmp.c_str(), num);
        //create buffer
        FILE* file_buff;
        //Open file
        file_buff = fopen(coil_file, "r");
        if (file_buff == NULL)
        {
            printf("Error al abrir archivo \n");
        }
        else
        {
            for (point = 0; point < TOTAL_OF_GRADES + 1; point++)
            {
                fscanf(file_buff, "%le %le %le", &x, &y, &z);
                coils[num]->x[point] = x;
                coils[num]->y[point] = y;
                coils[num]->z[point] = z;
            }
            fclose(file_buff);
        }
    }
    return coils;
}

cartesian magnetic_field(Coil** coils, const cartesian& point)
{
    cartesian B = {0, 0, 0}, V = {0, 0, 0}, U = {0, 0, 0};
    double norm_Rmi;
    double norm_Rmf;
    double C;
    for (int i = 0; i < TOTAL_OF_COILS; i++)
    {
        Coil Rmi, Rmf;
        R_vectors(coils, point, i, Rmi, Rmf);
        for (int j = 0; j < TOTAL_OF_GRADES; j++)
        {
            norm_Rmi = sqrt((( Rmi.x[j] * Rmi.x[j]) + ( Rmi.y[j] * Rmi.y[j]) + ( Rmi.z[j] * Rmi.z[j])));
            norm_Rmf = sqrt((( Rmf.x[j] * Rmf.x[j]) + ( Rmf.y[j] * Rmf.y[j]) + ( Rmf.z[j] * Rmf.z[j])));

            //firts vector of cross product in equation 8
            U.x = (( miu * I ) / ( 4 * PI )) * vec_e_roof[i].x[j];
            U.y = (( miu * I ) / ( 4 * PI )) * vec_e_roof[i].y[j];
            U.z = (( miu * I ) / ( 4 * PI )) * vec_e_roof[i].z[j];

            //second vector of cross product in equation 8
            C = ((( 2 * ( leng_segment[i][j] ) * ( norm_Rmi + norm_Rmf )) / ( norm_Rmi * norm_Rmf )) *
                 (( 1 ) / ( ( norm_Rmi + norm_Rmf ) * ( norm_Rmi + norm_Rmf ) - leng_segment[i][j] * leng_segment[i][j])));

            V.x = Rmi.x[j] * C;
            V.y = Rmi.y[j] * C;
            V.z = Rmi.z[j] * C;

            //cross product in equation 8
            B.x = B.x + (( U.y * V.z ) - ( U.z * V.y ));
            B.y = B.y - (( U.x * V.z ) - ( U.z * V.x ));
            B.z = B.z + (( U.x * V.y ) - ( U.y * V.x ));
        }
    }
    return B;
}

double norm_of(const cartesian& vec)
{
    double norm = sqrt(( vec.x * vec.x ) + ( vec.y * vec.y ) + ( vec.z * vec.z ));
    return ( norm );
}

void R_vectors(Coil** coils, const cartesian& point, const int act_coil, Coil& Rmi, Coil& Rmf)
{
    for (unsigned i = 0; i < TOTAL_OF_GRADES; i++)
    {
        Rmi.x[i] = point.x - coils[act_coil]->x[i];
        Rmi.y[i] = point.y - coils[act_coil]->y[i];
        Rmi.z[i] = point.z - coils[act_coil]->z[i];
        Rmf.x[i] = point.x - coils[act_coil]->x[i + 1];
        Rmf.y[i] = point.y - coils[act_coil]->y[i + 1];
        Rmf.z[i] = point.z - coils[act_coil]->z[i + 1];
    }
}

void RK4(Coil** coils, const cartesian& start_point, const unsigned int steps, const double& step_size, const int path, const int mode)
{
    cartesian p0;
    cartesian p1 = {0, 0, 0};
    cartesian p2 = {0, 0, 0};
    cartesian p3 = {0, 0, 0};
    cartesian K1;
    cartesian K2;
    cartesian K3;
    cartesian K4;
    cartesian Ovect = {0, 0, 0};
    cartesian p = {0, 0, 0};
    cartesian r_vector;
    double norm_temp;
    double r_radius;
    double actual_state;

    FileHandler handler(path);

    handler.write(start_point.x, start_point.y, start_point.z);

    p0 = start_point;
    const double steps_inverse = static_cast<double>(1) / steps;
    const int onePercent = static_cast<int>(steps / 100);

    for (unsigned i = 1; i < steps; i++)
    {
        K1 = magnetic_field(coils, p0);
        norm_temp = norm_of(K1);
        K1.x = ( K1.x / norm_temp ) * step_size;
        K1.y = ( K1.y / norm_temp ) * step_size;
        K1.z = ( K1.z / norm_temp ) * step_size;
        p1.x = ( K1.x / 2 ) + p0.x;
        p1.y = ( K1.y / 2 ) + p0.y;
        p1.z = ( K1.z / 2 ) + p0.z;

        K2 = magnetic_field(coils, p1);
        norm_temp = norm_of(K2);
        K2.x = ( K2.x / norm_temp ) * step_size;
        K2.y = ( K2.y / norm_temp ) * step_size;
        K2.z = ( K2.z / norm_temp ) * step_size;
        p2.x = ( K2.x / 2 ) + p0.x;
        p2.y = ( K2.y / 2 ) + p0.y;
        p2.z = ( K2.z / 2 ) + p0.z;

        K3 = magnetic_field(coils, p2);
        norm_temp = norm_of(K3);
        K3.x = ( K3.x / norm_temp ) * step_size;
        K3.y = ( K3.y / norm_temp ) * step_size;
        K3.z = ( K3.z / norm_temp ) * step_size;
        p3.x = K3.x + p0.x;
        p3.y = K3.y + p0.y;
        p3.z = K3.z + p0.z;

        K4 = magnetic_field(coils, p3);
        norm_temp = norm_of(K4);
        K4.x = ( K4.x / norm_temp ) * step_size;
        K4.y = ( K4.y / norm_temp ) * step_size;
        K4.z = ( K4.z / norm_temp ) * step_size;
        p0.x = p0.x + (( K1.x + 2 * K2.x + 2 * K3.x + K4.x ) / 6 );
        p0.y = p0.y + (( K1.y + 2 * K2.y + 2 * K3.y + K4.y ) / 6 );
        p0.z = p0.z + (( K1.z + 2 * K2.z + 2 * K3.z + K4.z ) / 6 );

        handler.write(p0.x, p0.y, p0.z);

        if (mode == 1)
        {
            p.x = p0.x;
            p.y = p0.y;
            Ovect.x = ( p.x / norm_of(p)) * 0.2381; //// Origen vector
            Ovect.y = ( p.y / norm_of(p)) * 0.2381;
            Ovect.z = 0;
            r_vector.x = p0.x - Ovect.x;
            r_vector.y = p0.y - Ovect.y;
            r_vector.z = p0.z - Ovect.z;
            r_radius = norm_of(r_vector);
            if (r_radius > 0.0944165)
            {
                handler.write(r_radius, 000, 000);
                break;
            }
        }
        if (0 == i % onePercent)
        {
            actual_state = static_cast<double>(i * 100) * steps_inverse;
            //printf("el porcentaje completado es %e\n", actual_state);
            std::cout << "El porcentaje completado es=[" << std::fixed << actual_state << "]." << std::endl;
        }
    }
}
