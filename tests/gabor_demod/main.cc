#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <imcore/seguidor.h>
#include "scanner.h"
#include <utils/utils.h>
#include <iostream>
#include "gabor_gears.h"

/**
 * Calculates the gradient differences.
 *
 * @param I is the image.
 * @param dx [output] the differences along x-direction (columns)
 * @param dy [output] the differences along y-direction (rows)
 */
void gradient(const cv::Mat I, cv::Mat& dx, cv::Mat& dy)
{
  dx.create(I.rows, I.cols, I.type());
  dy.create(I.rows, I.cols, I.type());

  for(int i=0; i<I.rows-1; i++)
    for(int j=0; j<I.cols-1; j++){
      dx.at<double>(i,j) = I.at<double>(i,j+1)-I.at<double>(i,j);
      dy.at<double>(i,j) = I.at<double>(i+1,j)-I.at<double>(i,j);
    }
  for(int i=0; i<I.rows; i++)
    dy.at<double>(i,I.cols-1)=dy.at<double>(i,I.cols-2);
  for(int i=0; i<I.cols; i++)
    dx.at<double>(I.rows-1,i)=dx.at<double>(I.rows-2,i);
}

/**
 * Filtrates the neighborhood around (i,j) with Gabor filter.
 *
 * It applies the Gabor filter to mage I around pixel (i, j) including itself.
 * It tries to filter neighbors (i-1,j) and (i,j-1) if they are into the image
 * matrix; otherwise it filters (i+1,j) or (i,j+1).
 *
 * @param I the image
 * @param fr [output] here is stored the real part of the result at position
 *           (i,j)
 * @param fi [output] here is stored the imaginary part of the result at
 *           position (i,j)
 * @param wx the tuning frequency of the Gabor filter at x-direction.
 * @param wy the tuning frequency of the Gabor filter at y-direction.
 * @param i the i-row of the image
 * @param j the j-column of the image
 */
inline
void filtraNeighborhood(const cv::Mat I, cv::Mat fr, cv::Mat fi,
                        double wx, double wy, int i, int j)
{
  if(i-1>=0)
   gabor_adaptiveFilterXY(I, fr, fi, wx, wy,
                          j, i-1);
  else if(i+1<I.rows)
   gabor_adaptiveFilterXY(I, fr, fi, wx, wy,
                          j, i+1);
  if(j-1>=0)
   gabor_adaptiveFilterXY(I, fr, fi, wx, wy,
                          j-1, i);
  else if(j+1<I.cols)
   gabor_adaptiveFilterXY(I, fr, fi, wx, wy,
                          j+1, i);
  gabor_adaptiveFilterXY(I, fr, fi, wx, wy,
                         j, i);
}

inline
void demodPixel(cv::Mat I, cv::Mat fr, cv::Mat fi, cv::Mat fx, cv::Mat fy,
                cv::Mat visited, int i, int j)
{
  cv::Vec2d freqs;
  freqs= peak_freqXY(fx, fy, visited, j, i);
  filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
  freqs = calc_freqXY(fr, fi, j, i);
  //freqs = stima_freqXY(I, freqs, j, i);

  fx.at<double>(i,j)=freqs[0];
  fy.at<double>(i,j)=freqs[1];
  visited.at<char>(i,j)=1;
}

inline
void demodNeighbor(cv::Mat I, cv::Mat fr, cv::Mat fi, cv::Mat fx, cv::Mat fy,
                   cv::Mat visited, int i, int j)
{
  cv::Mat_<char>& visit=(cv::Mat_<char>&)visited;
  //if(!visit(i,j))
    demodPixel(I, fr, fi, fx, fy, visited, i, j);
  if(j-1>=0)
    if(!visit(i,j-1))
      demodPixel(I, fr, fi, fx, fy, visited, i, j-1);
  if(j+1<I.cols)
    if(!visit(i,j+1))
      demodPixel(I, fr, fi, fx, fy, visited, i, j+1);
  if(i-1>=0)
    if(!visit(i-1,j))
      demodPixel(I, fr, fi, fx, fy, visited, i-1, j);
  if(i+1<I.rows)
    if(!visit(i+1,j))
      demodPixel(I, fr, fi, fx, fy, visited, i+1, j);
  if(j-1>=0 && i-1>=0)
    if(!visit(i-1,j-1))
      demodPixel(I, fr, fi, fx, fy, visited, i-1, j-1);
  if(j+1<I.cols && i-1>=0)
    if(!visit(i-1,j+1))
      demodPixel(I, fr, fi, fx, fy, visited, i-1, j+1);
  if(j+1<I.cols && i+1<I.rows)
    if(!visit(i+1,j+1))
      demodPixel(I, fr, fi, fx, fy, visited, i+1, j+1);
  if(j-1>=0 && i+1<I.rows)
    if(!visit(i+1,j-1))
      demodPixel(I, fr, fi, fx, fy, visited, i+1, j-1);
}

inline
void demodPixelSeed(cv::Mat I, cv::Mat fr, cv::Mat fi, cv::Mat fx, cv::Mat fy,
                    cv::Mat visited, cv::Vec2d freqs, int i, int j)
{
  filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
  freqs = calc_freqXY(fr, fi, j, i);
  fx.at<double>(i,j)=freqs[0];
  fy.at<double>(i,j)=freqs[1];
  visited.at<char>(i,j)=1;
}

int main(int argc, char* argv[])
{
  double wx= .7, wy=.7;
  const int M=456, N=456;
  cv::Mat I(M,N,CV_64F);
  cv::Mat phase(M,N,CV_64F);
  cv::Mat fx(cv::Mat::zeros(M,N,CV_64F)), fy(cv::Mat::zeros(M,N,CV_64F)),
      ffx, ffy; // las f's son las frecuencias teoricas y las
                // ff's las estimadas
  cv::Mat noise(M,N,CV_64F);
  cv::Mat fr(cv::Mat::zeros(M,N,CV_64F)), fi(cv::Mat::zeros(M,N,CV_64F)),
      visited;
  cv::Mat path(M,N,CV_64F);
  cv::Mat magn;
  cv::Mat hxr, hxi, hyr, hyi;
  cv::Mat h;
  cv::Mat fase;
  cv::Mat tmp;

  if(argc==1){
    // Genera datos de entrada
    //parabola(phase, 0.0008);
    phase = peaks(M, N)*33;
    //phase=ramp(wx, wy, M, N);
    phase.convertTo(phase, CV_64F);
    I=cos<double>(phase);
    
    gradient(phase, fx, fy);
    cv::randn(noise, 0, 1.5);
    I=I+noise;
  }
  else{
    I=cv::imread(argv[1], 0);
    I.convertTo(tmp, CV_64F);
    I=tmp.clone();
    cv::GaussianBlur(I, I, cv::Size(0,0), 1);
    cv::GaussianBlur(I, tmp, cv::Size(0,0), 13);
    I = I - tmp;
  }

  // Aplica filtro y busca el pixel que esta entonado con el filtro.
  // despues calcula su frecuencia local
  gabor_filter(I, fr, fi, wx, wy);
  cv::magnitude(fr, fi, magn);
  double min, max;
  cv::Point2i p;
  cv::minMaxLoc(magn, &min, &max, NULL, &p);
  std::cout<<"Puno inicial: ("<<p.x<<", "<<p.y<<")"<<std::endl;
  //I.at<double>(p.y, p.x)=20;
  cv::Vec2d freqs = calc_freqXY(fr, fi, p.x, p.y);
  std::cout<<"Frecuencia local en el punto: ("<<freqs[0]<<", "<<freqs[1]
          <<")"<<std::endl;
  std::cout<<"Frecuencia teorica local en el punto: ("<<fx.at<double>(p.y,p.x)
           <<", "<<fy.at<double>(p.y,p.x) <<")"<<std::endl;

  freqs[0]=.7; freqs[1]=.7;
  p.x=I.rows/2; p.y=I.cols/2;

  int i=p.y, j=p.x, cont=0;
  ffx = cv::Mat::ones(I.rows, I.cols, CV_64F)*M_PI/2.0;
  ffy = cv::Mat::ones(I.rows, I.cols, CV_64F)*M_PI/2.0;
  fr = cv::Mat::zeros(I.rows, I.cols, CV_64F);
  fi = cv::Mat::zeros(I.rows, I.cols, CV_64F);
  ffx.at<double>(i,j)=freqs[0];
  ffy.at<double>(i,j)=freqs[1];
  visited = cv::Mat::zeros(I.rows, I.cols, CV_8U);

  Scanner scan(ffx, ffy, p);
  scan.setFreqMin(.01);
  cv::Point pixel;
  do{
    pixel=scan.getPosition();
    i=pixel.y;
    j=pixel.x;
    if((i==p.y && j==p.x)){
      demodPixelSeed(I,fr,fi,ffx,ffy,visited,freqs,i,j);
      freqs[0]=ffx.at<double>(i,j); freqs[1]=ffy.at<double>(i,j);
      std::cout<<"Frecuencia estimada local en el punto-> ("<<freqs[0]
              <<", "<<freqs[1] <<")"<<std::endl;
      demodPixelSeed(I,fr,fi,ffx,ffy,visited,freqs,i,j);
      freqs[0]=ffx.at<double>(i,j); freqs[1]=ffy.at<double>(i,j);
      std::cout<<"Frecuencia estimada local en el punto-> ("<<freqs[0]
              <<", "<< freqs[1]<<")"<<std::endl;
      demodPixelSeed(I,fr,fi,ffx,ffy,visited,freqs,i,j);
      freqs[0]=ffx.at<double>(i,j); freqs[1]=ffy.at<double>(i,j);
      std::cout<<"Frecuencia estimada local en el punto-> ("<<freqs[0]
              <<", "<< freqs[1]<<")"<<std::endl;
      scan.setFreqMin(sqrt(freqs[0]*freqs[0]+freqs[1]*freqs[1]));
    }
    else
      demodNeighbor(I, fr, fi, ffx, ffy, visited, i,j);

    // Codigo para mostrar resultados en tiempo real
    if((cont++)%5000==000){
      // Genera kerneles del filtro de gabor
      wx = ffx.at<double>(i,j);
      wy = ffy.at<double>(i,j);
      double sx = fabs(1.5708/wx), sy = fabs(1.5708/wy);
      sx = sx>7? 7:(sx<1? 1:sx);
      sy = sy>7? 7:(sy<1? 1:sy);
      gen_gaborKernel(hxr, hxi, wx, sx, CV_64F);
      gen_gaborKernel(hyr, hyi, wy, sy, CV_64F);
      // Genera la parte imaginaria del filtro de gabor para desplegarlo
      h=cv::Mat::zeros(64,64, CV_64F)-1;
      for(int i=0; i<hyr.cols; i++)
        for(int j=0; j<hxr.cols; j++)
          h.at<double>(i,j)=hxr.at<double>(0,j)*hyi.at<double>(0,i) +
              hxi.at<double>(0,j)*hyr.at<double>(0,i);

      cv::normalize(fr,tmp,1,0,cv::NORM_MINMAX);
      cv::imshow("real", tmp);
      cv::normalize(h,tmp,1,0,cv::NORM_MINMAX);
      cv::imshow("cos(fase)", tmp);
      cv::waitKey(32);
    }
  }while(scan.next());

  // Calculo de la fase de salida
  fase = atan2<double>(fi,fr);
  cv::magnitude(fr, fi, magn);
  //cv::threshold(ffx, magn, 0, 1, cv::THRESH_BINARY);

  cv::namedWindow("I");
  cv::namedWindow("real");
  cv::namedWindow("imag");
  //cv::namedWindow("fx");
  //cv::namedWindow("fy");
  cv::namedWindow("ffy");
  cv::namedWindow("ffy");
  cv::namedWindow("cos(fase)");
  cv::namedWindow("fase");

  cv::normalize(I,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("I", tmp);
  cv::normalize(fr,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("real", tmp);
  cv::normalize(fi,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("imag", tmp);
  cv::normalize(fase,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("fase", tmp);
  //cv::normalize(fx,tmp,1,0,cv::NORM_MINMAX);
  //cv::imshow("fx", tmp);
  //cv::normalize(fy,tmp,1,0,cv::NORM_MINMAX);
  //cv::imshow("fy", tmp);
  cv::normalize(ffx,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("ffx", tmp);
  cv::normalize(ffy,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("ffy", tmp);
  cv::normalize(cos<double>(fase),tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("cos(fase)", tmp);

  cv::waitKey(0);

  return 0;
}
