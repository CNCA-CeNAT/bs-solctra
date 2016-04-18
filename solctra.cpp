//
// Created by lchavarr on 4/17/16.
//


#include "solctra.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>

struct coil num_coil[12],vec_e_roof[12],Rmi,Rmf;
float   leng_segment[12][360];

void    e_roof(void) {

    int i,j;

    cartesian segment;

    for (j=0;j<12;j++){

        for (i=0;i<360;i++){

            segment.x=(num_coil[j].x[i+1])-(num_coil[j].x[i]);

            segment.y=(num_coil[j].y[i+1])-(num_coil[j].y[i]);

            segment.z=(num_coil[j].z[i+1])-(num_coil[j].z[i]);

            leng_segment[j][i]=norm_of(segment);

            vec_e_roof[j].x[i]=segment.x/leng_segment[j][i];

            vec_e_roof[j].y[i]=segment.y/leng_segment[j][i];

            vec_e_roof[j].z[i]=segment.z/leng_segment[j][i];


        }

    }


}

void    load_coil_data(void){

    char coil_file[30];

    int num, point;


    for (num=0;num<12;num++){

        //Set coil files location

        sprintf(coil_file,"resources/Bobina%dm.txt",num);

        //create buffer

        FILE * file_buff;

        //Open file

        file_buff = fopen(coil_file, "r");

        if(file_buff==NULL)

            printf("Error al abrir archivo \n");

        else{

            for (point=0;point<361;point++){

                fscanf(file_buff,"%e %e %e",&num_coil[num].x[point],&num_coil[num].y[point],&num_coil[num].z[point]);

            }

            fclose(file_buff);

        }
    }

}

cartesian load_path(int point){
    cartesian p;
    char coil_file[30];
    int num,i;


    //Set coil files location
    num=1;
    sprintf(coil_file,"resources/particle%d.txt",num);
    //printf("Read x           Read y           Read z%s\n",coil_file);
    //create buffer
    FILE * file_buff;
    //Open file
    file_buff = fopen(coil_file, "a+");
    if(file_buff==NULL)
        printf("Error al abrir archivo \n");
    else{


        //printf("coil %d\n",num);
        //printf("Read x           Read y           Read z\n");

        //Load line by line each coordinate coil
        //fscanf(file_buff,"%E %E %E",&num_coil[num].x[point],&num_coil[num].y[point],&num_coil[num].z[point]);
        //fscanf(file_buff,"%e,%e,%e\n",&num_coil[num].x[point],&num_coil[num].y[point],&num_coil[num].z[point]);
        for (i=1;i<point+1;i++){
            fscanf(file_buff,"%e ",&p.x);
            ///fscanf(file_buff," ",trash);
            fscanf(file_buff,"%e ",&p.y);
            //scanf(file_buff," ",trash);25924914
            fscanf(file_buff,"%e",&p.z);
        }

        //save_data(num,num_coil[num].x[point],num_coil[num].y[point],num_coil[num].z[point]);
        /*printf("%d ",point);
        printf("%e ",num_coil[num].x[point]); */
        //printf("%e",num_coil[0].z[0]);
        //printf("%e\n",num_coil[num].x[point]);
        //printf("coil %d  %d\n",num,point);*/
        //printf("%d->>%e %e %e\n",num,num_coil[num].x[point],num_coil[num].y[point],num_coil[num].z[point]);
        //printf("%e %e %e\n",num_coil[0].x[0],num_coil[0].y[0],num_coil[0].z[0]);
        fclose(file_buff);

        // printf("---%e\n",num_coil[0].z[0]);
    }

    return p;
}

void            R_vectors(float,float,float,int);

cartesian     magnetic_field(cartesian point) {

    cartesian B={0,0,0},V={0,0,0}, U={0,0,0};

    float norm_Rmi,norm_Rmf,C;

    int i,j;

    for (i=0;i<12;i++){

        R_vectors(point.x,point.y,point.z,i);

        for (j=0;j<360;j++){

            norm_Rmi=sqrt(((pow(Rmi.x[j],2))+(pow(Rmi.y[j],2))+(pow(Rmi.z[j],2))));

            norm_Rmf=sqrt(((pow(Rmf.x[j],2))+(pow(Rmf.y[j],2))+(pow(Rmf.z[j],2))));

            //firts vector of cross product in equation 8
            U.x=((miu*I)/(4*PI))*vec_e_roof[i].x[j];

            U.y=((miu*I)/(4*PI))*vec_e_roof[i].y[j];

            U.z=((miu*I)/(4*PI))*vec_e_roof[i].z[j];


            //second vector of cross product in equation 8
            C=(((2*(leng_segment[i][j])*(norm_Rmi+norm_Rmf))/(norm_Rmi*norm_Rmf))*((1)/(pow((norm_Rmi+norm_Rmf),2)-pow(leng_segment[i][j],2))));

            V.x=Rmi.x[j]*C;

            V.y=Rmi.y[j]*C;

            V.z=Rmi.z[j]*C;


            //cross product in equation 8
            B.x=B.x+((U.y*V.z)-(U.z*V.y));

            B.y=B.y-((U.x*V.z)-(U.z*V.x));

            B.z=B.z+((U.x*V.y)-(U.y*V.x));


        }


    }

    return B;
}

float norm_of(cartesian vec) {

    float norm=sqrt((vec.x*vec.x)+(vec.y*vec.y)+(vec.z*vec.z));

    return (norm);
}

void    R_vectors(float xx, float yy, float zz, int act_coil) {

    int i;

    for (i=0;i<360;i++){

        Rmi.x[i]=xx-num_coil[act_coil].x[i];

        Rmi.y[i]=yy-num_coil[act_coil].y[i];

        Rmi.z[i]=zz-num_coil[act_coil].z[i];


        Rmf.x[i]=xx-num_coil[act_coil].x[i+1];

        Rmf.y[i]=yy-num_coil[act_coil].y[i+1];

        Rmf.z[i]=zz-num_coil[act_coil].z[i+1];


    }


}

cartesian   magnetic_field(cartesian);

void    RK4(cartesian start_point,int steps,float step_size, int path,int mode){

    cartesian p0={0,0,0}, p1={0,0,0}, p2={0,0,0},p3={0,0,0},K1={0,0,0},K2={0,0,0},K3={0,0,0},K4={0,0,0}, Ovect={0,0,0},p={0,0,0},r_vector;

    float norm_temp,r_radius;

    int i;
    float actual_state;

    save_data(path,start_point.x,start_point.y,start_point.z);

    p0=start_point;

    for (i=1;i<steps;i++){


        K1=magnetic_field(p0);

        norm_temp=norm_of(K1);

        K1.x=(K1.x/norm_temp)*step_size;

        K1.y=(K1.y/norm_temp)*step_size;

        K1.z=(K1.z/norm_temp)*step_size;


        p1.x=(K1.x/2)+p0.x;

        p1.y=(K1.y/2)+p0.y;

        p1.z=(K1.z/2)+p0.z;

        K2=magnetic_field(p1);

        norm_temp=norm_of(K2);

        K2.x=(K2.x/norm_temp)*step_size;

        K2.y=(K2.y/norm_temp)*step_size;

        K2.z=(K2.z/norm_temp)*step_size;


        p2.x=(K2.x/2)+p0.x;

        p2.y=(K2.y/2)+p0.y;

        p2.z=(K2.z/2)+p0.z;

        K3=magnetic_field(p2);

        norm_temp=norm_of(K3);

        K3.x=(K3.x/norm_temp)*step_size;

        K3.y=(K3.y/norm_temp)*step_size;

        K3.z=(K3.z/norm_temp)*step_size;


        p3.x=K3.x+p0.x;

        p3.y=K3.y+p0.y;

        p3.z=K3.z+p0.z;

        K4=magnetic_field(p3);

        norm_temp=norm_of(K4);

        K4.x=(K4.x/norm_temp)*step_size;

        K4.y=(K4.y/norm_temp)*step_size;

        K4.z=(K4.z/norm_temp)*step_size;


        p0.x=p0.x+((K1.x+2*K2.x+2*K3.x+K4.x)/6);

        p0.y=p0.y+((K1.y+2*K2.y+2*K3.y+K4.y)/6);

        p0.z=p0.z+((K1.z+2*K2.z+2*K3.z+K4.z)/6);


        save_data(path, p0.x,p0.y,p0.z);

        if (mode==1){

            p.x=p0.x;
            p.y=p0.y;
            Ovect.x=(p.x/norm_of(p))*0.2381; //// Origen vector
            Ovect.y=(p.y/norm_of(p))*0.2381;
            Ovect.z=0;
            r_vector.x=p0.x-Ovect.x;
            r_vector.y=p0.y-Ovect.y;
            r_vector.z=p0.z-Ovect.z;
            r_radius=norm_of(r_vector);
            if (r_radius>0.0944165){
                save_data(path,r_radius,000,000);
                break;
            }
        }
        actual_state=i/steps*100;
        if(actual_state<=10){

            printf("el porcentaje completado es %e\n",actual_state);
        }

    }


}

int save_data(int phat_num,float x,float y, float z) {

    char file_name[20];

    FILE *fp;

    sprintf(file_name,"results/path%d.txt",phat_num);

    /* open the file */
    fp = fopen(file_name, "a");

    if (fp == NULL) {

        printf("Unable to open %d for appending. Nothing to do\n",file_name);

        exit(0);
    }

    // write to the file
    fprintf(fp, "%e\t%e\t%e\n",x,y,z);

    // close the file
    fclose(fp);

    return 0;
}
