//
// Created by lchavarr on 4/17/16.
//


#include "solctra.h"
#include "FileHandler.h"
#include <iostream>
#include <omp.h>

void load_coil_data(double* x, double* y, double* z, const std::string& path)
{
    char coil_file[30];
    int num, point;
    double localX, localY, localZ;
    for (num = 0; num < TOTAL_OF_COILS; num++)
    {
        std::string tmp = path + "/Bobina%dm.txt";
        //Set Coil files location
        sprintf(coil_file, tmp.c_str(), num);
        //create buffer
        FILE* file_buff;
        //Open file
        file_buff = fopen(coil_file, "r");
        if (file_buff == nullptr)
        {
            printf("Error al abrir archivo \n");
        }
        else
        {
            for (point = 0; point < TOTAL_OF_GRADES + 1; point++)
            {
                fscanf(file_buff, "%le %le %le", &localX, &localY, &localZ);
                x[num * TOTAL_OF_GRADES_PADDED + point] = localX;
                y[num * TOTAL_OF_GRADES_PADDED + point] = localY;
                z[num * TOTAL_OF_GRADES_PADDED + point] = localZ;
            }
            fclose(file_buff);
        }
    }
}

void e_roof(GlobalData& data)
{
    cartesian segment;
    for (int j = 0; j < TOTAL_OF_COILS; j++)
    {
        const int base = j * TOTAL_OF_GRADES_PADDED;
#pragma ivdep
        for (int i = 0; i < TOTAL_OF_GRADES; i++)
        {
            segment.x = ( data.coils.x[base + i + 1] ) - ( data.coils.x[base + i] );
            segment.y = ( data.coils.y[base + i + 1] ) - ( data.coils.y[base + i] );
            segment.z = ( data.coils.z[base + i + 1] ) - ( data.coils.z[base + i] );
            data.leng_segment[base + i] = norm_of(segment);
            const double leng_segment_inverted = 1.0 / data.leng_segment[base + i];
            data.e_roof.x[base + i] = segment.x * leng_segment_inverted;
            data.e_roof.y[base + i] = segment.y * leng_segment_inverted;
            data.e_roof.z[base + i] = segment.z * leng_segment_inverted;
        }
    }
}

void R_vectors(double* x, double* y, double* z, const cartesian& point, Coil& Rmi, Coil& Rmf)
{
//#pragma nounroll
//#pragma ivdep
#pragma vector aligned
#pragma omp simd
    for (int i = 0; i < TOTAL_OF_GRADES; i++)
    {
        Rmi.x[i] = point.x - x[i];
        Rmi.y[i] = point.y - y[i];
        Rmi.z[i] = point.z - z[i];
    }
//#pragma nounroll
//#pragma ivdep
#pragma omp simd
//This gives a segmentation fault
//#pragma vector aligned
    for (int i = 0; i < TOTAL_OF_GRADES; i++)
    {
        Rmf.x[i] = point.x - x[i + 1];
        Rmf.y[i] = point.y - y[i + 1];
        Rmf.z[i] = point.z - z[i + 1];
    }
}

cartesian magnetic_field(const GlobalData& data, const cartesian& point)
{
    const int threads = omp_get_max_threads();
    cartesian* B_perIteration = new cartesian[threads];
#pragma omp parallel
    {
        const int myThread = omp_get_thread_num();
        //const int myThread = 0;
        B_perIteration[myThread].x = 0;
        B_perIteration[myThread].y = 0;
        B_perIteration[myThread].z = 0;
        Coil Rmi, Rmf;
        double rmiX[TOTAL_OF_GRADES + 1] __attribute__((aligned(64)));
        double rmiY[TOTAL_OF_GRADES + 1] __attribute__((aligned(64)));
        double rmiZ[TOTAL_OF_GRADES + 1] __attribute__((aligned(64)));
        double rmfX[TOTAL_OF_GRADES + 1] __attribute__((aligned(64)));
        double rmfY[TOTAL_OF_GRADES + 1] __attribute__((aligned(64)));
        double rmfZ[TOTAL_OF_GRADES + 1] __attribute__((aligned(64)));
        Rmi.x = rmiX;
        Rmi.y = rmiY;
        Rmi.z = rmiZ;
        Rmf.x = rmfX;
        Rmf.y = rmfY;
        Rmf.z = rmfZ;
        //Rmi.x = static_cast<double*>(_mm_malloc(TOTAL_OF_GRADES + 1, ALIGNMENT_SIZE));
        //Rmi.y = static_cast<double*>(_mm_malloc(TOTAL_OF_GRADES + 1, ALIGNMENT_SIZE));
        //Rmi.z = static_cast<double*>(_mm_malloc(TOTAL_OF_GRADES + 1, ALIGNMENT_SIZE));
        //Rmf.x = static_cast<double*>(_mm_malloc(TOTAL_OF_GRADES + 1, ALIGNMENT_SIZE));
        //Rmf.y = static_cast<double*>(_mm_malloc(TOTAL_OF_GRADES + 1, ALIGNMENT_SIZE));
        //Rmf.z = static_cast<double*>(_mm_malloc(TOTAL_OF_GRADES + 1, ALIGNMENT_SIZE));
        //std::cout << "before for" << std::endl;
#pragma omp for
        for (int i = 0; i < TOTAL_OF_COILS; i++)
        {
            cartesian B = {0, 0, 0};
            const int base = i * TOTAL_OF_GRADES_PADDED;
            //std::cout << "before R_vectors" << std::endl;
            R_vectors(&data.coils.x[base], &data.coils.y[base], &data.coils.z[base], point, Rmi, Rmf);
            //std::cout << "after R_vectors" << std::endl;
            const double multiplier = ( miu * I ) / ( 4 * PI );
#pragma ivdep
#pragma vector aligned
            for (int j = 0; j < TOTAL_OF_GRADES; j++)
            {
                const double norm_Rmi = sqrt((( Rmi.x[j] * Rmi.x[j] ) + ( Rmi.y[j] * Rmi.y[j] ) +
                                              ( Rmi.z[j] * Rmi.z[j] )));
                const double norm_Rmf = sqrt((( Rmf.x[j] * Rmf.x[j] ) + ( Rmf.y[j] * Rmf.y[j] ) +
                                              ( Rmf.z[j] * Rmf.z[j] )));

                //firts vector of cross product in equation 8
                cartesian U;
                U.x = multiplier * data.e_roof.x[base + j];
                U.y = multiplier * data.e_roof.y[base + j];
                U.z = multiplier * data.e_roof.z[base + j];

                //second vector of cross product in equation 8
                const double C = (
                        (( 2 * ( data.leng_segment[base + j] ) * ( norm_Rmi + norm_Rmf )) / ( norm_Rmi * norm_Rmf )) *
                        (( 1 ) / (( norm_Rmi + norm_Rmf ) * ( norm_Rmi + norm_Rmf ) -
                                  data.leng_segment[base + j] * data.leng_segment[base + j] )));

                cartesian V;
                V.x = Rmi.x[j] * C;
                V.y = Rmi.y[j] * C;
                V.z = Rmi.z[j] * C;

                //cross product in equation 8
                B.x = B.x + (( U.y * V.z ) - ( U.z * V.y ));
                B.y = B.y - (( U.x * V.z ) - ( U.z * V.x ));
                B.z = B.z + (( U.x * V.y ) - ( U.y * V.x ));
            }
            //std::cout << "after for for" << std::endl;
            B_perIteration[myThread].x += B.x;
            B_perIteration[myThread].y -= B.y;
            B_perIteration[myThread].z += B.z;
        }
        //std::cout << "after for" << std::endl;
        //_mm_free(Rmi.x);
        //std::cout << "after free x" << std::endl;
        //_mm_free(Rmi.y);
        //_mm_free(Rmi.z);
        //_mm_free(Rmf.x);
        //_mm_free(Rmf.y);
        //_mm_free(Rmf.z);
        //std::cout << "after deletes" << std::endl;
    }
    //std::cout << "after omp parallel." << std::endl;
    cartesian B = {0.0, 0.0, 0.0};
    for (int i = 0; i < threads; i++)
    {
        B.x += B_perIteration[i].x;
        B.y -= B_perIteration[i].y;
        B.z += B_perIteration[i].z;
    }
    //std::cout << "after forfor." << std::endl;
    delete[] B_perIteration;
    //std::cout << "after delete." << std::endl;
    //B.print();
    return B;
}

void RK4(const GlobalData& data, const cartesian& start_point, const int steps, const double& step_size, const int path, const int mode)
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
    const double half = 1.0 / 2.0;

    for (int i = 1; i < steps; i++)
    {
        K1 = magnetic_field(data, p0);
        //std::cout << "After magnetic fields." << std::endl;
        norm_temp = 1.0 / norm_of(K1);
        K1.x = ( K1.x * norm_temp ) * step_size;
        K1.y = ( K1.y * norm_temp ) * step_size;
        K1.z = ( K1.z * norm_temp ) * step_size;
        p1.x = ( K1.x * half ) + p0.x;
        p1.y = ( K1.y * half ) + p0.y;
        p1.z = ( K1.z * half ) + p0.z;

        K2 = magnetic_field(data, p1);
        norm_temp = 1.0 / norm_of(K2);
        K2.x = ( K2.x * norm_temp ) * step_size;
        K2.y = ( K2.y * norm_temp ) * step_size;
        K2.z = ( K2.z * norm_temp ) * step_size;
        p2.x = ( K2.x * half ) + p0.x;
        p2.y = ( K2.y * half ) + p0.y;
        p2.z = ( K2.z * half ) + p0.z;

        K3 = magnetic_field(data, p2);
        norm_temp = 1.0 / norm_of(K3);
        K3.x = ( K3.x * norm_temp ) * step_size;
        K3.y = ( K3.y * norm_temp ) * step_size;
        K3.z = ( K3.z * norm_temp ) * step_size;
        p3.x = K3.x + p0.x;
        p3.y = K3.y + p0.y;
        p3.z = K3.z + p0.z;

        K4 = magnetic_field(data, p3);
        norm_temp = 1.0 / norm_of(K4);
        K4.x = ( K4.x * norm_temp ) * step_size;
        K4.y = ( K4.y * norm_temp ) * step_size;
        K4.z = ( K4.z * norm_temp ) * step_size;
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
            std::cout << "El porcentaje completado es=[" << std::fixed << actual_state << "]." << std::endl;
        }
    }
}

void runParticles(const GlobalData& data, const int particles, const int steps, const double& step_size, const int mode)
{
    cartesian A={0,0,0};
    for(int i=0; i < particles ; ++i)
    {
        //A.z=-0.02111;
        A.x=1.87451e-01+0.00474228/2*i;
        //A.x=1.87451e-01+0.00474228/2*i;//0.19775;//+0.00474228/2*i;A.x=1.87451e-01+0.00474228/2*i; 
      
        RK4(data, A,steps,step_size,i, mode);
    }
}
