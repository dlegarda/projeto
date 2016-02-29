#ifndef LBP_H
#define LBP_H
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>


using namespace std;
using namespace cv;

class LBP
{
    public:
        LBP();
        float HistogramaLPB[8]={0};
        virtual ~LBP();
        void Calculo_HistogramaLBP(Mat image,int Hx, int Hy);
    protected:
    private:
    int cluster[8];
};

#endif // LBP_H
