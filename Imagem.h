#ifndef IMAGEM_H
#define IMAGEM_H
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <particula.h>
#include "LBP.h"
using namespace cv;

class Imagem
{
    float x, y, Hx, Hy, xo, yo, altoo, anchoo;
    int in;

    //int hbins = 30, sbins = 32;
    //int histSize[] = {hbins, sbins};
    MatND histo;
    MatND histo2;
    LBP lbp;
    Mat img;
    CvCapture *capt;
    IplImage* h_plane;
    IplImage* s_plane;
    IplImage* v_plane;
    Mat hsv;
    IplImage* hist_img;
    IplImage* hist_img2;
    IplImage* hist_img3;
    IplImage* hist_img4;
    CvHistogram* histHS;
    CvHistogram* histHSA;
    float bins[10][10];
    float bins2[10][10];
    float bins3[10][10];
    float bins4[10][10];
    float bins5[10][10];
    int    channels[2];
    int scale;
    int hbins ;
    int sbins ;
    float intensity;
    int an, al;
    float wimax;
    int num;
    double we;
    public:
        float Fc, Fh, Fl;
        Mat src, dst, cdst;
        int M, limx, limy;
        float Estado[12][3];
        int xm,ym, Hxm, Hym;
        float suma;
        float Vx, Vy;
        float HistLBP_Alvo[8];
        float HistLBP_MP[8];
        float HistLBP_Part[8];
        float hist[5]={0};
        float histR[5]={0};
        float Part_hist[5],Part_histR[5];
        float Alvo_hist[5],Alvo_histR[5];
        float MP_hist[5],MP_histR[5];
        Mat img2,img3,img4;
        IplImage *image;
        Imagem();
        Imagem(int index);
        void DeterminarROI();
        void correFrames(int num1, int video);
        void CalcularHistograma(int num, double particulas_c[12][2000], double particulas_h[12][2000], double particulas_l[12][2000], bool ocl, int tipos);
        void Calc(int num,double particulas[12][2000],bool ocl);
        void CalculoAlvo(int xo1, int yo1, int anc, int alt,int tipos);
        void LimpiarMemoria();
        void Proyectar();
        void mousecb(int event, int x, int y, int flags, void* param);
        void CalculoMP(int tipos);
        void AlgoritmoMetropolis(double particulas_c[12][2000], double particulas_h[12][2000], double particulas_l[12][2000], bool ocl, int tipos);
        void ActHistograma(float samples, bool ocl,float fator, int tipos);
        void Estimacion(double particulas_c[12][2000], double particulas_h[12][2000], double particulas_l[12][2000]);
        void DisenhoParticulas( double particulas_c[12][2000],double particulas_h[12][2000],double particulas_l[12][2000]);
        void Superpixel(int x, int y, int an, int al);
        void CalculoPesoH(int num,double particulas[12][2000],bool ocl);
        void CalcularHistogramaH(int x, int y, int Hx, int Hy);
        virtual ~Imagem();
    protected:
    private:
};

#endif // IMAGEM_H
