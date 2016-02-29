#include "particula.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

particula::particula()
{
    xc=yc=0;
    Hxc=Hyc=5;
    //ctor
}
particula::particula(int x1, int y1, int hx1, int samples)
{
    xc=x1;
    yc=y1;
    Hxc=hx1;
    //Hyc=hy1;
    numero=samples;
    oclusion=false;

    //ctor
}
void particula::variacion(int xo1, int yo1, int anc, int alt,int M, int limx, int limy)
{   float c1=0, c2=0;
     //printf("ancho=%i, alto=%i\n",anc, alt);
    for (int i=0;i<numero;i++)
    {
        //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;

        cvRand( &rng, x_k );
        cvRand( &ran, C_k );
        xc=xo1 + CV_MAT_ELEM( *x_k, float, 0, 0 );
        yc=yo1 + CV_MAT_ELEM( *x_k, float, 1, 0 );
        c1=CV_MAT_ELEM( *C_k, float, 0, 0 );
        c2=CV_MAT_ELEM( *C_k, float, 1, 0 );
        Hxc = anc + c1;
        Hyc = alt + c2;
        //printf("varHxc=%f varHyc=%f i=%i\n",c1,c2,i);
        //printf("xc=%f yc=%f Hxc=%f Hyc=%f i=%i\n",xc, yc, Hxc, Hyc,i);
        particula::control(M,limx, limy);
        particulas_c[0][i]=particulas_c[8][i]=xc;
        particulas_c[1][i]=particulas_c[9][i]=yc;
        particulas_c[2][i]=particulas_c[10][i]=Hxc;
        particulas_c[3][i]=particulas_c[11][i]=Hyc;

        particulas_h[0][i]=particulas_h[8][i]=xc;
        particulas_h[1][i]=particulas_h[9][i]=yc;
        particulas_h[2][i]=particulas_h[10][i]=Hxc;
        particulas_h[3][i]=particulas_h[11][i]=Hyc;


        particulas_l[0][i]=particulas_l[8][i]=xc;
        particulas_l[1][i]=particulas_l[9][i]=yc;
        particulas_l[2][i]=particulas_l[10][i]=Hxc;
        particulas_l[3][i]=particulas_l[11][i]=Hyc;


        //printf("xc=%f yc=%f Hxc=%f Hyc=%f i=%i\n",xc, yc, Hxc, Hyc,i);

    }

}

void particula::Inicia(float sig, float sig1)
{


   cvRandInit( &rng, 0,2, 0, CV_RAND_NORMAL );
   cvRandInit( &rng2, 0, 2, 0, CV_RAND_NORMAL );
   cvRandInit( &ran, 0, 0.5, 0, CV_RAND_NORMAL );
   cvRandInit( &U, 0, 1/numero, 0, CV_RAND_UNI );
   cvRandInit( &V, 0, 3, 0, CV_RAND_NORMAL );
   C_k = cvCreateMat( 2, 1, CV_32FC1 );
   x_k = cvCreateMat( 2, 1, CV_32FC1 );
   U_k = cvCreateMat( 1, 1, CV_32FC1 );
}

void particula::normaliza(float suma, float Estado[12][3])
{   float wi=0,sum=0;
    int Nc,Nh, Nl;
    Nc=Estado[11][0];
    Nh=Estado[11][1];
    Nl=Estado[11][2];

    for(int i=0;i<Nc;i++)
    {
        wi=particulas_c[12][i];
        particulas_c[12][i]=wi/Estado[5][0];
        sum=sum+particulas_c[12][i]*particulas_c[12][i];
        //printf("sum=%f Neff=%f\n",sum,Neff);
    }

    Neff_c=1/sum;
    wi=sum=0;
    for(int i=0;i<Nh;i++)
    {
        wi=particulas_h[12][i];
        particulas_h[12][i]=wi/Estado[5][1];
        sum=sum+particulas_h[12][i]*particulas_h[12][i];
        //printf("sum=%f Neff=%f\n",sum,Neff);
    }

    Neff_h=1/sum;
    wi=sum=0;
    for(int i=0;i<Nl;i++)
    {
        wi=particulas_l[12][i];
        particulas_l[12][i]=wi/Estado[5][2];
        sum=sum+particulas_l[12][i]*particulas_l[12][i];
        //printf("sum=%f Neff=%f\n",sum,Neff);
    }

    Neff_l=1/sum;


  /*  for(int i=0;i<numero;i++)
    {
        wi=particulas[5][i];
        particulas[5][i]=wi/suma;
        sum=sum+particulas[5][i]*particulas[5][i];
        //printf("sum=%f Neff=%f\n",sum,Neff);
    }

    Neff=1/sum;*/
//   printf("Neff_c=%i Neff_h=%i Neff_l=%i \n",Neff_c,Neff_h,Neff_l);
}

void particula::resampling(float Estado[12][3])
{   float u1;
    int k, m;
    int Nc,Nh, Nl;
    Nc=Estado[11][0];
    Nh=Estado[11][1];
    Nl=Estado[11][2];

    if(Neff_c<Nc*0.6)
    {
        //printf("resampling cor\n");
        particulas_c[6][0]=0;
//  PDF
        for (int i=1;i<Nc;i++)
        {
            particulas_c[6][i]=particulas_c[6][i-1]+particulas_c[5][i];

        }

        cvRand( &U, U_k );
        u1=CV_MAT_ELEM( *U_k, float, 0, 0 );
        //u1=0;
        k=0;
        m=0;

        for (int j=0;j<Nc;j++)
        {
            m++;
            particulas_c[7][j]=u1+(m-1)/Nc;
            //printf("c%f u1=%f\n",particulas_c[7][j],1/numero);
            //printf("ci=%f uj=%f j=%i\n",particulas_c[6][k],particulas_c[7][j],j);

            while(particulas_c[7][j]>particulas_c[6][k]){
                k=k+1;

            if(k>Nc-2){
            k=Nc-1;
            break;
            }
                }

            //printf("k=%i j=%i,num=%f\n",k,j,numero);
            particulas_c[0][j]=particulas_c[0][k];
            particulas_c[1][j]=particulas_c[1][k];
            particulas_c[2][j]=particulas_c[2][k];
            particulas_c[3][j]=particulas_c[3][k];
            particulas_c[8][j]=particulas_c[8][k];
            particulas_c[9][j]=particulas_c[9][k];
            //printf("xc=%f,yc=%f,i=%i Hxc=%f,Hyc=%f\n",particulas_c[0][j], particulas_c[1][j],j, particulas_c[5][j], particulas_c[6][j]);
            //printf("wi=%f i=%i xk=%f,yk=%f,k=%i Hxck=%f,Hyck=%f\n",particulas_c[5][j],j,particulas_c[0][k], particulas_c[1][k],k, particulas_c[2][k], particulas_c[3][k]);
             particulas_c[5][j]=1/Nc;
             particulas_c[12][j]=1/Nc;
        }
    }



    if(Neff_h<Nh*0.6)
    {
        //printf("resampling hough\n");
        particulas_h[6][0]=0;
//  PDF
        for (int i=1;i<Nh;i++)
        {
            particulas_h[6][i]=particulas_h[6][i-1]+particulas_h[5][i];

        }

        cvRand( &U, U_k );
        u1=CV_MAT_ELEM( *U_k, float, 0, 0 );
        //u1=0;
        k=0;
        m=0;

        for (int j=0;j<Nh;j++)
        {
            m++;
            particulas_h[7][j]=u1+(m-1)/Nh;
            //printf("c%f u1=%f\n",particulas_h[7][j],1/numero);
            //printf("ci=%f uj=%f j=%i\n",particulas_h[6][k],particulas_h[7][j],j);

            while(particulas_h[7][j]>particulas_h[6][k]){
                k=k+1;

            if(k>Nh-2){
            k=Nh-1;
            break;
            }
                }

            //printf("k=%i j=%i,num=%f\n",k,j,numero);
            particulas_h[0][j]=particulas_h[0][k];
            particulas_h[1][j]=particulas_h[1][k];
            particulas_h[2][j]=particulas_h[2][k];
            particulas_h[3][j]=particulas_h[3][k];
            particulas_h[8][j]=particulas_h[8][k];
            particulas_h[9][j]=particulas_h[9][k];
            //printf("xc=%f,yc=%f,i=%i Hxc=%f,Hyc=%f\n",particulas_h[0][j], particulas_h[1][j],j, particulas_h[5][j], particulas_h[6][j]);
            //printf("wi=%f i=%i xk=%f,yk=%f,k=%i Hxck=%f,Hyck=%f\n",particulas_h[5][j],j,particulas_h[0][k], particulas_h[1][k],k, particulas_h[2][k], particulas_h[3][k]);
             particulas_h[5][j]=1/Nh;
             particulas_h[12][j]=1/Nh;
        }
      }




    if(Neff_l<Nl*0.6)
    {
        //printf("resampling LBP\n");
        particulas_l[6][0]=0;
//  PDF
        for (int i=1;i<Nl;i++)
        {
            particulas_l[6][i]=particulas_l[6][i-1]+particulas_l[5][i];

        }

        cvRand( &U, U_k );
        u1=CV_MAT_ELEM( *U_k, float, 0, 0 );
        //u1=0;
        k=0;
        m=0;

        for (int j=0;j<Nl;j++)
        {
            m++;
            particulas_l[7][j]=u1+(m-1)/Nl;
            //printf("c%f u1=%f\n",particulas_l[7][j],1/numero);
            //printf("ci=%f uj=%f j=%i\n",particulas_l[6][k],particulas_l[7][j],j);

            while(particulas_l[7][j]>particulas_l[6][k]){
                k=k+1;

            if(k>Nl-2){
            k=Nl-1;
            break;
            }
                }

            //printf("k=%i j=%i,num=%f\n",k,j,numero);
            particulas_l[0][j]=particulas_l[0][k];
            particulas_l[1][j]=particulas_l[1][k];
            particulas_l[2][j]=particulas_l[2][k];
            particulas_l[3][j]=particulas_l[3][k];
            particulas_l[8][j]=particulas_l[8][k];
            particulas_l[9][j]=particulas_l[9][k];
            //printf("xc=%f,yc=%f,i=%i Hxc=%f,Hyc=%f\n",particulas_l[0][j], particulas_l[1][j],j, particulas_l[5][j], particulas_l[6][j]);
            //printf("wi=%f i=%i xk=%f,yk=%f,k=%i Hxck=%f,Hyck=%f\n",particulas_l[5][j],j,particulas_l[0][k], particulas_l[1][k],k, particulas_l[2][k], particulas_l[3][k]);
             particulas_l[5][j]=1/Nl;
             particulas_l[12][j]=1/Nl;
        }
     }


}

void particula::actualizacion(int M, int limx, int limy, float Estado[12][3])
{   float lim;
    int Nc,Nh, Nl;
    Nc=Estado[11][0];
    Nh=Estado[11][1];
    Nl=Estado[11][2];

    //float divx=0,divy=0, sx=0, sy=0;

//printf("vx=%f vy=%f\n", Vx, Vy);


//oclusion color

lim=3*Estado[5][0]/numero;

    if (lim<0.1)
    {
        Estado[6][0]=1;\
    }
    else
    {
        Estado[6][0]=0;
    }

//oclusion Hough

    lim=3*Estado[5][1]/numero;

        if (lim<0.1)
        {
            Estado[6][1]=1;\
        }
        else
        {
            Estado[6][1]=0;
        }

//oclusion LBP

        lim=3*Estado[5][2]/numero;

            if (lim<0.1)
            {
                Estado[6][2]=1;\
            }
            else
            {
                Estado[6][2]=0;
            }





//printf("oclusion: %s\n",(oclusion)?"true":"false");

//if (oclusion==false)
            Vx=Estado[7][0];
            Vy=Estado[8][0];
if (Estado[6][0]==0)

{ //printf("vx=%f, vy=%f\n",Vx, Vy);
   for (int i=0;i<Nc/2;i++)
    {   float c1=0,c2=0;
        //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;
        cvRand( &V, x_k );
        cvRand( &ran, C_k );
        xc=particulas_c[0][i] + Vx+(CV_MAT_ELEM( *x_k, float, 0, 0 ));
        yc=particulas_c[1][i] + Vy+(CV_MAT_ELEM( *x_k, float, 1, 0 ));
        c1=CV_MAT_ELEM( *C_k, float, 0, 0 );
        c2=CV_MAT_ELEM( *C_k, float, 1, 0 );
        Hxc = particulas_c[2][i] +c1;
        Hyc = particulas_c[3][i] +c2;
        //printf("varHxc=%f varHyc=%f i=%i\n",c1,c2,i);
        //printf("i=%i xOFa %f yOFa %f hxa %f hya %f\n", i, particulas_c[0][i], yc, particulas_c[2][i], Hyc);
        particula::control(M,limx, limy);
        particulas_c[0][i]=xc;
        particulas_c[1][i]=yc;
        particulas_c[2][i]=Hxc;
        particulas_c[3][i]=Hyc;

        //printf("i=%i vx=%f, vy=%f\n",i, particulas_c[8][i]-particulas_c[0][i], particulas_c[9][i]-particulas_c[1][i]);
        //printf("i=%i xOF %f yOF %f hx %f hy %f\n", i, particulas_c[0][i], yc, particulas_c[2][i], Hyc);
    }




   for (int i=Nc/2;i<Nc;i++)
    {   float c1=0,c2=0;
        //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;
        cvRand( &V, x_k );
        cvRand( &ran, C_k );
        xc=particulas_c[0][i] + (CV_MAT_ELEM( *x_k, float, 0, 0 ));
        yc=particulas_c[1][i] + (CV_MAT_ELEM( *x_k, float, 1, 0 ));
        c1=CV_MAT_ELEM( *C_k, float, 0, 0 );
        c2=CV_MAT_ELEM( *C_k, float, 1, 0 );
        Hxc = particulas_c[2][i] +c1;
        Hyc = particulas_c[3][i] +c2;
        //printf("varHxc=%f varHyc=%f i=%i\n",c1,c2,i);
        //printf("i=%i xOFa %f yOFa %f hxa %f hya %f\n", i, particulas_c[0][i], yc, particulas_c[2][i], Hyc);
        particula::control(M,limx, limy);
        particulas_c[0][i]=xc;
        particulas_c[1][i]=yc;
        particulas_c[2][i]=Hxc;
        particulas_c[3][i]=Hyc;

        //printf("i=%i vx=%f, vy=%f\n",i, particulas[8][i]-particulas[0][i], particulas[9][i]-particulas[1][i]);
        //printf("i=%i xOF %f yOF %f hx %f hy %f\n", i, particulas[0][i], yc, particulas[2][i], Hyc);
    }
}
else{
    for (int i=0;i<Nc/2;i++)
     {   float c1=0,c2=0;
         //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;
         cvRand( &V, x_k );
         cvRand( &ran, C_k );
         xc=particulas_c[0][i] +2*Vx+(CV_MAT_ELEM( *x_k, float, 0, 0 ));
         yc=particulas_c[1][i] +2*Vy+(CV_MAT_ELEM( *x_k, float, 1, 0 ));
         c1=CV_MAT_ELEM( *C_k, float, 0, 0 );
         c2=CV_MAT_ELEM( *C_k, float, 1, 0 );
         Hxc = particulas_c[2][i] +c1;
         Hyc = particulas_c[3][i] +c2;
         //printf("varHxc=%f varHyc=%f i=%i\n",c1,c2,i);
         //printf("i=%i xOFa %f yOFa %f hxa %f hya %f\n", i, particulas_c[0][i], yc, particulas_c[2][i], Hyc);
         particula::control(M,limx, limy);
         particulas_c[0][i]=xc;
         particulas_c[1][i]=yc;
         particulas_c[2][i]=Hxc;
         particulas_c[3][i]=Hyc;

         //printf("i=%i vx=%f, vy=%f\n",i, particulas_c[8][i]-particulas_c[0][i], particulas_c[9][i]-particulas_c[1][i]);
         //printf("i=%i xOF %f yOF %f hx %f hy %f\n", i, particulas_c[0][i], yc, particulas_c[2][i], Hyc);
     }
    for (int i=Nc/2;i<Nc;i++)
     {   float c1=0,c2=0;
         //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;
         cvRand( &V, x_k );
         cvRand( &ran, C_k );
         xc=particulas_c[0][i] + 2*Vx+(CV_MAT_ELEM( *x_k, float, 0, 0 ));
         yc=particulas_c[1][i] + 2*Vy+(CV_MAT_ELEM( *x_k, float, 1, 0 ));
         c1=CV_MAT_ELEM( *C_k, float, 0, 0 );
         c2=CV_MAT_ELEM( *C_k, float, 1, 0 );
         Hxc = particulas_c[2][i] +c1;
         Hyc = particulas_c[3][i] +c2;
         //printf("varHxc=%f varHyc=%f i=%i\n",c1,c2,i);
         //printf("i=%i xOFa %f yOFa %f hxa %f hya %f\n", i, particulas_c[0][i], yc, particulas_c[2][i], Hyc);
         particula::control(M,limx, limy);
         particulas_c[0][i]=xc;
         particulas_c[1][i]=yc;
         particulas_c[2][i]=Hxc;
         particulas_c[3][i]=Hyc;

         //printf("i=%i vx=%f, vy=%f\n",i, particulas_c[8][i]-particulas_c[0][i], particulas_c[9][i]-particulas_c[1][i]);
         //printf("i=%i xOF %f yOF %f hx %f hy %f\n", i, particulas_c[0][i], yc, particulas_c[2][i], Hyc);
     }


}


Vx=Estado[7][1];
Vy=Estado[8][1];


if (Estado[6][1]==0){
   for (int i=0;i<Nh/2;i++)
    {   float c1=0,c2=0;
        //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;
        cvRand( &V, x_k );
        cvRand( &ran, C_k );
        xc=particulas_h[0][i] + Vx+(CV_MAT_ELEM( *x_k, float, 0, 0 ));
        yc=particulas_h[1][i] + Vy+(CV_MAT_ELEM( *x_k, float, 1, 0 ));
        c1=CV_MAT_ELEM( *C_k, float, 0, 0 );
        c2=CV_MAT_ELEM( *C_k, float, 1, 0 );
        Hxc = particulas_h[2][i] +c1;
        Hyc = particulas_h[3][i] +c2;
        //printf("varHxc=%f varHyc=%f i=%i\n",c1,c2,i);
        //printf("i=%i xOFa %f yOFa %f hxa %f hya %f\n", i, particulas_h[0][i], yc, particulas_h[2][i], Hyc);
        particula::control(M,limx, limy);
        particulas_h[0][i]=xc;
        particulas_h[1][i]=yc;
        particulas_h[2][i]=Hxc;
        particulas_h[3][i]=Hyc;

        //printf("i=%i vx=%f, vy=%f\n",i, particulas_h[8][i]-particulas_h[0][i], particulas_h[9][i]-particulas_h[1][i]);
        //printf("i=%i xOF %f yOF %f hx %f hy %f\n", i, particulas_h[0][i], yc, particulas_h[2][i], Hyc);
    }


    for (int i=Nh/2;i<Nh;i++)
    {   //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;
        cvRand( &rng, x_k );
        cvRand( &ran, C_k );
        xc=particulas_h[0][i] + CV_MAT_ELEM( *x_k, float, 0, 0 );
        yc=particulas_h[1][i] + CV_MAT_ELEM( *x_k, float, 1, 0 );

        Hxc = particulas_h[2][i] + CV_MAT_ELEM( *C_k, float, 0, 0 );
        Hyc = particulas_h[3][i] + CV_MAT_ELEM( *C_k, float, 1, 0 );
        //printf("ia=%i xOF2a %f yOF2a %i hxa %f hya %i\n", i, particulas_h[0][i], yc, particulas_h[2][i], Hyc);
        particula::control(M,limx, limy);
        particulas_h[0][i]=xc;
        particulas_h[1][i]=yc;
        particulas_h[2][i]=Hxc;
        particulas_h[3][i]=Hyc;
        //printf("i=%i xOF2 %f yOF2 %f hx %f hy %f\n", i, particulas_h[0][i], yc, particulas_h[2][i], Hyc);
    }
}
else{
    for (int i=0;i<Nh/2;i++)
     {   float c1=0,c2=0;
         //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;
         cvRand( &V, x_k );
         cvRand( &ran, C_k );
         xc=particulas_h[0][i] + 2*Vx+(CV_MAT_ELEM( *x_k, float, 0, 0 ));
         yc=particulas_h[1][i] + 2*Vy+(CV_MAT_ELEM( *x_k, float, 1, 0 ));
         c1=CV_MAT_ELEM( *C_k, float, 0, 0 );
         c2=CV_MAT_ELEM( *C_k, float, 1, 0 );
         Hxc = particulas_h[2][i] +c1;
         Hyc = particulas_h[3][i] +c2;
         //printf("varHxc=%f varHyc=%f i=%i\n",c1,c2,i);
         //printf("i=%i xOFa %f yOFa %f hxa %f hya %f\n", i, particulas_h[0][i], yc, particulas_h[2][i], Hyc);
         particula::control(M,limx, limy);
         particulas_h[0][i]=xc;
         particulas_h[1][i]=yc;
         particulas_h[2][i]=Hxc;
         particulas_h[3][i]=Hyc;

         //printf("i=%i vx=%f, vy=%f\n",i, particulas_h[8][i]-particulas_h[0][i], particulas_h[9][i]-particulas_h[1][i]);
         //printf("i=%i xOF %f yOF %f hx %f hy %f\n", i, particulas_h[0][i], yc, particulas_h[2][i], Hyc);
     }


     for (int i=Nh/2;i<Nh;i++)
     {   //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;
         cvRand( &rng, x_k );
         cvRand( &ran, C_k );
         xc=particulas_h[0][i] + 2*Vx+CV_MAT_ELEM( *x_k, float, 0, 0 );
         yc=particulas_h[1][i] + 2*Vy+CV_MAT_ELEM( *x_k, float, 1, 0 );

         Hxc = particulas_h[2][i] + CV_MAT_ELEM( *C_k, float, 0, 0 );
         Hyc = particulas_h[3][i] + CV_MAT_ELEM( *C_k, float, 1, 0 );
         //printf("ia=%i xOF2a %f yOF2a %i hxa %f hya %i\n", i, particulas_h[0][i], yc, particulas_h[2][i], Hyc);
         particula::control(M,limx, limy);
         particulas_h[0][i]=xc;
         particulas_h[1][i]=yc;
         particulas_h[2][i]=Hxc;
         particulas_h[3][i]=Hyc;
         //printf("i=%i xOF2 %f yOF2 %f hx %f hy %f\n", i, particulas_h[0][i], yc, particulas_h[2][i], Hyc);
     }

}

Vx=Estado[7][2];
Vy=Estado[8][2];

if (Estado[6][2]==0)
{
    for (int i=0;i<Nl/2;i++)
    {   //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;
        cvRand( &rng, x_k );
        cvRand( &ran, C_k );
        xc=particulas_l[0][i] +Vx+ CV_MAT_ELEM( *x_k, float, 0, 0 );
        yc=particulas_l[1][i] +Vy+ CV_MAT_ELEM( *x_k, float, 1, 0 );

        Hxc = particulas_l[2][i] + CV_MAT_ELEM( *C_k, float, 0, 0 );
        Hyc = particulas_l[3][i] + CV_MAT_ELEM( *C_k, float, 1, 0 );
        //printf("ia=%i xOF2a %f yOF2a %i hxa %f hya %i\n", i, particulas_l[0][i], yc, particulas_l[2][i], Hyc);
        particula::control(M,limx, limy);
        particulas_l[0][i]=xc;
        particulas_l[1][i]=yc;
        particulas_l[2][i]=Hxc;
        particulas_l[3][i]=Hyc;
        //printf("i=%i xOF2 %f yOF2 %f hx %f hy %f\n", i, particulas_l[0][i], yc, particulas_l[2][i], Hyc);
    }
    for (int i=Nl/2;i<Nl;i++)
    {   //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;
        cvRand( &rng, x_k );
        cvRand( &ran, C_k );
        xc=particulas_l[0][i] + CV_MAT_ELEM( *x_k, float, 0, 0 );
        yc=particulas_l[1][i] + CV_MAT_ELEM( *x_k, float, 1, 0 );

        Hxc = particulas_l[2][i] + CV_MAT_ELEM( *C_k, float, 0, 0 );
        Hyc = particulas_l[3][i] + CV_MAT_ELEM( *C_k, float, 1, 0 );
        //printf("ia=%i xOF2a %f yOF2a %i hxa %f hya %i\n", i, particulas_l[0][i], yc, particulas_l[2][i], Hyc);
        particula::control(M,limx, limy);
        particulas_l[0][i]=xc;
        particulas_l[1][i]=yc;
        particulas_l[2][i]=Hxc;
        particulas_l[3][i]=Hyc;
        //printf("i=%i xOF2 %f yOF2 %f hx %f hy %f\n", i, particulas_l[0][i], yc, particulas_l[2][i], Hyc);
    }
}
else
{
     for (int i=0;i<Nl/2;i++)
     {   //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;
         cvRand( &rng, x_k );
         cvRand( &ran, C_k );
         xc=particulas_l[0][i] +2*Vx+ CV_MAT_ELEM( *x_k, float, 0, 0 );
         yc=particulas_l[1][i] +2*Vx+ CV_MAT_ELEM( *x_k, float, 1, 0 );

         Hxc = particulas_l[2][i] + CV_MAT_ELEM( *C_k, float, 0, 0 );
         Hyc = particulas_l[3][i] + CV_MAT_ELEM( *C_k, float, 1, 0 );
         //printf("ia=%i xOF2a %f yOF2a %i hxa %f hya %i\n", i, particulas_l[0][i], yc, particulas_l[2][i], Hyc);
         particula::control(M,limx, limy);
         particulas_l[0][i]=xc;
         particulas_l[1][i]=yc;
         particulas_l[2][i]=Hxc;
         particulas_l[3][i]=Hyc;
         //printf("i=%i xOF2 %f yOF2 %f hx %f hy %f\n", i, particulas_l[0][i], yc, particulas_l[2][i], Hyc);
     }
     for (int i=Nl/2;i<Nl;i++)
     {   //xo=xo1; yo=yo1; Hxco=anc; Hyco=alt;
         cvRand( &rng, x_k );
         cvRand( &ran, C_k );
         xc=particulas_l[0][i] +2*Vx+ CV_MAT_ELEM( *x_k, float, 0, 0 );
         yc=particulas_l[1][i] +2*Vx+ CV_MAT_ELEM( *x_k, float, 1, 0 );

         Hxc = particulas_l[2][i] + CV_MAT_ELEM( *C_k, float, 0, 0 );
         Hyc = particulas_l[3][i] + CV_MAT_ELEM( *C_k, float, 1, 0 );
         //printf("ia=%i xOF2a %f yOF2a %i hxa %f hya %i\n", i, particulas_l[0][i], yc, particulas_l[2][i], Hyc);
         particula::control(M,limx, limy);
         particulas_l[0][i]=xc;
         particulas_l[1][i]=yc;
         particulas_l[2][i]=Hxc;
         particulas_l[3][i]=Hyc;
         //printf("i=%i xOF2 %f yOF2 %f hx %f hy %f\n", i, particulas_l[0][i], yc, particulas_l[2][i], Hyc);
     }


    //Vx=2*sumx/numero;
    //Vy=2*sumy/numero;
    //printf("vxO=%f, vyO=%f\n",Vx, Vy);
}

}


void particula::control(int M, int limx, int limy)
{
    for (int i=0; i<2; i++)
    {
       if((xc+Hxc)>limx){
        if(xc>limx){
        xc=abs(xc)-limx;
        Hxc=limx-xc;
        }
        else{
        xc=xc;
        Hxc=limx-xc;
        }
     }


    if((yc+Hyc)>limy){
        if(yc>limy){
        yc=abs(yc)-limy;
        Hyc=limy-yc;
        }
        else{
        yc=yc;
        Hyc=limy-yc;
        }
     }

     if((xc)<M)
     {
        xc=M+abs(xc);
        Hxc=abs(xc);
     }

     if((yc)<M)
     {
        yc=M+abs(yc);
        Hyc=abs(yc);
     }

    if (Hxc<5){
        Hxc=5;}
    if(Hyc<5){
        Hyc=5;}
    }
//printf("ycont=%i Hycont=%i\n",yc, Hyc);
//printf("xcont=%i Hxcont=%i\n",xc, Hxc);

}

void particula::orden(float Estado[12][3]){


    int indiceMenor, i, j;
    int Nc,Nh, Nl;
    double aux0, aux1, aux2, aux3, aux4, aux5, aux6, aux7, aux8, aux9, aux10, aux11, aux12;
    Nc=Estado[11][0];
    Nh=Estado[11][1];
    Nl=Estado[11][2];



//*********************************************************************************
//Organiza color
    if(((Neff_c<Neff_h)&&(Neff_c<Neff_l))||((Neff_c>Neff_h)&&(Neff_c>Neff_l))){
printf("organiza cor\n");
/*    for (j = 0; j < Nc; j++){
     printf("pesos %f X%f y%f sem ordem\n",particulas_c[12][j],particulas_c[0][j],particulas_c[1][j]);
       }*/
    /* ordenar a[0]..a[n-2] y a[n-1] en cada pasada */
    for (i = 0; i < Nc-1; i++)
    {
    /* comienzo de la exploración en índice i */
    indiceMenor = i;
    /* j explora la sublista a[i+1]..a[n-1] */
    for (j = i+1; j < Nc; j++)
    if (particulas_c[12][j] > particulas_c[12][indiceMenor])
    {indiceMenor = j;}
    /* sitúa el elemento más pequeño en a[i] */
    //printf("i%i, indice menor%i\n",i,indiceMenor);
    if (i != indiceMenor)
    {
    aux0 = particulas_c[0][i];
    aux1 = particulas_c[1][i];
    aux2 = particulas_c[2][i];
    aux3 = particulas_c[3][i];
    aux4 = particulas_c[4][i];
    aux5 = particulas_c[5][i];
    aux6 = particulas_c[6][i];
    aux7 = particulas_c[7][i];
    aux8 = particulas_c[8][i];
    aux9 = particulas_c[9][i];
    aux10 = particulas_c[10][i];
    aux11 = particulas_c[11][i];
    aux12 = particulas_c[12][i];


    particulas_c[0][i] = particulas_c[0][indiceMenor];
    particulas_c[1][i] = particulas_c[1][indiceMenor];
    particulas_c[2][i] = particulas_c[2][indiceMenor];
    particulas_c[3][i] = particulas_c[3][indiceMenor];
    particulas_c[4][i] = particulas_c[4][indiceMenor];
    particulas_c[5][i] = particulas_c[5][indiceMenor];
    particulas_c[6][i] = particulas_c[6][indiceMenor];
    particulas_c[7][i] = particulas_c[7][indiceMenor];
    particulas_c[8][i] = particulas_c[8][indiceMenor];
    particulas_c[9][i] = particulas_c[9][indiceMenor];
    particulas_c[10][i] = particulas_c[10][indiceMenor];
    particulas_c[11][i] = particulas_c[11][indiceMenor];
    particulas_c[12][i] = particulas_c[12][indiceMenor];


    particulas_c[0][indiceMenor] = aux0 ;
    particulas_c[1][indiceMenor] = aux1 ;
    particulas_c[2][indiceMenor] = aux2 ;
    particulas_c[3][indiceMenor] = aux3 ;
    particulas_c[4][indiceMenor] = aux4 ;
    particulas_c[5][indiceMenor] = aux5 ;
    particulas_c[6][indiceMenor] = aux6 ;
    particulas_c[7][indiceMenor] = aux7 ;
    particulas_c[8][indiceMenor] = aux8 ;
    particulas_c[9][indiceMenor] = aux9 ;
    particulas_c[10][indiceMenor] = aux10;
    particulas_c[11][indiceMenor] = aux11;
    particulas_c[12][indiceMenor] = aux12;

    }

    }
/*    for (j = 0; j < Nc; j++){
        printf("pesos %f X%f y%f\n",particulas_c[12][j],particulas_c[0][j],particulas_c[1][j]);

    }*/
    }


    //*********************************************************************************
    //Organiza hough
        if(((Neff_h<Neff_c)&&(Neff_h<Neff_l))||((Neff_h>Neff_c)&&(Neff_h>Neff_l))){
            printf("organiza hough\n");
    /*    for (j = 0; j < Nc; j++){
         printf("pesos %f X%f y%f sem ordem\n",particulas_h[12][j],particulas_h[0][j],particulas_h[1][j]);
           }*/
        /* ordenar a[0]..a[n-2] y a[n-1] en cada pasada */
        for (i = 0; i < Nh-1; i++)
        {
        /* comienzo de la exploración en índice i */
        indiceMenor = i;
        /* j explora la sublista a[i+1]..a[n-1] */
        for (j = i+1; j < Nh; j++)
        if (particulas_h[12][j] > particulas_h[12][indiceMenor])
        {indiceMenor = j;}
        /* sitúa el elemento más pequeño en a[i] */
        //printf("i%i, indice menor%i\n",i,indiceMenor);
        if (i != indiceMenor)
        {
        aux0 = particulas_h[0][i];
        aux1 = particulas_h[1][i];
        aux2 = particulas_h[2][i];
        aux3 = particulas_h[3][i];
        aux4 = particulas_h[4][i];
        aux5 = particulas_h[5][i];
        aux6 = particulas_h[6][i];
        aux7 = particulas_h[7][i];
        aux8 = particulas_h[8][i];
        aux9 = particulas_h[9][i];
        aux10 = particulas_h[10][i];
        aux11 = particulas_h[11][i];
        aux12 = particulas_h[12][i];


        particulas_h[0][i] = particulas_h[0][indiceMenor];
        particulas_h[1][i] = particulas_h[1][indiceMenor];
        particulas_h[2][i] = particulas_h[2][indiceMenor];
        particulas_h[3][i] = particulas_h[3][indiceMenor];
        particulas_h[4][i] = particulas_h[4][indiceMenor];
        particulas_h[5][i] = particulas_h[5][indiceMenor];
        particulas_h[6][i] = particulas_h[6][indiceMenor];
        particulas_h[7][i] = particulas_h[7][indiceMenor];
        particulas_h[8][i] = particulas_h[8][indiceMenor];
        particulas_h[9][i] = particulas_h[9][indiceMenor];
        particulas_h[10][i] = particulas_h[10][indiceMenor];
        particulas_h[11][i] = particulas_h[11][indiceMenor];
        particulas_h[12][i] = particulas_h[12][indiceMenor];


        particulas_h[0][indiceMenor] = aux0 ;
        particulas_h[1][indiceMenor] = aux1 ;
        particulas_h[2][indiceMenor] = aux2 ;
        particulas_h[3][indiceMenor] = aux3 ;
        particulas_h[4][indiceMenor] = aux4 ;
        particulas_h[5][indiceMenor] = aux5 ;
        particulas_h[6][indiceMenor] = aux6 ;
        particulas_h[7][indiceMenor] = aux7 ;
        particulas_h[8][indiceMenor] = aux8 ;
        particulas_h[9][indiceMenor] = aux9 ;
        particulas_h[10][indiceMenor] = aux10;
        particulas_h[11][indiceMenor] = aux11;
        particulas_h[12][indiceMenor] = aux12;

        }

        }
    /*    for (j = 0; j < Nc; j++){
            printf("pesos %f X%f y%f\n",particulas_h[12][j],particulas_h[0][j],particulas_h[1][j]);
    particulas_c[0][i] = particulas_c[0][indiceMenor];
    particulas_c[1][i] = particulas_c[1][indiceMenor];
    particulas_c[2][i] = particulas_c[2][indiceMenor];
    particulas_c[3][i] = particulas_c[3][indiceMenor];
    particulas_c[4][i] = particulas_c[4][indiceMenor];
    particulas_c[5][i] = particulas_c[5][indiceMenor];
    particulas_c[6][i] = particulas_c[6][indiceMenor];
    particulas_c[7][i] = particulas_c[7][indiceMenor];
    particulas_c[8][i] = particulas_c[8][indiceMenor];
    particulas_c[9][i] = particulas_c[9][indiceMenor];
    particulas_c[10][i] = particulas_c[10][indiceMenor];
    particulas_c[11][i] = particulas_c[11][indiceMenor];
    particulas_c[12][i] = particulas_c[12][indiceMenor];
        }*/
        }


//**************************************************************** particulas_c[0][i] = particulas_c[0][indiceMenor];
        particulas_c[1][i] = particulas_c[1][indiceMenor];
        particulas_c[2][i] = particulas_c[2][indiceMenor];
        particulas_c[3][i] = particulas_c[3][indiceMenor];
        particulas_c[4][i] = particulas_c[4][indiceMenor];
        particulas_c[5][i] = particulas_c[5][indiceMenor];
        particulas_c[6][i] = particulas_c[6][indiceMenor];
        particulas_c[7][i] = particulas_c[7][indiceMenor];
        particulas_c[8][i] = particulas_c[8][indiceMenor];
        particulas_c[9][i] = particulas_c[9][indiceMenor];
        particulas_c[10][i] = particulas_c[10][indiceMenor];
        particulas_c[11][i] = particulas_c[11][indiceMenor];
        particulas_c[12][i] = particulas_c[12][indiceMenor];
        //Organiza textura
            if(((Neff_l<Neff_c)&&(Neff_l<Neff_h))||((Neff_l>Neff_c)&&(Neff_l>Neff_h))){
                printf("organiza textura\n");
        /*    for (j = 0; j < Nc; j++){
             printf("pesos %f X%f y%f sem ordem\n",particulas_l[12][j],particulas_l[0][j],particulas_l[1][j]);
               }*/
            /* ordenar a[0]..a[n-2] y a[n-1] en cada pasada */
            for (i = 0; i < Nl-1; i++)
            {
            /* comienzo de la exploración en índice i */
            indiceMenor = i;
            /* j explora la sublista a[i+1]..a[n-1] */
            for (j = i+1; j < Nl; j++)
            if (particulas_l[12][j] > particulas_l[12][indiceMenor])
            {indiceMenor = j;}
            /* sitúa el elemento más pequeño en a[i] */
            //printf("i%i, indice menor%i\n",i,indiceMenor);
            if (i != indiceMenor)
            {
            aux0 = particulas_l[0][i];
            aux1 = particulas_l[1][i];
            aux2 = particulas_l[2][i];
            aux3 = particulas_l[3][i];
            aux4 = particulas_l[4][i];
            aux5 = particulas_l[5][i];
            aux6 = particulas_l[6][i];
            aux7 = particulas_l[7][i];
            aux8 = particulas_l[8][i];
            aux9 = particulas_l[9][i];
            aux10 = particulas_l[10][i];
            aux11 = particulas_l[11][i];
            aux12 = particulas_l[12][i];


            particulas_l[0][i] = particulas_l[0][indiceMenor];
            particulas_l[1][i] = particulas_l[1][indiceMenor];
            particulas_l[2][i] = particulas_l[2][indiceMenor];
            particulas_l[3][i] = particulas_l[3][indiceMenor];
            particulas_l[4][i] = particulas_l[4][indiceMenor];
            particulas_l[5][i] = particulas_l[5][indiceMenor];
            particulas_l[6][i] = particulas_l[6][indiceMenor];
            particulas_l[7][i] = particulas_l[7][indiceMenor];
            particulas_l[8][i] = particulas_l[8][indiceMenor];
            particulas_l[9][i] = particulas_l[9][indiceMenor];
            particulas_l[10][i] = particulas_l[10][indiceMenor];
            particulas_l[11][i] = particulas_l[11][indiceMenor];
            particulas_l[12][i] = particulas_l[12][indiceMenor];


            particulas_l[0][indiceMenor] = aux0 ;
            particulas_l[1][indiceMenor] = aux1 ;
            particulas_l[2][indiceMenor] = aux2 ;
            particulas_l[3][indiceMenor] = aux3 ;
            particulas_l[4][indiceMenor] = aux4 ;
            particulas_l[5][indiceMenor] = aux5 ;
            particulas_l[6][indiceMenor] = aux6 ;
            particulas_l[7][indiceMenor] = aux7 ;
            particulas_l[8][indiceMenor] = aux8 ;
            particulas_l[9][indiceMenor] = aux9 ;
            particulas_l[10][indiceMenor] = aux10;
            particulas_l[11][indiceMenor] = aux11;
            particulas_l[12][indiceMenor] = aux12;

            }particulas_c[0][i] = particulas_c[0][indiceMenor];
            particulas_c[1][i] = particulas_c[1][indiceMenor];
            particulas_c[2][i] = particulas_c[2][indiceMenor];
            particulas_c[3][i] = particulas_c[3][indiceMenor];
            particulas_c[4][i] = particulas_c[4][indiceMenor];
            particulas_c[5][i] = particulas_c[5][indiceMenor];
            particulas_c[6][i] = particulas_c[6][indiceMenor];
            particulas_c[7][i] = particulas_c[7][indiceMenor];
            particulas_c[8][i] = particulas_c[8][indiceMenor];
            particulas_c[9][i] = particulas_c[9][indiceMenor];
            particulas_c[10][i] = particulas_c[10][indiceMenor];
            particulas_c[11][i] = particulas_c[11][indiceMenor];
            particulas_c[12][i] = particulas_c[12][indiceMenor];


            }
        /*    for (j = 0; j < Nc; j++){
                printf("pesos %f X%f y%f\n",particulas_l[12][j],particulas_l[0][j],particulas_l[1][j]);

            }*/
            }

}


void particula::cambio(float Estado[12][3]){
    int Nc,Nh, Nl;
    Nc=Estado[11][0];
    Nh=Estado[11][1];
    Nl=Estado[11][2];
    int diferencia;
    int i;
    int PNeff_c,PNeff_h,PNeff_l;
    int por=100;
printf("Neff_c=%i Neff_h=%i Neff_l=%i \n",Neff_c,Neff_h,Neff_l);
    PNeff_c=(Neff_c/Nc)*por;
    PNeff_h=(Neff_h/Nh)*por;
    PNeff_l=(Neff_l/Nl)*por;
printf("PNc=%i PNh=%i PNl=%i\n",PNeff_c,PNeff_h,PNeff_l);

if((PNeff_c>PNeff_h)&&(PNeff_c>PNeff_l)){
    if((PNeff_l>PNeff_h)) {
       diferencia=Nh-Neff_h;

       for(i=Nc;i<Nc+(diferencia/2);i++){
           particulas_c[0][i] = particulas_c[0][0];
           particulas_c[1][i] = particulas_c[1][0];
           particulas_c[2][i] = particulas_c[2][0];
           particulas_c[3][i] = particulas_c[3][0];
           particulas_c[4][i] = particulas_c[4][0];
           particulas_c[5][i] = particulas_c[5][0];
           particulas_c[6][i] = particulas_c[6][0];
           particulas_c[7][i] = particulas_c[7][0];
           particulas_c[8][i] = particulas_c[8][0];
           particulas_c[9][i] = particulas_c[9][0];
           particulas_c[10][i] = particulas_c[10][0];
           particulas_c[11][i] = particulas_c[11][0];
           particulas_c[12][i] = particulas_c[12][0];
       Estado[11][0]=Nc+(diferencia/2);
       Estado[11][1]=Nh-(diferencia/2);

       }


        }
    else{
       diferencia=Nl-Neff_l;
       for(i=Nc;i<Nc+(diferencia/2);i++){
           particulas_c[0][i] = particulas_c[0][0];
           particulas_c[1][i] = particulas_c[1][0];
           particulas_c[2][i] = particulas_c[2][0];
           particulas_c[3][i] = particulas_c[3][0];
           particulas_c[4][i] = particulas_c[4][0];
           particulas_c[5][i] = particulas_c[5][0];
           particulas_c[6][i] = particulas_c[6][0];
           particulas_c[7][i] = particulas_c[7][0];
           particulas_c[8][i] = particulas_c[8][0];
           particulas_c[9][i] = particulas_c[9][0];
           particulas_c[10][i] = particulas_c[10][0];
           particulas_c[11][i] = particulas_c[11][0];
           particulas_c[12][i] = particulas_c[12][0];
       Estado[11][0]=Nc+(diferencia/2);
       Estado[11][2]=Nl-(diferencia/2);
    }

}
    }

if((PNeff_h>PNeff_c)&&(PNeff_h>PNeff_l)){
    if((PNeff_l>PNeff_c)) {
       diferencia=Nc-Neff_c;
       for(i=Nh;i<Nh+(diferencia/2);i++){
           particulas_h[0][i] = particulas_h[0][0];
           particulas_h[1][i] = particulas_h[1][0];
           particulas_h[2][i] = particulas_h[2][0];
           particulas_h[3][i] = particulas_h[3][0];
           particulas_h[4][i] = particulas_h[4][0];
           particulas_h[5][i] = particulas_h[5][0];
           particulas_h[6][i] = particulas_h[6][0];
           particulas_h[7][i] = particulas_h[7][0];
           particulas_h[8][i] = particulas_h[8][0];
           particulas_h[9][i] = particulas_h[9][0];
           particulas_h[10][i] = particulas_h[10][0];
           particulas_h[11][i] = particulas_h[11][0];
           particulas_h[12][i] = particulas_h[12][0];
       Estado[11][1]=Nh+(diferencia/2);
       Estado[11][0]=Nc-(diferencia/2);

        }

        }
    else{

       diferencia=Nl-Neff_l;
       for(i=Nh;i<Nh+(diferencia/2);i++){
           particulas_h[0][i] = particulas_h[0][0];
           particulas_h[1][i] = particulas_h[1][0];
           particulas_h[2][i] = particulas_h[2][0];
           particulas_h[3][i] = particulas_h[3][0];
           particulas_h[4][i] = particulas_h[4][0];
           particulas_h[5][i] = particulas_h[5][0];
           particulas_h[6][i] = particulas_h[6][0];
           particulas_h[7][i] = particulas_h[7][0];
           particulas_h[8][i] = particulas_h[8][0];
           particulas_h[9][i] = particulas_h[9][0];
           particulas_h[10][i] = particulas_h[10][0];
           particulas_h[11][i] = particulas_h[11][0];
           particulas_h[12][i] = particulas_h[12][0];
       Estado[11][1]=Nh+(diferencia/2);
       Estado[11][2]=Nl-(diferencia/2);
    }

}
}
if((PNeff_l>PNeff_c)&&(PNeff_l>PNeff_h)){
    if((PNeff_h>PNeff_c)) {
       diferencia=Nc-Neff_c;
       for(i=Nl;i<Nl+(diferencia/2);i++){
           particulas_l[0][i] = particulas_l[0][0];
           particulas_l[1][i] = particulas_l[1][0];
           particulas_l[2][i] = particulas_l[2][0];
           particulas_l[3][i] = particulas_l[3][0];
           particulas_l[4][i] = particulas_l[4][0];
           particulas_l[5][i] = particulas_l[5][0];
           particulas_l[6][i] = particulas_l[6][0];
           particulas_l[7][i] = particulas_l[7][0];
           particulas_l[8][i] = particulas_l[8][0];
           particulas_l[9][i] = particulas_l[9][0];
           particulas_l[10][i] = particulas_l[10][0];
           particulas_l[11][i] = particulas_l[11][0];
           particulas_l[12][i] = particulas_l[12][0];
       Estado[11][2]=Nl+(diferencia/2);
       Estado[11][0]=Nc-(diferencia/2);
        }
        }
    else{
       diferencia=Nh-Neff_h;
       for(i=Nl;i<Nl+(diferencia/2);i++){
           particulas_l[0][i] = particulas_l[0][0];
           particulas_l[1][i] = particulas_l[1][0];
           particulas_l[2][i] = particulas_l[2][0];
           particulas_l[3][i] = particulas_l[3][0];
           particulas_l[4][i] = particulas_l[4][0];
           particulas_l[5][i] = particulas_l[5][0];
           particulas_l[6][i] = particulas_l[6][0];
           particulas_l[7][i] = particulas_l[7][0];
           particulas_l[8][i] = particulas_l[8][0];
           particulas_l[9][i] = particulas_l[9][0];
           particulas_l[10][i] = particulas_l[10][0];
           particulas_l[11][i] = particulas_l[11][0];
           particulas_l[12][i] = particulas_l[12][0];
       Estado[11][2]=Nl+(diferencia/2);
       Estado[11][1]=Nh-(diferencia/2);
    }


}
}
printf("diferencia %i\n",diferencia);

printf("Nc=%f Nh=%f Nl=%f\n",Estado[11][0],Estado[11][1],Estado[11][2]);
}










particula::~particula()
{
    //dtor
}
