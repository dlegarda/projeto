#include <iostream>
#include "Imagem.h"
#include "particula.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

IplImage *frame;
using namespace std;
ofstream myfile;
int xo1, yo1, anc, alt, xi, yi, xf, yf;
bool act=false;
float dados[6];
float erro;
void mouse(int event, int x, int y, int flags, void* param)
{

   switch(event){
      case CV_EVENT_LBUTTONDOWN:
        //if(flags & CV_EVENT_FLAG_CTRLKEY)
            xi=x;
            yi=y;
            break;

      case CV_EVENT_LBUTTONUP:
            xf=x;
            yf=y;
            xo1=xi+(xf-xi)/2;
            yo1=yi+(yf-yi)/2;
            anc=abs(xf-xi);
            alt=abs(yf-yi);
            act=true;

            break;
       }
}

int grabar (float dados[6], int frames) {
  //for(int i=0;i<100;i++)
  {
    char cadena[100]="";
    char cad[15]="";
    sprintf(cad,"%d",frames);
    strcat(cadena,cad);
    strcat(cadena," ");
    sprintf(cad,"%f",dados[1]);//particulas[0][i]);
    strcat(cadena,cad);
    strcat(cadena," ");
    sprintf(cad,"%f",dados[2]);//particulas[1][i]);
    strcat(cadena,cad);
    strcat(cadena," ");
    sprintf(cad,"%f",dados[3]);//particulas[8][i]);
    strcat(cadena,cad);
    strcat(cadena," ");
//    sprintf(cad,"%f",dados[4]);//particulas[9][i]);
//    strcat(cadena,cad);
//    strcat(cadena," ");
//    sprintf(cad,"%f",dados[5]);
//    strcat(cadena,cad);
    strcat(cadena,"\n");
    //printf(cadena);
    myfile << cadena;
  }
 return 0;
}





int main()
{

float samp;
double Recall, fscore;
float sum;
float xi1, yi1;
float Ex, Ey;
int tipos=7;
int video=3;
int particulas;
int limframes;
float SIG,SIG1;
int num=0;
int Ax, Ay;
float coc, Positivos;
float fator;

srand(time(NULL));

particulas=100;
SIG1=1.9;
SIG=2;
fator=0.1;


cout<<"Metodo"<<endl;
cin>>tipos;
cout<<"Sequencia"<<endl;
cin>>video;
cout<<"No Particulas"<<endl;
cin>>particulas;




float GT[5][1200];
char cadena[100];
char arquivo[100]="";
char Arqsaida[100]="";
char resul[10]="Res";
char Npar[10]="";
char Met[10]="";
char cad1[30]="CarDark";
char cad2[30]="Crossing";
char cad3[30]="FaceOcc1";
char cad4[30]="Singer1";
char cad5[30]="Singer2";
char ar[10]=".txt";

sprintf(Npar,"%d",particulas);
sprintf(Met,"%d",tipos);


switch(video){
case 1:
strcat(Arqsaida,Met);
strcat(Arqsaida,cad1);
strcat(Arqsaida,Npar);
strcat(Arqsaida,ar);


strcat(arquivo,cad1);
strcat(arquivo,ar);


limframes=392;
//limframes=10;
break;
case 2:
strcat(Arqsaida,Met);
strcat(Arqsaida,cad2);
strcat(Arqsaida,Npar);
strcat(Arqsaida,ar);


strcat(arquivo,cad2);
strcat(arquivo,ar);

limframes=119;
break;
case 3:
strcat(Arqsaida,Met);
strcat(Arqsaida,cad3);
strcat(Arqsaida,Npar);
strcat(Arqsaida,ar);


strcat(arquivo,cad3);
strcat(arquivo,ar);

limframes=891;
break;
case 4:
strcat(Arqsaida,Met);
strcat(Arqsaida,cad4);
strcat(Arqsaida,Npar);
strcat(Arqsaida,ar);

strcat(arquivo,cad4);
strcat(arquivo,ar);

limframes=350;
break;
case 5:
strcat(Arqsaida,Met);
strcat(Arqsaida,cad5);
strcat(Arqsaida,Npar);
strcat(Arqsaida,ar);

strcat(arquivo,cad5);
strcat(arquivo,ar);

limframes=365;
break;
}







//file saida de resultados
myfile.open (Arqsaida);




    ifstream leer(arquivo);//ground truth
if(!leer)
    {
        printf("NO se pudo leer el archivo");

    }
else
    {
        while (!leer.eof())
        //for(int i=0;i<2;i++)
        {  leer>>cadena;
           GT[0][num]=atof(cadena);
           //cout<<cadena<<"\n";
           leer>>cadena;
           GT[1][num]=atof(cadena);
           //cout<<cadena<<"\n";
           leer>>cadena;
           GT[2][num]=atof(cadena);
           leer>>cadena;
           //cout<<cadena<<"\n";
           GT[3][num]=atof(cadena);
           leer>>cadena;
           //cout<<cadena<<"\n";
           //GT[4][num]=atof(cadena);
           //cout<<cadena<<"\n";
           num++;
        }
    }
leer.close();










for(int i=1;i<5;i++) {
    //particulas=particulas+100;
    //SIG=SIG+1;
   // fator=fator+0.01;
//    char cade[20]="";
//    char cad[15]="";
//    strcat(cade,"face");
//    sprintf(cad,"%d",particulas);
//    strcat(cade,cad);
//    strcat(cade,".txt");
//    printf(cade);





Imagem nova(1);
particula p1(20,20,20,particulas);
nova.Estado[11][0]=particulas/3;
nova.Estado[11][1]=particulas/3;
nova.Estado[11][2]=particulas/3;
printf("Nc=%f Nh=%f Nl=%f\n",nova.Estado[11][0],nova.Estado[11][1],nova.Estado[11][2]);
p1.Inicia(SIG,SIG1);
nova.DeterminarROI();
xi1=yi1=150;
anc=alt=5;
//p1.variacion( xi1,  yi1,  anc,  alt,nova.M,nova.limx,nova.limy);
samp=p1.numero;
int frames=0;
int num1=1;

nova.Fc=nova.Fh=nova.Fl=1/3;

nova.correFrames(num1,video);
        xi=GT[0][0];
        yi=GT[1][0];
        anc=GT[2][0];
        alt=GT[3][0];
        act=true;
        erro=0;
        Positivos=0;
    while(1)
    {   num1++;
        frames++;
        //printf("frame=%i\n",frames);
        //p1.variacion( xi1,  yi1,  anc,  alt,nova.M,nova.limx,nova.limy);
        //printf("limx=%i, limy=%i\n",nova.limx, nova.limy);
        //nova.Proyectar();
        nova.correFrames(num1,video);
        //cvSetMouseCallback( ventana, mouse, NULL);


            if (act==true)
        {
            nova.CalculoAlvo(xi,yi,anc,alt,tipos);
            printf("xi=%i yi=%i xf=%i yf=%i anc=%i alt=%i\n",xi, yi, xf, yf, anc, alt);
            p1.variacion( xi,  yi,  anc,  alt,nova.M,nova.limx,nova.limy);
            //p1.actualizacion(nova.M,nova.limx,nova.limy);
            act=false;
        }
//printf("rodada No%i\n",frames);

        nova.CalcularHistograma(samp, p1.particulas_c,p1.particulas_h,p1.particulas_l,p1.oclusion,tipos);
        sum=nova.suma;
        p1.actualizacion(nova.M,nova.limx,nova.limy, nova.Estado);
        nova.AlgoritmoMetropolis(p1.particulas_c,p1.particulas_h,p1.particulas_l, p1.oclusion,tipos);
        nova.CalculoMP(tipos);
        nova.Estimacion(p1.particulas_c,p1.particulas_h,p1.particulas_l);
        nova.DisenhoParticulas(p1.particulas_c,p1.particulas_h,p1.particulas_l);
        p1.normaliza(sum, nova.Estado);
        p1.cambio(nova.Estado);
        p1.orden(nova.Estado);
        p1.resampling(nova.Estado);
//printf("rodada No%i\n",frames);
//            printf("rodada No%i\n");


/*


//Calculo de estatisticas do filtro.
//        Ex=(GT[0][frames]+(GT[2][frames]/2))+(nova.xm+(nova.Hxm/2));
//        Ey=(GT[1][frames]+(GT[3][frames]/2))+(nova.ym+(nova.Hym/2));
//        erro=erro+sqrt(Ex*Ex+Ey*Ey);


//Calculo de overlaping
            if(nova.xm<GT[0][frames]){
                 if((nova.xm+nova.Hxm)<(GT[0][frames]+(GT[2][frames]))){
                 Ax=nova.xm+nova.Hxm-GT[0][frames]; // Xf+Anf-Xg
                 }
                 else {
                 Ax=GT[2][frames];
                 }
            }
            else{
            if((nova.xm+nova.Hxm)>(GT[0][frames]+(GT[2][frames]))){
                 Ax=GT[0][frames]+GT[2][frames]-nova.xm;// Xg+Ang-Xf
                 }
                 else {
                 Ax=nova.Hxm;
                 }
            }



            if(nova.ym<GT[1][frames]){
                 if((nova.ym+nova.Hym)<(GT[1][frames]+(GT[3][frames]))){
                 Ay=nova.ym+nova.Hym-GT[1][frames]; // Xf+Anf-Xg
                 }
                 else {
                 Ay=GT[2][frames];
                 }
            }
            else{
            if((nova.ym+nova.Hym)>(GT[1][frames]+(GT[3][frames]))){
                 Ay=GT[1][frames]+GT[3][frames]-nova.ym;// Xg+Ang-Xf
                 }
                 else {
                 Ay=nova.Hym;
                 }
            }
        coc=(Ax*Ay)/((nova.Hxm*nova.Hym)+(GT[2][frames]*GT[3][frames])-(Ax*Ay));
            if(coc>0.5){
            Positivos=Positivos+1;
            }
       //printf("erro=%i\n",Positivos);


*/

        nova.ActHistograma(samp, p1.oclusion,fator,tipos);
//        frame=cvCloneImage(nova.img2);
//        cvNamedWindow(ventana,0);
//        cvShowImage( ventana, frame);
namedWindow( "Source", 0 );
    imshow( "Source", nova.img2 );
        if (frames>1)
        {
            while(1)
            {
            char p = cvWaitKey(1);
            if( p == 9 ) break;
            }
        }

     char c = cvWaitKey(1);
     if( frames == limframes) break;
     nova.LimpiarMemoria();
     cvReleaseImage(&frame);



 //***************************************************************
     //rutina para guardar datos de numero de particulas
     dados[0]=10;
     dados[1]=nova.Estado[11][0];
     dados[2]=nova.Estado[11][1];
     dados[3]=nova.Estado[11][2];

//            dados[5]=fator;

grabar(dados,particulas);
        }

    //ojo cambiar para cuando se este usando datos de error

//        Recall=Positivos/limframes;
//        fscore=(2*Recall)/(1+Recall);

/*            dados[1]=nova.Estado[11][0];
            dados[2]=nova.Estado[11][1];
            dados[3]=nova.Estado[11][2];
//            dados[4]=Positivos/limframes;
//            dados[5]=fator;

    grabar(dados,particulas);*/

}




myfile.close();

}
