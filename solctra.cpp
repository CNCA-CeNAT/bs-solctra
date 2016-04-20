//
// Created by lchavarr on 4/17/16.
//


#include "solctra.h"
#include "FileHandler.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <iostream>

struct coil num_coil[12], vec_e_roof[12];
float leng_segment[12][360];

void e_roof(void)
{
    cartesian segment;
    for (int j = 0; j < 12; j++)
    {
        for (int i = 0; i < 360; i++)
        {
            segment.x = ( num_coil[j].x[i + 1] ) - ( num_coil[j].x[i] );
            segment.y = ( num_coil[j].y[i + 1] ) - ( num_coil[j].y[i] );
            segment.z = ( num_coil[j].z[i + 1] ) - ( num_coil[j].z[i] );
            leng_segment[j][i] = norm_of(segment);
            vec_e_roof[j].x[i] = segment.x / leng_segment[j][i];
            vec_e_roof[j].y[i] = segment.y / leng_segment[j][i];
            vec_e_roof[j].z[i] = segment.z / leng_segment[j][i];
        }
    }
}

void load_coil_data(void)
{
    char coil_file[30];
    int num, point;
    for (num = 0; num < 12; num++)
    {
        //Set coil files location
        sprintf(coil_file, "resources/Bobina%dm.txt", num);
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
            for (point = 0; point < 361; point++)
            {
                fscanf(file_buff, "%e %e %e", &num_coil[num].x[point], &num_coil[num].y[point], &num_coil[num]
                        .z[point]);
            }
            fclose(file_buff);
        }
    }
}

cartesian magnetic_field(const cartesian& point)
{
    cartesian B = {0, 0, 0};
    cartesian B_thread[12];
#pragma omp parallel for
    for (int i = 0; i < 12; i++)
    {
        //float norm_Rmi;
        //float norm_Rmf;
        //float C;
        B_thread[i].x = 0;
        B_thread[i].y = 0;
        B_thread[i].z = 0;
        struct coil Rmi, Rmf;
        R_vectors(point, i, Rmi, Rmf);
        for (int j = 0; j < 360; j++)
        {
            cartesian U;
            const float norm_Rmi = sqrt((( pow(Rmi.x[j], 2)) + ( pow(Rmi.y[j], 2)) + ( pow(Rmi.z[j], 2))));
            const float norm_Rmf = sqrt((( pow(Rmf.x[j], 2)) + ( pow(Rmf.y[j], 2)) + ( pow(Rmf.z[j], 2))));

            //firts vector of cross product in equation 8
            U.x = (( miu * I ) / ( 4 * PI )) * vec_e_roof[i].x[j];
            U.y = (( miu * I ) / ( 4 * PI )) * vec_e_roof[i].y[j];
            U.z = (( miu * I ) / ( 4 * PI )) * vec_e_roof[i].z[j];

            //second vector of cross product in equation 8
            const float C = ((( 2 * ( leng_segment[i][j] ) * ( norm_Rmi + norm_Rmf )) / ( norm_Rmi * norm_Rmf )) *
                 (( 1 ) / ( pow(( norm_Rmi + norm_Rmf ), 2) - pow(leng_segment[i][j], 2))));

            cartesian V;
            V.x = Rmi.x[j] * C;
            V.y = Rmi.y[j] * C;
            V.z = Rmi.z[j] * C;

            //cross product in equation 8
            B_thread[i].x = B_thread[i].x + (( U.y * V.z ) - ( U.z * V.y ));
            B_thread[i].y = B_thread[i].y - (( U.x * V.z ) - ( U.z * V.x ));
            B_thread[i].z = B_thread[i].z + (( U.x * V.y ) - ( U.y * V.x ));
        }
    }
    for(int i = 0 ; i < 12 ; ++i)
    {
        B.x += B_thread[i].x;
        B.y += B_thread[i].y;
        B.z += B_thread[i].z;
    }
    return B;
}

float norm_of(const cartesian& vec)
{
    float norm = sqrt(( vec.x * vec.x ) + ( vec.y * vec.y ) + ( vec.z * vec.z ));
    return ( norm );
}

void R_vectors(const cartesian& point, const int act_coil, struct coil& Rmi, struct coil& Rmf)
{
    for (int i = 0; i < 360; i++)
    {
        Rmi.x[i] = point.x - num_coil[act_coil].x[i];
        Rmi.y[i] = point.y - num_coil[act_coil].y[i];
        Rmi.z[i] = point.z - num_coil[act_coil].z[i];
        Rmf.x[i] = point.x - num_coil[act_coil].x[i + 1];
        Rmf.y[i] = point.y - num_coil[act_coil].y[i + 1];
        Rmf.z[i] = point.z - num_coil[act_coil].z[i + 1];
    }
}

void RK4(const cartesian& start_point, const unsigned int steps, const float& step_size, const int path, const int mode)
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
    float norm_temp;
    float r_radius;
    float actual_state;

    FileHandler handler(path);

    handler.write(start_point.x, start_point.y, start_point.z);

    p0 = start_point;

    for (unsigned i = 1; i < steps; i++)
    {
        K1 = magnetic_field(p0);
        norm_temp = norm_of(K1);
        K1.x = ( K1.x / norm_temp ) * step_size;
        K1.y = ( K1.y / norm_temp ) * step_size;
        K1.z = ( K1.z / norm_temp ) * step_size;
        p1.x = ( K1.x / 2 ) + p0.x;
        p1.y = ( K1.y / 2 ) + p0.y;
        p1.z = ( K1.z / 2 ) + p0.z;

        K2 = magnetic_field(p1);
        norm_temp = norm_of(K2);
        K2.x = ( K2.x / norm_temp ) * step_size;
        K2.y = ( K2.y / norm_temp ) * step_size;
        K2.z = ( K2.z / norm_temp ) * step_size;
        p2.x = ( K2.x / 2 ) + p0.x;
        p2.y = ( K2.y / 2 ) + p0.y;
        p2.z = ( K2.z / 2 ) + p0.z;

        K3 = magnetic_field(p2);
        norm_temp = norm_of(K3);
        K3.x = ( K3.x / norm_temp ) * step_size;
        K3.y = ( K3.y / norm_temp ) * step_size;
        K3.z = ( K3.z / norm_temp ) * step_size;
        p3.x = K3.x + p0.x;
        p3.y = K3.y + p0.y;
        p3.z = K3.z + p0.z;

        K4 = magnetic_field(p3);
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
        actual_state = static_cast<float>(i * 100) / static_cast<float>(steps);
        if (actual_state <= 10)
        {
            //printf("el porcentaje completado es %e\n", actual_state);
            std::cout << "El porcentaje completado es=[" << std::fixed << actual_state << "]." << std::endl;
        }
    }
}
