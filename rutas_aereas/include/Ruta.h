#ifndef _RUTA__H
#define _RUTA__H
#include <list>
#include "Punto.h"
#include <string>
using namespace std;
class Ruta{
    private:
      list<Punto> puntos;
      string code;
    public:
        Ruta(){
	  code="";
	}
	void Insertar(const Punto & n){
	    puntos.insert(puntos.end(),n);
	}
	void Borrar(const Punto &n){
	    
	    puntos.remove(n);
	}
	string GetCode()const{
	  return code;
	}
	void SetCode(const string & code){
	    this->code =code;
	}
	bool operator==(const Ruta &R)const{
	    return code==R.code;
	}
	bool operator<(const Ruta &R)const{
	    return code<R.code;
	}    
	class const_iterator;
	class iterator{
	private:
	    list<Punto>::iterator p;
	public:
	    iterator(){}
	    iterator & operator ++(){
		++p;
		return * this;
	    }
	    
	    iterator & operator --(){
		--p;
		return * this;
	    }
	    bool operator ==(const iterator  & it){
		return it.p ==p;
	    }
	    bool operator !=(const iterator  & it){
		return it.p !=p;
	    }
	     Punto & operator*(){
		  return *p;
	    }
	    friend class Ruta;
	    friend class const_iterator;
	};    
	class const_iterator{
	private:
	    list<Punto>::const_iterator p;
	public:
	    const_iterator(){}
	    const_iterator(const iterator & it){
	      p=it.p;
	      
	    }
	    const_iterator & operator=(const iterator & it){
		p=it.p;
		return *this;
	    }	
	    const_iterator & operator ++(){
		++p;
		return * this;
	    }
	    
	    const_iterator & operator --(){
		--p;
		return * this;
	    }
	    bool operator ==(const const_iterator  & it){
		return it.p ==p;
	    }
	    bool operator !=(const const_iterator  & it){
		return it.p !=p;
	    }
	    const Punto &operator*()const {
		  return *p;
	    }
	    friend class Ruta;
	   
	};
	iterator begin(){
	  iterator it;
	  it.p = puntos.begin();
	  return it;
	}  
	
	const_iterator begin()const{
	  const_iterator it;
	  it.p = puntos.begin();
	  return it;
	}
	iterator end(){
	  iterator it;
	  it.p = puntos.end();
	  return it;
	}
	const_iterator end()const{
	  const_iterator it;
	  it.p = puntos.end();
	  return it;
	}
	iterator find(const Punto &p){
	    iterator it;
	    list<Punto>::iterator i;
	    for (i=puntos.begin(); i!=puntos.end() && !(*i==p);++i);
	    it.p=i;
	    return it;
	}
	friend istream & operator>>(istream & is, Ruta & R){
	      Ruta rlocal;
	      
	      is>>rlocal.code; //leemos el code
	      int n;
	      is>>n;// el numero de puntos de la ruta
	      int i;
	      for (i=0;i<n && is;i++){
		QuitaSeparadores(is);
		Punto p;
		  is>>p;
		  rlocal.Insertar(p);
		 
	      }
	      R=rlocal;
	      return is;
	}
	friend ostream & operator<<(ostream & os, const Ruta &R){
	    os<<R.code<<'\t'<<R.puntos.size()<<'\t';
	    Ruta::const_iterator it;
	    for (it=R.begin(); it!=R.end(); ++it){
	        
		os<<*it<<"\t";
	    }
	    return os;
	}
};
#endif
		  
	