
#include "solctra.h"

int main()
{
    float spoint[3];
    cartesian A={0,0,0},B;          // A:start point of field line
    int i;                          // i:Número de líne en calculo
    int mode;

    mode=1;
    load_coil_data();
    e_roof();




    /* for (i=46;i<47;i=i++){
         A.x=(1.982197e-01)+0.00474228/2*i;
         //A.z=-0.02111;
         //A.x=(0.2381-0.0944165)+0.00474228/2*40;
         //A.x=1.87451e-01+0.00474228/2*i;//0.19775;//+0.00474228/2*i;A.x=1.87451e-01+0.00474228/2*i;
         RK4(A,500000,0.001,i,1);

     }*/

    A.x=2.284e-01;
    A.z=-0.0295;
    //A.x=(0.2381-0.0944165)+0.00474228/2*40;
    //A.x=1.87451e-01+0.00474228/2*i;//0.19775;//+0.00474228/2*i;A.x=1.87451e-01+0.00474228/2*i;
    RK4(A,500000,0.001,5,1);



/*
    return 0;
  ///////

    float spoint[3];
    cartesian A={0,0,0},B;          // A:start point of field line
    int i;                          // i:Número de líne en calculo
    int b;


    load_coil_data();

    e_roof();

    for (i=0;i<30;i++){

        //A.z=-0.02111;

        A.x=1.87451e-01+0.00474228/2*i;
        //A.x=1.87451e-01+0.00474228/2*i;//0.19775;//+0.00474228/2*i;A.x=1.87451e-01+0.00474228/2*i;

      RK4(A,1000000,0.001,i);

    }
*/
    return (5);
}