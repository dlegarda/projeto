#include "LBP.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>
using namespace std;
using namespace cv;

LBP::LBP()
{
    //ctor
}

LBP::~LBP()
{
    //dtor
}

void LBP::Calculo_HistogramaLBP(Mat image, int Hx, int Hy){
 Mat src(image),dst;

    //cvResetImageROI(image);

 cvtColor(src, dst, CV_BGR2GRAY );
 int s,s1;
 int fator=0;


 for (int i=1;i<(Hx-1);i++){
    for(int j=1;j<(Hy-1);j++){
        int cont=0;
        int suma=0;
        uchar* data=dst.ptr<uchar>(j);
        s=data[i];
             //s = cvGet2D(&dst, j, i);;
            for(int n=-1;n<2;n++){
                for (int m=-1;m<2;m++){
                uchar* data2=dst.ptr<uchar>(m);
                s1=data2[n];

                //s1 = cvGet2D(&dst, j, i);
                if((s-s1)<0){
                fator=0;
                }
                else{
                fator=1;
                }
                //suma=suma+fator*(2^cont);
                cluster[cont]=fator;
                cont=cont+1;
                }
            }
            for(int h=0;h<8;h++){
            if(cluster[h]!=cluster[h+1]){
            suma=suma+1;
            }
            }

    //printf("valor do pixel %i comparação %i, suma %i \n",s,s1,suma);
    //aqui estamos fazendo o Histograma
    switch(suma) {
    case 0:
    HistogramaLPB[0]=HistogramaLPB[0]+1;
    break;
    case 1:
    HistogramaLPB[1]=HistogramaLPB[1]+1;
    break;
    case 2:
    HistogramaLPB[2]=HistogramaLPB[2]+1;
    break;
    case 3:
    HistogramaLPB[3]=HistogramaLPB[3]+1;
    break;
    case 4:
    HistogramaLPB[4]=HistogramaLPB[4]+1;
    break;
    case 5:
    HistogramaLPB[5]=HistogramaLPB[5]+1;
    break;
    case 6:
    HistogramaLPB[6]=HistogramaLPB[6]+1;
    break;
//    case 128:
//    HistogramaLPB[7]=HistogramaLPB[7]+1;
//    break;
//    case 10:
//    HistogramaLPB[8]=HistogramaLPB[8]+1;
//    break;
//    case 38:
//    HistogramaLPB[9]=HistogramaLPB[9]+1;
//    break;
//    case 9:
//    HistogramaLPB[10]=HistogramaLPB[10]+1;
//    break;
//    case 17:
//    HistogramaLPB[11]=HistogramaLPB[11]+1;
//    break;
//    case 33:
//    HistogramaLPB[12]=HistogramaLPB[12]+1;
//    break;
//    case 65:
//    HistogramaLPB[13]=HistogramaLPB[13]+1;
//    break;
//    case 129:
//    HistogramaLPB[14]=HistogramaLPB[14]+1;
//    break;
//    case 150:
//    HistogramaLPB[15]=HistogramaLPB[15]+1;
//    break;
    default:
    HistogramaLPB[7]=HistogramaLPB[16]+1;
    }

    }


 }
float suma4=0;

 for (int i=0; i<8;i++){
 HistogramaLPB[i]=HistogramaLPB[i]/((Hx-2)*(Hy-2));
 suma4=HistogramaLPB[i]+suma4;
 //printf("valor del bin  %i %f soma %f \n", i, HistogramaLPB[i],suma4);
 }

//  namedWindow( "ventana", CV_WINDOW_AUTOSIZE );
// namedWindow( "Gray image", CV_WINDOW_NORMAL );
// namedWindow( "original", CV_WINDOW_NORMAL );
//
// cvShowImage( "original", image );
//
// imshow( "ventana", src );
// imshow( "Gray image", dst );


 //calcHist(xo1, yo1, anc, alt);



 //waitKey();



}
