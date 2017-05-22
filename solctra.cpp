//
// Created by lchavarr on 4/17/16.
//


#include "solctra.h"
#include <omp.h>
#include <mpi.h>
#include <cstdio>

void initializeGlobals(Coil* rmi, Coil* rmf)
{
    for(unsigned int i = 0 ; i < TOTAL_OF_COILS ; ++i)
    {
        rmi[i].x = static_cast<double*>(_mm_malloc(sizeof(double) * (TOTAL_OF_GRADES + 1), ALIGNMENT_SIZE));
        rmi[i].y = static_cast<double*>(_mm_malloc(sizeof(double) * (TOTAL_OF_GRADES + 1), ALIGNMENT_SIZE));
        rmi[i].z = static_cast<double*>(_mm_malloc(sizeof(double) * (TOTAL_OF_GRADES + 1), ALIGNMENT_SIZE));
        rmf[i].x = static_cast<double*>(_mm_malloc(sizeof(double) * (TOTAL_OF_GRADES + 1), ALIGNMENT_SIZE));
        rmf[i].y = static_cast<double*>(_mm_malloc(sizeof(double) * (TOTAL_OF_GRADES + 1), ALIGNMENT_SIZE));
        rmf[i].z = static_cast<double*>(_mm_malloc(sizeof(double) * (TOTAL_OF_GRADES + 1), ALIGNMENT_SIZE));
    }
}

void finishGlobal(Coil* rmi, Coil* rmf)
{
    for(unsigned int i = 0 ; i < TOTAL_OF_COILS ; ++i)
    {
        _mm_free(rmi[i].x);
        //printf("after free x\n");;
        _mm_free(rmi[i].y);
        _mm_free(rmi[i].z);
        _mm_free(rmf[i].x);
        _mm_free(rmf[i].y);
        _mm_free(rmf[i].z);
        //printf("after deletes\n");;
    }
}
void load_coil_data(double* x, double* y, double* z, const std::string& path)
{
    for (int num = 0; num < TOTAL_OF_COILS; num++)
    {
        //char coil_file[30];
        std::string tmp = path + "/Bobina"+std::to_string(num)+"m.txt";
        //Set Coil files location
        //sprintf(coil_file, tmp.c_str(), num);
        loadFile(&(x[num * TOTAL_OF_GRADES_PADDED]), &(y[num * TOTAL_OF_GRADES_PADDED]), &(z[num * TOTAL_OF_GRADES_PADDED]), TOTAL_OF_GRADES + 1, tmp);
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

void R_vectors(const Coil& coil, const cartesian& point, Coil* Rmi, Coil* Rmf)
{
#pragma omp for
    for(unsigned int i = 0 ; i < TOTAL_OF_COILS ; ++i)
    {
        const int base = i * TOTAL_OF_GRADES_PADDED;
        double* x = &coil.x[base];
        double* y = &coil.y[base];
        double* z = &coil.z[base];
//#pragma nounroll
//#pragma ivdep
#pragma vector aligned
#pragma omp simd
        for (int j = 0; j < TOTAL_OF_GRADES; j++)
        {
            Rmi[i].x[j] = point.x - x[j];
            Rmi[i].y[j] = point.y - y[j];
            Rmi[i].z[j] = point.z - z[j];
        }
//#pragma nounroll
//#pragma ivdep
#pragma omp simd
//This gives a segmentation fault
//#pragma vector aligned
        for (int j = 0; j < TOTAL_OF_GRADES; j++)
        {
            Rmf[i].x[j] = point.x - x[j + 1];
            Rmf[i].y[j] = point.y - y[j + 1];
            Rmf[i].z[j] = point.z - z[j + 1];
        }
    }
}

cartesian magnetic_field(Coil* rmi, Coil* rmf, const GlobalData& data, const cartesian& point)
{
    const int threads = omp_get_max_threads();
    cartesian* B_perIteration = new cartesian[threads];
    const double multiplier = ( miu * I ) / ( 4 * PI );
#pragma omp parallel
    {
        //printf("before R_vectors\n");
        //R_vectors(data.coils, point, rmi, rmf);
        //printf("after R_vectors\n");
        const int myThread = omp_get_thread_num();
        //const int myThread = 0;
        B_perIteration[myThread].x = 0;
        B_perIteration[myThread].y = 0;
        B_perIteration[myThread].z = 0;
        //cartesian B = {0, 0, 0};
        double Bx = 0;
        double By = 0;
        double Bz = 0;
        //printf("before for\n");
            // **************************************************************
            // * Implementing strip-mining to allow more threads per particle
            // **************************************************************
//#pragma ivdep
//#pragma vector aligned
            //for (int j = 0; j < TOTAL_OF_GRADES; j++)
#pragma omp for collapse(2)
        for (int i = 0; i < TOTAL_OF_COILS; i++)
            for (int jj = 0; jj < TOTAL_OF_GRADES; jj += GRADES_PER_PAGE)
            {
                const unsigned final = (TOTAL_OF_GRADES < jj + GRADES_PER_PAGE) ? TOTAL_OF_GRADES : jj + GRADES_PER_PAGE;
                const int base = i * TOTAL_OF_GRADES_PADDED;
                double* x = &data.coils.x[base];
                double* y = &data.coils.y[base];
                double* z = &data.coils.z[base];
#pragma omp simd
//#pragma ivdep
//#pragma vector aligned
                for (int j = jj; j < final ; ++j)
                {
                    rmi[i].x[j] = point.x - x[j];
                    rmi[i].y[j] = point.y - y[j];
                    rmi[i].z[j] = point.z - z[j];
//                }
////#pragma ivdep
//#pragma omp simd
//                for (int j = jj; j < final ; ++j)
//                {
                    rmf[i].x[j] = point.x - x[j + 1];
                    rmf[i].y[j] = point.y - y[j + 1];
                    rmf[i].z[j] = point.z - z[j + 1];
                }
//#pragma omp simd private(B)
//#pragma omp simd private(Bx) private(By) private(Bz)
#pragma omp simd reduction(+:Bx) reduction(-:By) reduction(+:Bz)
//#pragma ivdep
#pragma vector aligned
                for (int j = jj; j < final ; ++j)
                {
                    const double norm_Rmi = sqrt((( rmi[i].x[j] * rmi[i].x[j] ) + ( rmi[i].y[j] * rmi[i].y[j] ) +
                                                  ( rmi[i].z[j] * rmi[i].z[j] )));
                    const double norm_Rmf = sqrt((( rmf[i].x[j] * rmf[i].x[j] ) + ( rmf[i].y[j] * rmf[i].y[j] ) +
                                                  ( rmf[i].z[j] * rmf[i].z[j] )));

                    //firts vector of cross product in equation 8
                    cartesian U;
                    U.x = multiplier * data.e_roof.x[base + j];
                    U.y = multiplier * data.e_roof.y[base + j];
                    U.z = multiplier * data.e_roof.z[base + j];

                    //second vector of cross product in equation 8
                    const double C = (
                            (( 2 * ( data.leng_segment[base + j] ) * ( norm_Rmi + norm_Rmf )) /
                             ( norm_Rmi * norm_Rmf )) *
                            (( 1 ) / (( norm_Rmi + norm_Rmf ) * ( norm_Rmi + norm_Rmf ) -
                                      data.leng_segment[base + j] * data.leng_segment[base + j] )));

                    cartesian V;
                    V.x = rmi[i].x[j] * C;
                    V.y = rmi[i].y[j] * C;
                    V.z = rmi[i].z[j] * C;

                    //cross product in equation 8
                    //B.x = B.x + (( U.y * V.z ) - ( U.z * V.y ));
                    //B.y = B.y - (( U.x * V.z ) - ( U.z * V.x ));
                    //B.z = B.z + (( U.x * V.y ) - ( U.y * V.x ));
                    Bx = Bx + (( U.y * V.z ) - ( U.z * V.y ));
                    By = By - (( U.x * V.z ) - ( U.z * V.x ));
                    Bz = Bz + (( U.x * V.y ) - ( U.y * V.x ));
                }
            }
        //std::cout << "after for for" << std::endl;
//        B_perIteration[myThread].x += B.x;
//        B_perIteration[myThread].y -= B.y;
//        B_perIteration[myThread].z += B.z;
        B_perIteration[myThread].x += Bx;
        B_perIteration[myThread].y -= By;
        B_perIteration[myThread].z += Bz;
    }
    //printf("after for\n");;

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

void RK4(const GlobalData& data, const std::string& output, const cartesian& start_point, const int steps, const double& step_size, const int particle, const int mode)
{
    Coil rmi[TOTAL_OF_COILS];
    Coil rmf[TOTAL_OF_COILS];
    //printf("Before initializeGlobals\n");
    initializeGlobals(rmi, rmf);
    //printf("RK4 begins...\n");
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
    //double actual_state;

    int myRank;
    int commSize;
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    FILE* handler;
    std::string file_name = output +  "/path" + getZeroPadded(particle) + ".txt";
    handler = fopen(file_name.c_str(), "w");
    if(nullptr == handler)
    {
        printf("Unable to open file=[%s]. Nothing to do\n", file_name.c_str());
        exit(0);
    }
    fprintf(handler, "%e\t%e\t%e\n", start_point.x, start_point.y, start_point.z);

    p0 = start_point;
    //const double steps_inverse = static_cast<double>(1) / steps;
    //const int onePercent = static_cast<int>(steps / 100);
    const double half = 1.0 / 2.0;

    for (int i = 1; i < steps; i++)
    {
        K1 = magnetic_field(rmi, rmf, data, p0);
        //printf("After magnetic fields.\n");
        norm_temp = 1.0 / norm_of(K1);
        K1.x = ( K1.x * norm_temp ) * step_size;
        K1.y = ( K1.y * norm_temp ) * step_size;
        K1.z = ( K1.z * norm_temp ) * step_size;
        p1.x = ( K1.x * half ) + p0.x;
        p1.y = ( K1.y * half ) + p0.y;
        p1.z = ( K1.z * half ) + p0.z;

        K2 = magnetic_field(rmi, rmf, data, p1);
        norm_temp = 1.0 / norm_of(K2);
        K2.x = ( K2.x * norm_temp ) * step_size;
        K2.y = ( K2.y * norm_temp ) * step_size;
        K2.z = ( K2.z * norm_temp ) * step_size;
        p2.x = ( K2.x * half ) + p0.x;
        p2.y = ( K2.y * half ) + p0.y;
        p2.z = ( K2.z * half ) + p0.z;

        K3 = magnetic_field(rmi, rmf, data, p2);
        norm_temp = 1.0 / norm_of(K3);
        K3.x = ( K3.x * norm_temp ) * step_size;
        K3.y = ( K3.y * norm_temp ) * step_size;
        K3.z = ( K3.z * norm_temp ) * step_size;
        p3.x = K3.x + p0.x;
        p3.y = K3.y + p0.y;
        p3.z = K3.z + p0.z;

        K4 = magnetic_field(rmi, rmf, data, p3);
        norm_temp = 1.0 / norm_of(K4);
        K4.x = ( K4.x * norm_temp ) * step_size;
        K4.y = ( K4.y * norm_temp ) * step_size;
        K4.z = ( K4.z * norm_temp ) * step_size;
        p0.x = p0.x + (( K1.x + 2 * K2.x + 2 * K3.x + K4.x ) / 6 );
        p0.y = p0.y + (( K1.y + 2 * K2.y + 2 * K3.y + K4.y ) / 6 );
        p0.z = p0.z + (( K1.z + 2 * K2.z + 2 * K3.z + K4.z ) / 6 );

        fprintf(handler, "%e\t%e\t%e\n", p0.x, p0.y, p0.z);

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
                fprintf(handler, "%e\t%e\t%e\n", r_radius, 0.0, 0.0);
                break;
            }
        }
        //if (0 == i % (onePercent * 10))
        //{
        //    actual_state = static_cast<double>(i * 100) * steps_inverse;
        //    printf("El porcentaje completado para particula=[%d] es=[%f].\n", particle, actual_state);
        //}
    }
    fclose(handler);
}

void runParticles(const GlobalData& data, const std::string& output, const Coil& particles, const int length, const int steps, const double& step_size, const int mode)
{
    int myRank;
    int commSize;
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    cartesian A={0,0,0};
    for(int i=0; i < length ; ++i)
    {
        if(i % commSize == myRank)
        {
            A.x = particles.x[i];
            A.y = particles.y[i];
            A.z = particles.z[i];
            //A.z=-0.02111;
            //A.x=1.87451e-01+0.00474228/2*i;
            //A.x=1.87451e-01+0.00474228/2*i;//0.19775;//+0.00474228/2*i;A.x=1.87451e-01+0.00474228/2*i;
            printf("Rank=[%d] working on particle=[%d] with initial point=", myRank, i);
            A.print();
            RK4(data, output, A,steps,step_size,i, mode);
        }
    }
}
