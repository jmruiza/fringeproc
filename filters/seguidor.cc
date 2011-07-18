#include "seguidor.h"
#include <cmath>
#include <iostream>

namespace fringe{
Punto::Punto(int col, int row)
{
  c=col;
  r=row;
}

void Seguidor::calcQualityMap(int levels)
{
  int nr=_I.rows();
  int nc=_I.cols();
  Matrix dx(nr,nc);
  Matrix dy(nr,nc);
  double** g=_I.getMat();

  double** qmap=_qmap.getMat();
  Matrix path(nr,nc);
        
  double a=0,b=0;
  for(int i=0; i<nc; i++)
    for(int j=0; j<nr; j++){
      if(i-1>=0)
        dx[i][j]=g[i][j] - g[i-1][j];
      else
        dx[i][j]=g[i+1][j] - g[i][j];
      if(j-1>=0)
        dy[i][j]=g[i][j] - g[i][j-1];
      else
        dy[i][j]=g[i][j+1] - g[i][j];
      path[i][j]=sqrt(dx[i][j]*dx[i][j] + dy[i][j]*dy[i][j]);
      a=(a>path[i][j])? path[i][j]:a;
      b=(b<path[i][j])? path[i][j]:b;
    }
    double step=(b-a)/levels;
    for(int i=0; i<nc; i++)
      for(int j=0; j<nr; j++){
        for(int l=0; l<levels; l++)
          if(path[i][j]>=(a+l*step) && path[i][j]<=((l+1)*step))
            qmap[i][j]=l;
      }
}

void Seguidor::set_inicio()
{
   int i_init=0, j_init=0;
   double** qmap= _qmap.getMat();
   int nr=_qmap.rows();
   int nc=_qmap.cols();
      
   //Primero se busca el punto con nivel mas alto de calidad
   double level=0;
   for(int i=0; i<nc; i++)
     for(int j=0; j<nr; j++){    
       if(level<qmap[i][j]){
         level=qmap[i][j];
         i_init=i;
         j_init=j;
         break;
       }
     }
   _punto=new Punto(i_init,j_init);
   _puntos.push_back(_punto);
   _colas[(int)qmap[i_init][j_init]].push_back(_punto);
   _caminado[i_init][j_init]=1;
}

void Seguidor::cargaVecinos()
{
  int i=_punto->c;
  int j=_punto->r;
  int nr=_I.rows();
  int nc=_I.cols();
  double** caminado=_caminado.getMat();
  double** qmap=_qmap.getMat();
  Punto* ptmp=NULL;
  
  if(i-1>=0)
    if(caminado[i-1][j]==0){
      ptmp=new Punto(i-1,j);
      _puntos.push_back(ptmp);
      _colas[(int)qmap[i-1][j]].push_back(ptmp);
      caminado[i-1][j]=1;
    }
  if(i-1>=0 && j-1>=0)
    if(caminado[i-1][j-1]==0){
      ptmp=new Punto(i-1,j-1);
      _puntos.push_back(ptmp);
      _colas[(int)qmap[i-1][j-1]].push_back(ptmp);
      caminado[i-1][j-1]=1;
    }
  if(j-1>=0)
    if(caminado[i][j-1]==0){
      ptmp=new Punto(i,j-1);
      _puntos.push_back(ptmp);
      _colas[(int)qmap[i][j-1]].push_back(ptmp);
      caminado[i][j-1]=1;
    }
  if(i+1<nc && j-1>=0)
    if(caminado[i+1][j-1]==0){
      ptmp=new Punto(i+1,j-1);
      _puntos.push_back(ptmp);
      _colas[(int)qmap[i+1][j-1]].push_back(ptmp);
      caminado[i+1][j-1]=1;
    }
  if(i+1<nc)
    if(caminado[i+1][j]==0){
      ptmp=new Punto(i+1,j);
      _puntos.push_back(ptmp);
      _colas[(int)qmap[i+1][j]].push_back(ptmp);
      caminado[i+1][j]=1;
    }
  if(i+1<nc && j+1<nr)
    if(caminado[i+1][j+1]==0){
      ptmp=new Punto(i+1,j+1);
      _puntos.push_back(ptmp);
      _colas[(int)qmap[i+1][j+1]].push_back(ptmp);
      caminado[i+1][j+1]=1;
    }
  if(j+1<nr)
    if(caminado[i][j+1]==0){
      ptmp=new Punto(i,j+1);
      _puntos.push_back(ptmp);
      _colas[(int)qmap[i][j+1]].push_back(ptmp);
      caminado[i][j+1]=1;
    }
  if(i-1>=0 && j+1<nr)
    if(caminado[i-1][j+1]==0){
      ptmp=new Punto(i-1,j+1);
      _puntos.push_back(ptmp);
      _colas[(int)qmap[i-1][j+1]].push_back(ptmp);
      caminado[i-1][j+1]=1;
    }

}

Seguidor::Seguidor(const Matrix& I,int levels)
{
  _I=I;
  _colas=new std::list<Punto*>[(int)levels];
  _m.resize(_I.rows(),_I.cols());
  _caminado.resize(_I.rows(),_I.cols());
  _qmap.resize(_I.rows(),_I.cols());
  _levels=levels;
  _m=0;
  _caminado=0;
  _qmap=0;
  _punto=NULL;
  //la variable _punto se inicializa en set_inicio()     
  //Primero calculamos el mapa de calidad por niveles
  calcQualityMap(levels);
  //Finalmente establecemos el punto inicial
  set_inicio();
  cargaVecinos();
}
Seguidor::Seguidor(const Matrix& I,int r, int c, int levels)
{
  _I=I;
  _colas=new std::list<Punto*>[(int)levels];
  _m.resize(_I.rows(),_I.cols());
  _caminado.resize(_I.rows(),_I.cols());
  _qmap.resize(_I.rows(),_I.cols());
  _levels=levels;
  _m=0;
  _caminado=0;
  _qmap=0;
  _punto=NULL;
  //la variable _punto se inicializa en set_inicio()     
  //Primero calculamos el mapa de calidad por niveles
  calcQualityMap(levels);
  //Finalmente establecemos el punto inicial
  _punto=new Punto(c,r);
  _puntos.push_back(_punto);
  _colas[(int)_qmap[c][r]].push_back(_punto);
  _caminado[c][r]=1;
  cargaVecinos();
}

int Seguidor::get_c()
{
  if(_punto!=NULL)
    return _punto->c;
  return -1;
}

int Seguidor::get_r()
{
  if(_punto!=NULL)
    return _punto->r;
  return -1;
}

Matrix Seguidor::get_qmap()
{
  return _qmap;
}

bool Seguidor::siguiente()
{
  std::list<Punto*>* cola=NULL;
  for(int i=_levels-1; i>=0; i--)
    if(_colas[i].begin()!=_colas[i].end()){
      cola=_colas+i;
      break;
    }
  if(cola!=NULL){
    _punto=cola->front();
    cola->pop_front();
    cargaVecinos();
    return true;
  }
  _punto=NULL;
  return false;
}

void Seguidor::setQMap(Matrix& qmap)
{
  _qmap=qmap;
  double** map=_qmap.getMat();
        
  double a,b;
  a=map[0][0];
  b=a;
  for(int i=0; i<_qmap.cols(); i++)
    for(int j=0; j<_qmap.rows(); j++){
      a=(a>map[i][j])? map[i][j]:a;
      b=(b<map[i][j])? map[i][j]:b;
    }
  int levels=_levels;
  double step=(b-a)/levels;
  for(int i=0; i<_qmap.cols(); i++)
    for(int j=0; j<_qmap.rows(); j++){
      for(int l=0; l<levels; l++)
        if(map[i][j]>=(a+l*step) && map[i][j]<=((l+1)*step))
          map[i][j]=l;
    }
}

Seguidor::~Seguidor()
{
  std::list<Punto*>::iterator j;
  
  int n=0, N=_I.rows()*_I.cols();
  for(j=_puntos.begin(); j!=_puntos.end() && n<N; j++, n++){
    delete *j;
  }
 
  delete []_colas;
}
}
