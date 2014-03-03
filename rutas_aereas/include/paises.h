#include "pais.h"
#include <set>
using namespace std;
class Paises{
  private:
    set<Pais> datos;
  public:
	Paises(){}
	void Insertar(const Pais &P){
	    datos.insert(P);
	}
	void Borrar(const Pais &P){
	    datos.erase(P);
	}
	
	/**
	* Para poder iterar sobre la estructura de datos utilizada en esta clase, nos definimos un iterador que haga de interfaz sobre
	* ella ya que es private
	*/
	class const_iterator; //Debes ponerlo aqui para que no de error al compilar
	class iterator{
	    private:
		set<Pais>::iterator p;
	    public:
		
		iterator begin();
		iterator end();
		iterator find(const Pais &p);
		iterator find(const Punto &p);
		friend istream & operator>>(istream & is, Paises & R);
		friend ostream & operator<<(ostream & os, const Paises &R);
		bool operator !=(const iterator  & it){
		    return it.p != p;
		}
		set<Pais>::iterator operator ++(){
		    return ++p;
		}
		
		Pais operator *(){
		    return *p;
		}
				
		friend class Paises;
		friend class const_iterator;
	};
	
	class const_iterator{
	    private:
		set<Pais>::const_iterator p;
	    public:
		
		const_iterator begin()const;
		const_iterator end()const;
		friend istream & operator>>(istream & is, Paises & R);
		friend ostream & operator<<(ostream & os, const Paises &R);
		bool operator!=(const_iterator it) const{
		    return it.p != p;
		}
		set<Pais>::const_iterator operator ++(){
			return ++p;
		}
		Pais operator *(){
		    return *p;
		}
		
		
		friend class Paises;
	};
	
	
	
	iterator begin(){
	  iterator it;
	  it.p = datos.begin();
	  return it;
	}  
	
	const_iterator begin()const{
	  const_iterator it;
	  //it.p = datos.cbegin(); //Solo para compiladores posteriores al 2011
	  it.p = datos.begin();
	  return it;
	}
	
	iterator end(){
	  iterator it;
	  it.p = datos.end();
	  return it;
	}
	const_iterator end()const{
	  const_iterator it;
	  //it.p = datos.cend();//Solo para compiladores posteriores al 2011
	  it.p = datos.end();
	  return it;
	}
	
	iterator find(const Pais &p){
	    iterator it;
	    set<Pais>::iterator i;
	    for (i=datos.begin(); i!=datos.end() && !((*i)==p);++i);
		it.p=i;
	    return it;
	}
	iterator find(const Punto &p){
	    iterator it;
	    set<Pais>::iterator i;
	    for (i=datos.begin(); i!=datos.end() && !((*i).GetPunto()==p);++i);
		it.p=i;
	    return it;
	}
	
	friend istream & operator>>(istream & is, Paises & R){
	      Paises rlocal;
	      //leemos el comentario
	      if (is.peek()=='#'){
		string a;
		getline(is,a);
	      }	
	      
	      Pais P;
	      while (is>>P){
		  rlocal.Insertar(P);
		  
	      }
	      R=rlocal;
	      return is;
	}
	
	//TODO repasar el uso de estos punteros
	friend ostream & operator<<(ostream & os, const Paises &R){
	  
	    Paises::const_iterator it;
	    for (it=R.begin(); it!=R.end(); ++it){
		os<<*it<<"\t";
	    }
	    return os;
	}
};