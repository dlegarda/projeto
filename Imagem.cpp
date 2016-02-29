#include "Imagem.h"
#include "slic.h"
#include "LBP.h"
#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#define PI 3.14159265
using namespace cv;
using namespace std;

Imagem::Imagem()
{
    x=y=50;
    Hx=Hy=5;

    //ctor
}
void Imagem::correFrames(int num1, int video)
{       num=num1;
        char medio[11]="";
        char nume[11]="";
        char archivo[50]="";
        char pathfile[10] = "";
        char path1[10] = "img4/";
        char path2[10] = "img3/";
        char path3[10] = "img/";
        char path4[10] = "img2/";
        char path5[10] = "img1/";



        switch(video){
        case 1:
        strcat(pathfile,path1);
        break;
        case 2:
        strcat(pathfile,path2);
        break;
        case 3:
        strcat(pathfile,path3);
        break;
        case 4:
        strcat(pathfile,path4);
        break;
        case 5:
        strcat(pathfile,path5);
        break;


        }



        //char inicio[30]="0000";
        char fin[8]=".jpg";
        if (num<10){
        char medio1[11]="000";
        sprintf(nume,"%d",num);
        strcat(medio,medio1);
        strcat(medio,nume);
        }
        else if (num<100){
        char medio1[11]="00";
        sprintf(nume,"%d",num);
        strcat(medio,medio1);
        strcat(medio,nume);
        }
        else if (num<1000){
        char medio1[11]="0";
        sprintf(nume,"%d",num);
        strcat(medio,medio1);
        strcat(medio,nume);
        }
        else{
        sprintf(medio,"%d",num);
        }
        //sprintf(medio,"%d",num); //nombre de frame corto
        strcat(archivo,pathfile);
        //strcat(archivo,inicio);
        strcat(archivo,medio);
        strcat(archivo,fin);
        //printf(archivo);
        img = imread( archivo, 1 );
        //img = cvLoadImage(archivo);
        img2=img.clone();
        img3=img.clone();

        //hsv= cvCreateImage(cvGetSize(img),8,3);
        //cvtColor( image, gray_image, CV_BGR2GRAY );
        cvtColor( img, hsv, CV_BGR2HSV );
        an=img.cols;
        al=img.rows;
        limx=an-M;
        limy=al-M;
        wimax=0;
}



Imagem::Imagem(int index)
{
    in=index;
    //capt=cvCreateFileCapture("persona.avi");
    //capt=cvCaptureFromCAM(0);
    xm=ym=100;
    Hxm=Hym=5;
    M=6;
    img=cvQueryFrame(capt);
    an=img.cols;
    al=img.rows;
    limx=an-M;
    limy=al-M;
}
void Imagem::DeterminarROI()
{
    scale=30;
    hbins = 10;
    sbins = 10;

}


void Imagem::Superpixel(int x, int y, int an, int al){

////    IplImage *image1 = cvCloneImage(img);
////    int xp, yp, anp, alp;
////    xp=x;
////    yp=y;
////    anp=an;
////    alp=al;
////
////    cvSetImageROI(image1, cvRect(xp, yp, anp, alp));
////    IplImage *lab_image = cvCloneImage(image1);
////    image= cvCloneImage(image1);
////    //CvSize sz=cvGetSize(image);
////    CvRect sz=cvGetImageROI(image1);
////     //cvSetImageROI(image, cvRect(xm,ym,Hxm,Hym));
////    cvCvtColor(image, lab_image, CV_BGR2Lab);
////
////    /* Yield the number of superpixels and weight-factors from the user. */
////    int w = lab_image->width, h = lab_image->height;
////    int nr_superpixels = 400;
////    int nc = 40;
////
////    double step = sqrt((w* h) / (double) nr_superpixels);
////    printf("ancho %i alto %i Hx%i Hy%i %f\n",sz.width,sz.height, lab_image->width, lab_image->height, step);
////
////    /* Perform the SLIC superpixel algorithm. */
////    Slic slic;
////
////    slic.generate_superpixels(lab_image, step, nc);
////    slic.create_connectivity(lab_image);
////
////    /* Display the contours and show the result. */
////    slic.display_contours(image, CV_RGB(255,0,0));
////    slic.display_center_grid(image, CV_RGB(0,255,0));
////    slic.colour_with_cluster_means(image);
////
////    //cvShowImage("result", image);
////    cvWaitKey(0);
}


void Imagem::CalcularHistogramaH(int x, int y, int Hx, int Hy)
{
    double matr[600],matrR[600], paso;
    int xho, yho;

    //Mat src, dst, cdst;
    //printf("Calculando Histograma hough\n");
    //printf("xh=%i yh=%i anh=%i alh=%i\n",x,y,Hx,Hy);
    //printf("ancho=%i alto=%i \n",img.cols,img.rows);

            //src(img3, Rect(x, y, Hx, Hy))
            Mat D (img3, Rect(x, y, Hx, Hy) );

            Canny(D, dst, 50, 200, 3);
            cvtColor(dst, cdst, CV_GRAY2BGR);
            vector<Vec4i> lines;
            HoughLinesP(dst, lines, 1, CV_PI/180, 10, 5, 10 );
//            cvResetImageROI(img3);
//printf("teste\n");
//printf ("lineas %i \n", lines.size());
            for( size_t t = 0; t < lines.size(); t++ )
            {
            Vec4i l = lines[t];
            xho = l[2]-l[0];
            yho = l[3]-l[1];
            matr[t]=atan2 (yho,xho) * 180 / PI;
            matrR[t]=sqrt(xho*xho+yho*yho);
            }

//printf("Paso\n");

             for(int p=0; p<5; p++){
            hist[p]=0;
//            printf("PPP %i\n",p);
              }


        for( size_t t = 0; t < lines.size(); t++ )
            {

            if ((-91<matr[t])&&(matr[t]<=-54)){
                hist[0]=hist[0]+1;
                }
            if ((-54<matr[t])&&(matr[t]<=-18)){
                hist[1]=hist[1]+1;
                }
            if ((-18<matr[t])&&(matr[t]<=18)){
                hist[2]=hist[2]+1;
                }
            if ((18<matr[t])&&(matr[t]<=54)){
                hist[3]=hist[3]+1;
                }
            if ((54<matr[t])&&(matr[t]<91)){
                hist[4]=hist[4]+1;
                }
            }


paso=sqrt(Hx*Hx+Hy*Hy)/5;


for(int i=0; i<5; i++){
    histR[i]=0;
    }


    for( size_t i = 0; i < lines.size(); i++ )
    {
    if ((matrR[i]<=paso)){
        histR[0]=histR[0]+1;
        }
    if ((paso<matrR[i])&&(matrR[i]<=2*paso)){
        histR[1]=histR[1]+1;
        }
    if ((2*paso<matrR[i])&&(matrR[i]<=3*paso)){
        histR[2]=histR[2]+1;
        }
    if ((3*paso<matrR[i])&&(matrR[i]<=4*paso)){
        histR[3]=histR[3]+1;
        }
    if ((4*paso<matrR[i])){
        histR[4]=histR[4]+1;
        }
    }

    for(int i=0; i<5; i++){
         hist[i]=hist[i]/(2*lines.size());
         }
    for(int i=0; i<5; i++){
         histR[i]=histR[i]/(2*lines.size());
         }



}



void Imagem::CalculoPesoH(int num,double particulas[12][2000],bool ocl)
{
    int anc,alt,di;
    double  d, d1, d2, dif, wi;
    float Part_hist[5],Part_histR[5];
    //printf("Calculando peso transformada hough\n");
    suma=0;
    for (int i=0;i<num;i++)
    {
        particulas[8][i]=x=particulas[0][i];
        particulas[9][i]=y=particulas[1][i];
        particulas[10][i]=Hx=particulas[2][i];
        particulas[11][i]=Hy=particulas[3][i];

        CalcularHistogramaH(x, y, Hx, Hy);
         for(int i=0; i<5; i++){
        Part_hist[i]=hist[i];
        Part_histR[i]=histR[i];
        }
         d2=0;
          d1=0;
        for(int p=0;p<5;p++){
            d1=d1+sqrt(Part_hist[p]*Alvo_hist[p]);
            d2=d2+sqrt(Part_histR[p]*Alvo_histR[p]);
            }
        d=(d2+d1);
        if(d<=1){
        dif=sqrt(1-d);
        //printf("dif=%f d=%f\n",dif,d);
        wi=exp(-20*dif*dif);
        }
        else{
        wi=0;
        }
        //printf("dif=%f d1=%f d2=%f wi=%f\n",d,d1,d2,wi);
        wimax=0;
        if ((wi>wimax)&&(ocl==false))
        {
            wimax=wi;
            xm=x;
            ym=y;
            Hxm=Hx;
            Hym=Hy;
        }
        //printf("xm=%i ym=%i anm=%i alm=%i\n",xm,ym,Hxm,Hym);
        suma=suma+wi;
        particulas[5][i]=wi;
       }
}







void Imagem::CalcularHistograma(int num,double particulas_c[12][2000],double particulas_h[12][2000],double particulas_l[12][2000],bool ocl,int tipos)
{   double d, d1,d2, wi, dif=0;
    double Wcor, Wlbp, Whough;
    int Nc,Nh, Nl;
    Nc=Estado[11][0];
    Nh=Estado[11][1];
    Nl=Estado[11][2];
    suma=0;
    Fc=0.4;
    Fh=Fl=0.3;


   Estado[4][0]=0; //Wmaximo
   Estado[5][0]=0; //Soma de pesos
    for (int i=0;i<Nc;i++)
    {
        particulas_c[8][i]=x=particulas_c[0][i];
        particulas_c[9][i]=y=particulas_c[1][i];
        particulas_c[10][i]=Hx=particulas_c[2][i];
        particulas_c[11][i]=Hy=particulas_c[3][i];



//*****************************************************************
//Histograma de cores
   // case 1:

    int    channels[] = {0, 1};
    int histSize[] = {hbins, sbins};
    float hranges[] = { 0, 180 };
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    Mat CHSV (hsv, Rect(x, y, Hx, Hy) );
    calcHist( &CHSV, 1, channels, Mat(), // do not use mask
             histo2, 2, histSize, ranges,
             true, // the histogram is uniform
             false );
             int acum=0;
             for( int h = 0; h < hbins; h++ ){
                for( int s = 0; s < sbins; s++ )
                    {
                    acum = acum+histo2.at<float>(h, s);
                    }
                }
            for( int h = 0; h < hbins; h++ ){
                for( int s = 0; s < sbins; s++ )
                    {
                    bins2[h][s] = histo2.at<float>(h, s)/acum;
                    }
                }


    d=0;
        for(int p=0;p<sbins;p++)
            {
            for(int q=0;q<hbins;q++)
                {
                    d=d+sqrt(bins[p][q]*bins2[p][q]);
                    //printf("bins2=%f,bins5=%f i=%i,j=%i,d=%f\n",bins2[p][q],bins[p][q],p,q,d);
                }
            }

        if(d<1){
        dif=sqrt(1-d);
        //printf("dif=%f d=%f\n",dif,d);
        Wcor=exp(-20*dif*dif);
        }
        else{
        Wcor=0;
        }

        //printf("Wcor= %f \n",Wcor);
    particulas_c[12][i]=Wcor;

    if ((Wcor>Estado[4][0])&&(Estado[6][0]==0))
    {
        Estado[4][0]=Wcor;
        Estado[0][0]=x;
        Estado[1][0]=y;
        Estado[2][0]=Hx;
        Estado[3][0]=Hy;
        //printf("Cor xm=%f ym=%f anm=%f alm=%f\n",x,y,Hx,Hy);
        //printf("Wimax= %f \n",wimax);
    }
    Estado[5][0]=Estado[5][0]+Wcor;

}




    Estado[4][1]=0;
    Estado[5][1]=0;
   for (int i=0;i<Nh;i++)
    {
        particulas_h[8][i]=x=particulas_h[0][i];
        particulas_h[9][i]=y=particulas_h[1][i];
        particulas_h[10][i]=Hx=particulas_h[2][i];
        particulas_h[11][i]=Hy=particulas_h[3][i];

    //*************************************************************
// HIstograma HOUGH
    //case 2:

    CalcularHistogramaH(x, y, Hx, Hy);

        for(int i=0; i<5; i++){
        Part_hist[i]=hist[i];
        Part_histR[i]=histR[i];
        //printf("hist=%f histR=%f\n",Part_hist[i],Part_histR[i]);
        }

           d2=0;
          d1=0;
        for(int p=0;p<5;p++){
            d1=d1+sqrt(Part_hist[p]*Alvo_hist[p]);
            d2=d2+sqrt(Part_histR[p]*Alvo_histR[p]);
            //printf("hist=%f Ahist=%f d1=%f\n",Part_hist[p],Alvo_hist[p],d);
            }
        d=(d2+d1);

        //printf("d1=%f d2=%f d=%f\n",d1,d2,d);
        if(d<=1){
        dif=sqrt(1-d);

        Whough=exp(-20*dif*dif);
        }
        else{
        Whough=0;
        }
         //printf("dif=%f d=%f Whough=%f\n",dif,d,Whough);

    //break;
        //printf("Whough= %f \n",Whough);
   particulas_h[12][i]=Whough;

   if ((Whough>Estado[4][1])&&(Estado[6][0]==0))
   {
       Estado[4][1]=Whough;
       Estado[0][1]=x;
       Estado[1][1]=y;
       Estado[2][1]=Hx;
       Estado[3][1]=Hy;
       //printf("Hough xm=%f ym=%f anm=%f alm=%f\n",x,y,Hx,Hy);
       //printf("Wimax= %f \n",wimax);
   }
   Estado[5][1]=Estado[5][1]+Whough;
    }

//*****************************************************************
    //case 10:
//        //Superpixel(xo1,yo1,anc,alt);
//         if((tipos==10)||(tipos==11)||(tipos==12)){
//        Mat    D (img3, Rect(x, y, Hx, Hy) );
//        lbp.Calculo_HistogramaLBP(D,Hx,Hy);
//        for (int i=0;i<8;i++){
//        HistLBP_Alvo[i]=lbp.HistogramaLPB[i];
//        }
//       // printf("xi=%i yi=%i anc=%i alt=%i\n",x, y, Hx, Hy);
//        }
//       //cvReleaseImage(&hist_img2);
//    //break;





   Estado[4][2]=0;
   Estado[5][2]=0;

       for (int i=0;i<Nl;i++)
    {
        particulas_l[8][i]=x=particulas_l[0][i];
        particulas_l[9][i]=y=particulas_l[1][i];
        particulas_l[10][i]=Hx=particulas_l[2][i];
        particulas_l[11][i]=Hy=particulas_l[3][i];
//*******************************************************************
//HIstograma LBP
    //case 3;

  Mat s (img3, Rect(x, y, Hx, Hy) );
   lbp.Calculo_HistogramaLBP(s,Hx, Hy);
        for (int i=0;i<8;i++){
        HistLBP_Part[i]=lbp.HistogramaLPB[i];

        }

        d=0;



        for(int p=0;p<sbins;p++)
            {
                    d=d+sqrt(HistLBP_Part[p]*HistLBP_Alvo[p]);
                    //printf("bins2=%f,bins5=%f i=%i,j=%i,d=%f\n",bins2[i][j],bins5[i][j],i,j,d);
             }

        if(d<1){
        dif=sqrt(1-d);
       // printf("dif=%f d=%f\n",dif,d);
        Wlbp=exp(-20*dif*dif);
        }
        else{
        Wlbp=0;
        }
//printf("Wlbp= %f \n",Wlbp);

    //break;
    //    }
    particulas_l[12][i]=Wlbp;

    if ((Wlbp>Estado[4][2])&&(Estado[6][0]==0))
    {
        Estado[4][2]=Wlbp;
        Estado[0][2]=x;
        Estado[1][2]=y;
        Estado[2][2]=Hx;
        Estado[3][2]=Hy;
        //printf("LBP xm=%f ym=%f anm=%f alm=%f\n",x,y,Hx,Hy);
        //printf("Wimax= %f \n",wimax);
    }
    Estado[5][2]=Estado[5][2]+Wlbp;

}



}





void Imagem::CalculoAlvo(int xo1, int yo1, int anc, int alt, int tipos)
{
     Mat src, dst, cdst;

   // switch(tipos){

//*****************************************************************
//Histograma de cores
   // case 1:

    int    channels[] = {0, 1};
    int histSize[] = {hbins, sbins};
    float hranges[] = { 0, 180 };
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    Mat CHSV (hsv, Rect(xo1, yo1, anc, alt) );
    calcHist( &CHSV, 1, channels, Mat(), // do not use mask
             histo, 2, histSize, ranges,
             true, // the histogram is uniform
             false );
            int acum=0;
             for( int h = 0; h < hbins; h++ ){
                for( int s = 0; s < sbins; s++ )
                    {
                    acum = acum+histo.at<float>(h, s);
                    }
                }
            for( int h = 0; h < hbins; h++ ){
                for( int s = 0; s < sbins; s++ )
                    {
                    bins[h][s] = histo.at<float>(h, s)/acum;
                    }
                }


    //break;

    //*************************************************************
// HIstograma HOUGH
    //case 2:

    CalcularHistogramaH(xo1, yo1, anc, alt);
        for(int i=0; i<5; i++){
        Alvo_hist[i]=hist[i];
        Alvo_histR[i]=histR[i];
        }

    //break;
//*****************************************************************
    //case 10:
        //Superpixel(xo1,yo1,anc,alt);
         if((tipos==10)||(tipos==11)||(tipos==12)){
        Mat    D (img3, Rect(xo1, yo1, anc, alt) );
        lbp.Calculo_HistogramaLBP(D,anc,alt);
        for (int i=0;i<8;i++){
        HistLBP_MP[i]=lbp.HistogramaLPB[i];
        }
        //printf("xi=%i yi=%i anc=%i alt=%i\n",xo1, yo1,  anc, alt);
        }
       //cvReleaseImage(&hist_img2);
    //break;
//*******************************************************************
//HIstograma LBP
    //case 3;

  Mat s (img3, Rect(xo1, yo1, anc, alt) );
   lbp.Calculo_HistogramaLBP(s,anc, alt);
        for (int i=0;i<8;i++){
        HistLBP_Alvo[i]=lbp.HistogramaLPB[i];

        }

    //break;
    //    }
}







void Imagem::Proyectar()
{
    img=cvQueryFrame(capt);

        img2=img.clone();
        img3=img.clone();
    //hsv= cvCreateImage(cvSize(an,al),8,3);
    cvtColor( img, hsv, CV_BGR2HSV );
    wimax=0;
    //cvNamedWindow( "ventana", 1 );
    //cvShowImage( "ventana", img);
}



void Imagem::CalculoMP(int tipos)
{

//*****************************************************************
//Histograma de cores
   // case 1:
    int    channels[] = {0, 1};
    int histSize[] = {hbins, sbins};
    float hranges[] = { 0, 180 };
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };

    xm=Estado[0][0];
    ym=Estado[1][0];
    Hxm=Estado[2][0];
    Hym=Estado[3][0];

    rectangle(img2,
        Point( xm, ym ),
        Point( xm+Hxm, ym+Hym),
        CV_RGB(255,0,0),
        1);

    Mat CHSV (hsv, Rect(xm, ym, Hxm, Hym) );
    calcHist( &CHSV, 1, channels, Mat(), // do not use mask
             histo, 2, histSize, ranges,
             true, // the histogram is uniform
             false );
             int acum=0;
             for( int h = 0; h < hbins; h++ ){
                for( int s = 0; s < sbins; s++ )
                    {
                    acum = acum+histo.at<float>(h, s);
                    }
                }
            for( int h = 0; h < hbins; h++ ){
                for( int s = 0; s < sbins; s++ )
                    {
                    bins3[h][s] = histo.at<float>(h, s)/acum;
                    }
                }



    //*************************************************************
// HIstograma HOUGH
    //case 2:

            xm=Estado[0][1];
            ym=Estado[1][1];
            Hxm=Estado[2][1];
            Hym=Estado[3][1];
            rectangle(img2,
                Point( xm, ym ),
                Point( xm+Hxm, ym+Hym),
                CV_RGB(0,255,0),
                1);
    CalcularHistogramaH(xm, ym, Hxm, Hym);
        for(int i=0; i<5; i++){
        MP_hist[i]=hist[i];
        MP_histR[i]=histR[i];
        }

//*****************************************************************
/*    //case 10:
        //Superpixel(xo1,yo1,anc,alt);
         if((tipos==10)||(tipos==11)||(tipos==12)){
        Mat    D (img3, Rect(xm, ym, Hxm, Hym) );
        lbp.Calculo_HistogramaLBP(D,Hx,Hy);
        for (int i=0;i<8;i++){
        HistLBP_Alvo[i]=lbp.HistogramaLPB[i];
        }
        //printf("xi=%i yi=%i anc=%i alt=%i\n",xm, ym, Hxm, Hym);
        }
       //cvReleaseImage(&hist_img2);
    //break;
         */
//*******************************************************************
//HIstograma LBP
    //case 3;
         xm=Estado[0][2];
         ym=Estado[1][2];
         Hxm=Estado[2][2];
         Hym=Estado[3][2];
         rectangle(img2,
             Point( xm, ym ),
             Point( xm+Hxm, ym+Hym),
             CV_RGB(0,0,255),
             1);

  Mat s (img3, Rect(xm, ym, Hxm, Hym) );
   lbp.Calculo_HistogramaLBP(s,Hxm,Hym);
        for (int i=0;i<8;i++){
        HistLBP_Alvo[i]=lbp.HistogramaLPB[i];
        }
}


void Imagem::Estimacion(double particulas_c[12][2000],double particulas_h[12][2000],double particulas_l[12][2000])
{
    float sumx=0, sumy=0, sumHx=0, sumHy=0, wi=0;
    double d, dif;
    int Nc,Nh, Nl;
    Nc=Estado[11][0];
    Nh=Estado[11][1];
    Nl=Estado[11][2];

    for(int i=0; i<Nc;i++)
    {
    sumx=sumx+particulas_c[0][i]+(particulas_c[2][i]/2);
    sumy=sumy+particulas_c[1][i]+(particulas_c[3][i]/2);
    }
    Estado[9][0]=sumx/(Nc);
    Estado[10][0]=sumy/(Nc);
    //printf("Xcc=%f Ycc=%f\n",Estado[9][0],Estado[10][0]);

    sumx=sumy=0;
    for(int i=0; i<Nh;i++)
    {
    sumx=sumx+particulas_h[0][i]+(particulas_h[2][i]/2);
    sumy=sumy+particulas_h[1][i]+(particulas_h[3][i]/2);
    }
    Estado[9][1]=sumx/(Nh);
    Estado[10][1]=sumy/(Nh);

    sumx=sumy=0;
    for(int i=0; i<Nh;i++)
    {
    sumx=sumx+particulas_l[0][i]+(particulas_l[2][i]/2);
    sumy=sumy+particulas_l[1][i]+(particulas_l[3][i]/2);
    }
    Estado[9][2]=sumx/(Nl);
    Estado[10][2]=sumy/(Nl);

}


void Imagem::AlgoritmoMetropolis( double particulas_c[12][2000],double particulas_h[12][2000],double particulas_l[12][2000], bool ocl, int tipos)
{
    float d,d1,d2, dif, wi;
    double Wcor, Wlbp, Whough;
    float sumx=0, sumy=0, sx, sy;
    float divx=0, divy=0;
    int Nc,Nh, Nl;
    Nc=Estado[11][0];
    Nh=Estado[11][1];
    Nl=Estado[11][2];


    if(Estado[6][0]==0){

    //*****************************************************************
//Histograma de cores
// case 1:
//if((tipos==1)||(tipos==4)||(tipos==5)||(tipos==7)){
    for (int i=0;i<Nc;i++)
    {

        x=particulas_c[0][i];
        y=particulas_c[1][i];
        Hx=particulas_c[2][i];
        Hy=particulas_c[3][i];

    int    channels[] = {0, 1};
    int histSize[] = {hbins, sbins};
    float hranges[] = { 0, 180 };
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    Mat CHSV (hsv, Rect(x, y, Hx, Hy) );
    calcHist( &CHSV, 1, channels, Mat(), // do not use mask
             histo2, 2, histSize, ranges,
             true, // the histogram is uniform
             false );
             //Wcor=compareHist(histo2, histo, CV_COMP_BHATTACHARYYA);
             //cvNormalizeHist(&histo2, 2);
             int acum=0;
             for( int h = 0; h < hbins; h++ ){
                for( int s = 0; s < sbins; s++ )
                    {
                    acum = acum+histo2.at<float>(h, s);
                    }
                }
            for( int h = 0; h < hbins; h++ ){
                for( int s = 0; s < sbins; s++ )
                    {
                    bins2[h][s] = histo2.at<float>(h, s)/acum;
                    }
                }


    d=0;
        for(int p=0;p<sbins;p++)
            {
            for(int q=0;q<hbins;q++)
                {
                    d=d+sqrt(bins[p][q]*bins2[p][q]);
                    //printf("bins2=%f,bins5=%f i=%i,j=%i,d=%f\n",bins2[i][j],bins5[i][j],i,j,d);
                }
            }

        if(d<1){
        dif=sqrt(1-d);
        //printf("dif=%f d=%f\n",dif,d);
        Wcor=exp(-20*dif*dif);
        }
        else{
        Wcor=0;
        }
//
//    }

    //break;
        if((Wcor/particulas_c[12][i])>1)
                {
                    particulas_c[0][i] = x;
                    particulas_c[1][i] = y;
                    particulas_c[2][i] = Hx;
                    particulas_c[3][i] = Hy;
                    particulas_c[12][i] = Wcor;
                }
                else
                {
                    particulas_c[0][i] = particulas_c[8][i];
                    particulas_c[1][i] = particulas_c[9][i];
                    particulas_c[2][i] = particulas_c[10][i];
                    particulas_c[3][i] = particulas_c[11][i];
                    particulas_c[12][i] = particulas_c[12][i];
                }

}
    }


    if(Estado[6][1]==0){
    //*************************************************************
// HIstograma HOUGH
    //case 2:
//     if((tipos==2)||(tipos==4)||(tipos==6)||(tipos==7)){
for (int i=0;i<Nh;i++){
    x=particulas_h[0][i];
    y=particulas_h[1][i];
    Hx=particulas_h[2][i];
    Hy=particulas_h[3][i];
    CalcularHistogramaH(x, y, Hx, Hy);

        for(int i=0; i<5; i++){
        Part_hist[i]=hist[i];
        Part_histR[i]=histR[i];
        }

           d2=0;
          d1=0;
        for(int p=0;p<5;p++){
            d1=d1+sqrt(Part_hist[p]*Alvo_hist[p]);
            d2=d2+sqrt(Part_histR[p]*Alvo_histR[p]);
            }
        d=(d2+d1);
        if(d<=1){
        dif=sqrt(1-d);
        //printf("dif=%f d=%f\n",dif,d);
        Whough=exp(-20*dif*dif);
        }
        else{
        Whough=0;
        }
         //printf("dif=%f d=%f Whough=%f\n",dif,d,Whough);
        if((Whough/particulas_h[12][i])>1)
                {
                    particulas_h[0][i] = x;
                    particulas_h[1][i] = y;
                    particulas_h[2][i] = Hx;
                    particulas_h[3][i] = Hy;
                    particulas_h[12][i] = Whough;
                }
                else
                {
                    particulas_h[0][i] = particulas_h[8][i];
                    particulas_h[1][i] = particulas_h[9][i];
                    particulas_h[2][i] = particulas_h[10][i];
                    particulas_h[3][i] = particulas_h[11][i];
                    particulas_h[12][i] = particulas_h[12][i];
                }
        }
    //break;

    }

 //*****************************************************************
/*    //case 10:
        //Superpixel(xo1,yo1,anc,alt);
         if((tipos==10)||(tipos==11)||(tipos==12)){
        Mat    D (img3, Rect(x, y, Hx, Hy) );
        lbp.Calculo_HistogramaLBP(D,Hx,Hy);
        for (int i=0;i<8;i++){
        HistLBP_Alvo[i]=lbp.HistogramaLPB[i];
        }
       // printf("xi=%i yi=%i anc=%i alt=%i\n",x, y, Hx, Hy);
        }
       //cvReleaseImage(&hist_img2);
    //break;

*/


    if(Estado[6][2]==0){

//*******************************************************************
//HIstograma LBP
    //case 3;
    // if((tipos==3)||(tipos==5)||(tipos==6)||(tipos==7)){
for (int i=0;i<Nl;i++){
    x=particulas_l[0][i];
    y=particulas_l[1][i];
    Hx=particulas_l[2][i];
    Hy=particulas_l[3][i];
  Mat s (img3, Rect(x, y, Hx, Hy) );
   lbp.Calculo_HistogramaLBP(s,Hx, Hy);
        for (int i=0;i<8;i++){
        HistLBP_Part[i]=lbp.HistogramaLPB[i];

        }

        d=0;



        for(int p=0;p<sbins;p++)
            {
                    d=d+sqrt(HistLBP_Part[p]*HistLBP_Alvo[p]);
                    //printf("bins2=%f,bins5=%f i=%i,j=%i,d=%f\n",bins2[i][j],bins5[i][j],i,j,d);
             }

        if(d<1){
        dif=sqrt(1-d);
        //printf("dif=%f d=%f\n",dif,d);
        Wlbp=exp(-20*dif*dif);
        }
        else{
        Wlbp=0;
        }
        if((Wlbp/particulas_l[12][i])>1)
                {
                    particulas_l[0][i] = x;
                    particulas_l[1][i] = y;
                    particulas_l[2][i] = Hx;
                    particulas_l[3][i] = Hy;
                    particulas_l[12][i] = Wlbp;
                }
                else
                {
                    particulas_l[0][i] = particulas_l[8][i];
                    particulas_l[1][i] = particulas_l[9][i];
                    particulas_l[2][i] = particulas_l[10][i];
                    particulas_l[3][i] = particulas_l[11][i];
                    particulas_l[12][i] = particulas_l[12][i];
                }

        }
    }


    sumx=sumy=divx=divy=0;
for (int i=0;i<Nc;i++){
        sx=(particulas_c[0][i]-particulas_c[8][i]);
        sy=(particulas_c[1][i]-particulas_c[9][i]);
        if (sx!=0)
        {
            sumx=sumx+sx;
            divx++;
        }
        if (sy!=0)
        {
            sumy=sumy+sy;
            divy++;
        }



        }

    if (divx==0)
    {
        Vx=0;
    }
    else
    {
        Vx=sumx/divx;
    }
    if (divy==0)
    {
        Vy=0;
    }
    else
    {
        Vy=sumy/divy;
    }


    Estado[7][0]=Vx;
    Estado[7][0]=Vy;

    sumx=sumy=divx=divy=0;
for (int i=0;i<Nh;i++){
        sx=(particulas_h[0][i]-particulas_h[8][i]);
        sy=(particulas_h[1][i]-particulas_h[9][i]);
        if (sx!=0)
        {
            sumx=sumx+sx;
            divx++;
        }
        if (sy!=0)
        {
            sumy=sumy+sy;
            divy++;
        }



        }

    if (divx==0)
    {
        Vx=0;
    }
    else
    {
        Vx=sumx/divx;
    }
    if (divy==0)
    {
        Vy=0;
    }
    else
    {
        Vy=sumy/divy;
    }


    Estado[7][1]=Vx;
    Estado[7][1]=Vy;

    sumx=sumy=divx=divy=0;
for (int i=0;i<Nl;i++){
        sx=(particulas_l[0][i]-particulas_l[8][i]);
        sy=(particulas_l[1][i]-particulas_l[9][i]);
        if (sx!=0)
        {
            sumx=sumx+sx;
            divx++;
        }
        if (sy!=0)
        {
            sumy=sumy+sy;
            divy++;
        }



        }

    if (divx==0)
    {
        Vx=0;
    }
    else
    {
        Vx=sumx/divx;
    }
    if (divy==0)
    {
        Vy=0;
    }
    else
    {
        Vy=sumy/divy;
    }


    Estado[7][0]=Vx;
    Estado[7][0]=Vy;

}






void Imagem::LimpiarMemoria()
{
    //printf("x %i y %i Hx %i Hy %i\n", x, y, Hx, Hy);
    //cvNamedWindow("ventana2",0);
    //cvShowImage( "ventana2", img2);



}

void Imagem::ActHistograma(float samples, bool ocl,float fator,int tipos)
{
  double w;

      //w=20*wimax/suma;
      w=0.3;
      we=we/(suma+we);



      if(Estado[6][0]==0){
          for( int s = 0; s < 10; s++ ){
              for( int h = 0; h < 10; h++ ){
                  bins[h][s]=(1-(w))*bins[h][s]+(w)*bins3[h][s];
                  }
            }
      }

      if(Estado[6][1]==0){

          for( int s = 0; s < 5; s++){
              Alvo_hist[s]=(1-w)*Alvo_hist[s]+w*MP_hist[s];
              Alvo_histR[s]=(1-w)*Alvo_histR[s]+w*MP_histR[s];
          }
     }


      if(Estado[6][0]==0){

          for( int s = 0; s < 8; s++ ){
                  HistLBP_Alvo[s]=(1-(w))*HistLBP_Alvo[s]+(w)*HistLBP_MP[s];
                  }
          }

    }


void Imagem::DisenhoParticulas(double particulas_c[12][2000], double particulas_h[12][2000], double particulas_l[12][2000]){
//*****************************************************************
// disenho das particulas
    int Nc,Nh, Nl;
    Nc=Estado[11][0];
    Nh=Estado[11][1];
    Nl=Estado[11][2];

        if(Estado[6][0]==0){
                for (int i=0;i<Nc;i++){
                    x=particulas_c[0][i];
                    y=particulas_c[1][i];
                    Hx=particulas_c[2][i];
                    Hy=particulas_c[3][i];
                //printf("xM=%f yM=%f an=%f al=%f\n",x,y,Hx,Hy);



                    rectangle(img2,
                            Point( x+Hx/2, y+Hy/2 ),
                            Point( (x+Hx/2)+1, (y+Hy/2)+1),
                            CV_RGB(255,0,0),
                            0);

                    }


        }
        else{
            for (int i=0;i<Nc;i++){
                x=particulas_c[0][i];
                y=particulas_c[1][i];
                Hx=particulas_c[2][i];
                Hy=particulas_c[3][i];
            //printf("xM=%f yM=%f an=%f al=%f\n",x,y,Hx,Hy);
                rectangle(img2,
                        Point( x+Hx/2, y+Hy/2 ),
                        Point( (x+Hx/2)+1, (y+Hy/2)+1),
                        CV_RGB(155,0,0),
                        0);

                }
        }
         if(Estado[6][1]==0){


                for (int i=0;i<Nh;i++){
                        x=particulas_h[0][i];
                        y=particulas_h[1][i];
                        Hx=particulas_h[2][i];
                        Hy=particulas_h[3][i];
                        //printf("xM=%f yM=%f an=%f al=%f\n",x,y,Hx,Hy);
                        rectangle(img2,
                            Point( x+Hx/2, y+Hy/2 ),
                            Point( (x+Hx/2)+1, (y+Hy/2)+1),
                            CV_RGB(0,255,0),
                            0);
                    }

         }

         else{
             for (int i=0;i<Nh;i++){
                     x=particulas_h[0][i];
                     y=particulas_h[1][i];
                     Hx=particulas_h[2][i];
                     Hy=particulas_h[3][i];
                     //printf("xM=%f yM=%f an=%f al=%f\n",x,y,Hx,Hy);
                     rectangle(img2,
                         Point( x+Hx/2, y+Hy/2 ),
                         Point( (x+Hx/2)+1, (y+Hy/2)+1),
                         CV_RGB(0,155,0),
                         0);
                 }

         }
          if(Estado[6][2]==0){
                for (int i=0;i<Nl;i++){
                        x=particulas_l[0][i];
                        y=particulas_l[1][i];
                        Hx=particulas_l[2][i];
                        Hy=particulas_l[3][i];
                        //printf("xM=%f yM=%f an=%f al=%f\n",x,y,Hx,Hy);
                        rectangle(img2,
                            Point( x+Hx/2, y+Hy/2 ),
                            Point( (x+Hx/2)+1, (y+Hy/2)+1),
                            CV_RGB(0,0,255),
                            0);
                    }

          }

          else{

            for (int i=0;i<Nl;i++){
                    x=particulas_l[0][i];
                    y=particulas_l[1][i];
                    Hx=particulas_l[2][i];
                    Hy=particulas_l[3][i];
                    //printf("xM=%f yM=%f an=%f al=%f\n",x,y,Hx,Hy);
                    rectangle(img2,
                        Point( x+Hx/2, y+Hy/2 ),
                        Point( (x+Hx/2)+1, (y+Hy/2)+1),
                        CV_RGB(0,0,155),
                        0);
                }

                    }



}















Imagem::~Imagem()
{
    //dtor
}

