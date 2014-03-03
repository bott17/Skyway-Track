#ifndef __PUNTO__H
#define __PUNTO__H
#include <ios>
#include <iostream>
#include <string>
using namespace std;
void QuitaSeparadores(istream &is){
  while (is &&(is.peek()=='\t' ||is.peek()==' ' || is.peek()=='\n'))
    is.get();
}
class Punto{
  private:
    double latitud,longitud;
    string descripcion;
    
  public:
    Punto(){
	latitud=longitud=0;
	descripcion="";
    }
    Punto(double l,double L, const string & d):latitud(l),longitud(L),descripcion(d){}
	
    string GetDescripcion()const{
	return descripcion;
    }
    double GetLatitud()const{
	return latitud;
    }
    double GetLongitud()const{
	return longitud;
    }
    void SetDescripcion(string d){
	descripcion=d;
    }
    void SetLatitud(double l){
	latitud=l;
    }
    void SetLongitud(double l){
	longitud=l;
    }
    bool operator<(const Punto &p)const{
	  return (p.latitud<latitud) || (p.latitud==latitud && p.longitud<longitud);
    }
    bool operator==(const Punto &p)const{
	  return (p.latitud==latitud &&  p.longitud==longitud);
    }
    friend istream & operator>>(istream & is, Punto &p){
	char c;
	is>>c;
	if (c!='('){
	  is.setstate(ios::failbit);
	  return is;
	}  
	is>>p.latitud>>c;
	if (c!=','){
	  is.setstate(ios::failbit);
	  return is;
	}
	is>>p.longitud>>c;
	if (c!=')'){
	  is.setstate(ios::failbit);
	  return is;
	}
	p.descripcion="";
	return is;
    }
    friend ostream & operator<<(ostream & os, const Punto &p){
      os<<"("<<p.latitud<<","<<p.longitud<<")\t"<<p.descripcion;
      return os;
    }
};
#endif
	    
    