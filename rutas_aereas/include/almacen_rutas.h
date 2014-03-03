#ifndef _ALMACEN_RUTAS_
#define _ALMACEN_RUTAS_
#include <map>
#include <utility>
#include "Ruta.h"


class Almacen_Rutas{
  private:
    map<string,Ruta> rutas; //codigo de ruta y ruta
    multimap<Punto,string> puntos_enrutas; // por cada punto en las rutas en las que aparece

  public:
    Almacen_Rutas(){}
    void Insertar(const Ruta & R){
      rutas.insert(pair<string,Ruta>(R.GetCode(),R));
      Ruta::const_iterator it;
      for (it=R.begin(); it!=R.end();++it){
	puntos_enrutas.insert(pair<Punto,string>(*it,R.GetCode()));
      }
    }
    void Borrar(const Ruta &R){
       pair<multimap<Punto,string>::iterator ,multimap<Punto,string>::iterator>ret;
       Ruta::const_iterator it;
  
  
       for (it=R.begin(); it!=R.end();++it){
	       ret= puntos_enrutas.equal_range(*it);
	      multimap<Punto,string>::iterator it2=ret.first;
	       while (it2!=ret.second)
		if ((*it2).second==R.GetCode()){
		    puntos_enrutas.erase(it2);
		    ret= puntos_enrutas.equal_range(*it);
		    it2=ret.first;
		}
	        else ++it2;

	 
      }
      rutas.erase(R.GetCode());
    }
    
    Ruta GetRuta(const string & a){
	  return rutas[a];
    }
    
    Almacen_Rutas GetRutas_Con_Punto(const Punto &p){
      Almacen_Rutas ar;
      pair<multimap<Punto,string>::iterator ,multimap<Punto,string>::iterator>ret;
      ret= puntos_enrutas.equal_range(p);
      multimap<Punto,string>::iterator it;
      for (it=ret.first;it!=ret.second;++it){
	  ar.Insertar(GetRuta((*it).second));
      }
      return ar;
    }
    
    string GetDescripcion_point(Punto p){
	if (puntos_enrutas.find(p)!=puntos_enrutas.end()){
	    cout<<"Punto encontrado"<<endl;
	    pair<multimap<Punto,string>::iterator ,multimap<Punto,string>::iterator>ret;
	    ret= puntos_enrutas.equal_range(p);
	    Ruta r = GetRuta((*(ret.first)).second);
	    Punto aux= *(r.find(p));
	    return aux.GetDescripcion();
	}
	else return "";
    }	
        
	
    
    class const_iterator;
	class iterator{
	private:
	    map<string,Ruta>::iterator p;
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
	    Ruta & operator*(){
		  return (*p).second;
	    }
	    friend class Almacen_Rutas;
	    friend class const_iterator;
	};    
	class const_iterator{
	private:
	    map<string,Ruta>::const_iterator p;
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
	    const Ruta &operator*()const {
		  return (*p).second;
	    }
	    friend class Almacen_Rutas;
	   
	};
	iterator begin(){
	  iterator it;
	  it.p = rutas.begin();
	  return it;
	}  
	
	const_iterator begin()const{
	  const_iterator it;
	  it.p = rutas.begin();
	  return it;
	}
	iterator end(){
	  iterator it;
	  it.p = rutas.end();
	  return it;
	}
	const_iterator end()const{
	  const_iterator it;
	  it.p = rutas.end();
	  return it;
	}
	friend istream & operator>>(istream & is, Almacen_Rutas & AR){
	      Almacen_Rutas arlocal;
	      string a;
	      is>>a;
	      if (a=="#Rutas"){
		Ruta rlocal;
		bool salir=false;
		QuitaSeparadores(is);
		while (!salir){
		  if (is.peek()!='#' && is){
		      is>>rlocal;
		      arlocal.Insertar(rlocal);
		      QuitaSeparadores(is);
		  }
		  else salir=true;
		}
		if (is.peek()=='#'){
		    is>>a;
		    if (a=="#Puntos_de_Interes"){
			cout<<"Entramos en la zona de puntos de interes"<<endl;
			QuitaSeparadores(is);
			Punto p;
			while (is>>p){
			   map<string,Ruta>::iterator it;
			   for (it=arlocal.rutas.begin(); it!=arlocal.rutas.end();++it){
			    Ruta::iterator i = ((*it).second).find(p);
			    if (i!=((*it).second).end()){
			      string aux;
			      getline(is,aux);
			      (*i).SetDescripcion(aux);
			      cout<<"Poniendo descripcion "<<aux<<endl;
			    }  
			    
			   }
			   
			}
		    }
		}	   
	      }
	      AR=arlocal;
	      return is;
	}
	friend ostream & operator<<(ostream & os, const Almacen_Rutas &R){
	    Almacen_Rutas::const_iterator it;
	    for (it=R.begin(); it!=R.end(); ++it){
	        os<<(*it)<<"\t";
	    }
	    return os;
	}
	
      
      
};
#endif