#ifndef PARTICULA_H
#define PARTICULA_H
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

class particula
{
    float xc, yc, Hxc, Hyc;
    float Vx, Vy;
    int Neff;
    int Neff_c,Neff_h,Neff_l;
    CvRandState rng;
    CvRandState rng2;
    CvRandState ran;
    CvRandState V;
    CvRandState U;
    CvMat* C_k;
    CvMat* V_k;
    CvMat* x_k;
    CvMat* U_k;
    int in;
    public:
        float numero;
        bool oclusion;
        double particulas_c[13][2000];
        double particulas_h[13][2000];
        double particulas_l[13][2000];
        particula();
        particula(int x1, int y1, int hx1, int samples);
        void variacion(int xo1, int yo1, int anc, int alt,int M, int limx, int limy);
        void control(int M, int limx, int limy);
        void normaliza(float suma, float Estado[7][3]);
        void resampling(float Estado[7][3]);
        void actualizacion(int M, int limx, int limy,  float Estado[12][3]);
        void Inicia(float sig, float sig1);
        void orden(float Estado[12][3]);
        void cambio(float Estado[12][3]);
        virtual ~particula();
    protected:
    private:
};

#endif // PARTICULA_H

