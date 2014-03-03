#include "imagen.h"
#include <assert.h>
#include <iostream>



/**
 * Constructor por defecto
 */
Imagen::Imagen(){
    datos=NULL;
    nf = nc = 0;    
}
/**
 * Constructor de copia.
 * @brief Se hace una copia en profundidad
 */
Imagen::Imagen(const Imagen &I){
    nf = I.nf;
    nc = I.nc;
    datos=new Pixel*[nf];
    for (int i=0; i<nf; i++){
	   datos[i] = new Pixel[nc];
      for(int j=0; j<nc; j++){
          datos[i][j].r=I(i,j).r;
	       datos[i][j].g=I(i,j).g;
	       datos[i][j].b=I(i,j).b;
	       datos[i][j].transparencia=I(i,j).transparencia;
      }
     }
    
}

/**
 * Constructor por parámetros
 */
Imagen::Imagen(int f,int c){
    nf = f;
    nc = c;
    datos = new Pixel*[nf];
    for (int i=0;i<nf;i++){
	   datos[i]=new Pixel[nc];
	   for (int j=0;j<nc;j++){
	       datos[i][j].r=255;
	       datos[i][j].g=255;
	       datos[i][j].b=255;
	       datos[i][j].transparencia=255;
	   }
    } 
}


/**
 * Destructor
 * */
Imagen::~Imagen(){
    for(int i=0; i<nf; i++)
	delete datos[i];
    delete datos;
}

/**
 * Sobreescritura del operador de asignación
 */
Imagen& Imagen:: operator =(const Imagen & I){
    
    for(int i=0; i<nf; i++)
	     delete datos[i];
    delete datos;    
    nf = I.nf;
    nc = I.nc;

    datos = new Pixel* [nf];
    for (int i=0; i<nf; i++){
	   datos[i] = new Pixel[nc];
      for(int j=0; j<nc; j++){
          datos[i][j].r=I(i,j).r;
	       datos[i][j].g=I(i,j).g;
	       datos[i][j].b=I(i,j).b;
	       datos[i][j].transparencia=I(i,j).transparencia;
      }
     }
    
    
    
    return *this;
}

int Imagen::GetFilas()const{ return nf;}
int Imagen::GetCols()const{return nc;}

/**
 * Devuelve el pixel en la posicion i,j
 */
Pixel & Imagen::operator()(int i,int j){
    return datos[i][j];
}

/**
 * Devuelve el pixel en la posicion i,j
 */
const Pixel & Imagen::operator()(int i,int j)const{
  assert(i>=0 && i<nf && j>=0 && j<nc);
  return datos[i][j];
}


void Imagen::EscribirImagen(const char * nombre){
      unsigned char * aux = new unsigned char [nf*nc*3];
      unsigned char * m = new unsigned char [nf*nc];
      
      int total = nf*nc*3;
      for (int i=0;i<total;i+=3){
	    int posi = i /(nc*3);
	   int posj = (i%(nc*3))/3;
	    
	    aux[i]=datos[posi][posj].r;
	    aux[i+1]=datos[posi][posj].g;
	    aux[i+2]=datos[posi][posj].b;
	    m[i/3]=datos[posi][posj].transparencia;
	     
	 }    
	
      if (!EscribirImagenPPM(nombre, aux,nf,nc)){
	  cerr<<"Ha habido un problema en la escritura de "<<nombre<<endl;
      }	  
      delete[]aux;
      string n_aux = "mascara_";
      n_aux =n_aux+nombre;
      std::size_t found =n_aux.find(".ppm");
      if (found!=std::string::npos){
	n_aux =n_aux.substr(0,found);
      }
      n_aux =n_aux+".pgm";

      
      if (!EscribirImagenPGM (n_aux.c_str(), m,nf,nc)){
	  cerr<<"Ha habido un problema en la escritura de "<<n_aux<<endl;
      }	    
      delete []m;
  
}


void Imagen::LeerImagen(const char * nombre,const string &nombremascara){
      int f,c;
      unsigned char * aux,*aux_mask ;
      
      LeerTipoImagen(nombre, f, c);
      aux = new unsigned char [f*c*3];
      LeerImagenPPM (nombre, f,c,aux);
      if (nombremascara!=""){
	int fm,cm;
	LeerTipoImagen(nombremascara.c_str(), fm, cm);
	aux_mask = new unsigned char [fm*cm];
	LeerImagenPGM(nombremascara.c_str(), fm,cm,aux_mask);
      }
      else{
	aux_mask=0;
      }	
      
      
      Imagen I(f,c);
      int total = f*c*3;
      for (int i=0;i<total;i+=3){
	   int posi = i /(c*3);
	   int posj = (i%(c*3))/3;
	//   cout<<posi<<" " <<posj<<endl;
	     I.datos[posi][posj].r=aux[i];
	     I.datos[posi][posj].g=aux[i+1];
	     I.datos[posi][posj].b=aux[i+2];
	     if (aux_mask!=0)
	      I.datos[posi][posj].transparencia=aux_mask[i/3];
	     else  
	       I.datos[posi][posj].transparencia=255;
	 }    
	  
      *this = I;   
      if (aux_mask!=0) delete[]aux_mask;
      delete []aux;
      
}

void Imagen::PutImagen(int posi,int posj, const Imagen &I,Tipo_Pegado tippegado){
    //assert(nf>=posi+I.nf && nc>=posj+I.nc);
    
    for (int i=0;i<I.nf;i++)
      for (int j=0;j<I.nc;j++)
	if (i+posi>=0 && i+posi<nf && j+posj>=0 && j+posj<nc){
	if (I.datos[i][j].transparencia!=0){
	  if (tippegado==OPACO)
	    datos[i+posi][j+posj]=I.datos[i][j];
	  else{
	    datos[i+posi][j+posj].r=(datos[i+posi][j+posj].r+I.datos[i][j].r)/2;
	    datos[i+posi][j+posj].g=(datos[i+posi][j+posj].r+I.datos[i][j].g)/2;
	    datos[i+posi][j+posj].b=(datos[i+posi][j+posj].r+I.datos[i][j].b)/2;
	  }  
	  
	}  
	}	
	    
}
