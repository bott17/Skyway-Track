#include "imagen.h"
#include "paises.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include "almacen_rutas.h"
#include <string.h>
#include <stdlib.h>

#define rutaAlmacenRutas "datos/almacen_rutas.txt"
#define rutaPaises "datos/paises.txt"
#define rutaIMG "datos/imagenes/aviones/avion1.ppm"
#define rutaMASC "datos/imagenes/aviones/mascara_avion1.pgm"
#define rutaMAPA "datos/imagenes/mapas/mapa1.ppm"
#define rutaBandera "datos/imagenes/banderas/"
#define rutaSalida "rutaFINAL"

using namespace std;

void mostrarAlmacenRutas(Almacen_Rutas Ar);
void Pintar( int f1,int f2,int c1,int c2,Imagen &I, const Imagen &avion,int mindisf,int mindisc);
Imagen Rota(const Imagen & Io,double angulo);
int calcularPunto(int flag, double valor, const Imagen &img);
Imagen colocaBanderas(Imagen IMAP, Paises paises, Tipo_Pegado tp, Ruta ruta);

int main(int argc, char * argv[]){
    
    if(argc !=2){
	
	cout<<"Parametros del programa: "<<endl;
	cout<<"1-Nombre de la ruta (R1, R2, R3, R4)"<<endl;
	
	return 0;
    }
    
    ifstream f(rutaAlmacenRutas);
    if (!f){
	cout<<"No puedo abrir el fichero "<<rutaAlmacenRutas<<endl;
	return 0;
    }
	
    ///Almacen de rutas
    Almacen_Rutas Ar;
    f>>Ar; //leemos las rutas y los puntos de interes	
	
    //Para mostrar el almacen de rutas
    //mostrarAlmacenRutas(Ar);
    ifstream g(rutaPaises);
    if (!g){
	cout<<"No puedo abrir el fichero "<<rutaAlmacenRutas<<endl;
	return 0;
    }
    
    ///Paises
    Paises paises;
    Pais p;
    g>>paises;
    
    //cout<<paises<<endl;
    
	
    //Utilizamos la ruta 1 por defecto
    Ruta rutaActual = Ar.GetRuta(argv[1]);
    
    ///Obtencion del avion
    Imagen IAV;
    IAV.LeerImagen(rutaIMG,rutaMASC);
    ///Obtencion del mapa
    Imagen IMAP;
    IMAP.LeerImagen(rutaMAPA);
    
    //Definimos el tipo de pegado
    Tipo_Pegado tp=OPACO;
    
    /// Pintar banderas
    IMAP = colocaBanderas(IMAP, paises, tp, rutaActual);
    
    
    /// Pintar ruta
    tp=BLENDING;
    for(Ruta::iterator it = rutaActual.begin(); it!=rutaActual.end(); ++it){	
	
	int f1 = (*it).GetLatitud(), c1 =(*it).GetLongitud();
	++it;
	int f2 = (*it).GetLatitud(), c2 =(*it).GetLongitud();
	
	
	Pintar((calcularPunto(0,f1, IMAP))-(IAV.GetFilas()/2), (calcularPunto(0,f2, IMAP))-(IAV.GetFilas()/2), //Centrado de la imagen 
	       (calcularPunto(1,c1, IMAP))-(IAV.GetCols()/2),(calcularPunto(1,c2,IMAP))-(IAV.GetCols()/2), 
	       IMAP, IAV, 100, 100);
	--it;
    }    
    
    
    
    IMAP.EscribirImagen(rutaSalida);
	
	
    
    return 0;
    
}

/**
 * Coloca las imagenes de las banderas de los paises que estan en la ruta seleccionada
 * @param IMAP Imagan de fondo para las banderas
 * @param paises Contenedor con todos los paises
 * @param tp Tipo de pegado
 * @param rutaActual Ruta usada
 * @return Imagen con las banderas colocadas
 */
Imagen colocaBanderas(Imagen IMAP, Paises paises, Tipo_Pegado tp, Ruta rutaActual){
    /// Dibujando las banderas en el mapa
    string ruta = rutaBandera;
    Imagen bandera;
    for(Paises::iterator itP = paises.begin(); itP!=paises.end();++itP){
	
	Pais pais = *itP;
	bool continuar = true;
	for(Ruta::iterator itR = rutaActual.begin(); itR!= rutaActual.end() && continuar; ++itR){
	    
	    Punto punto = *itR;
	    
	    if((pais.GetPunto().GetLatitud() == punto.GetLatitud()) && (pais.GetPunto().GetLongitud() == punto.GetLongitud())){
	    
	    
		char * extension = new char [ruta.length()+pais.GetBandera().length()+1];
		strcpy(extension, ruta.c_str());
		strcat(extension, pais.GetBandera().c_str());
		bandera.LeerImagen(extension);
		//primero filas, luego columnas
		//latitud = columnas, longitud=filas
		IMAP.PutImagen((calcularPunto(0,pais.GetPunto().GetLatitud(), IMAP))-(bandera.GetFilas()/2), //Centrado de la imagen
			    calcularPunto(1,pais.GetPunto().GetLongitud(), IMAP)-(bandera.GetCols()/2),
			    bandera, tp);
		
		continuar=false;
	    }
	}
    }
    
    return IMAP;
}



void Pintar( int f1,int f2,int c1,int c2,Imagen &I, const Imagen &avion,int mindisf,int mindisc){
      
      int fila,col;
      if (abs(f2-f1)>=mindisf || abs(c2-c1)>=mindisc){ 
	
	if (c1!=c2){
	    double a,b;
	    a= double(f2-f1)/double(c2-c1);
	    b= f1-a*c1;
	    col = (int)(c1+c2)/2;
	    fila = (int)rint(a*col+b);
	}  
	else{
	    col = c1;
	    fila = (f1+f2)/2;
	}  
	    
	
	//TODO
	Tipo_Pegado tp=OPACO;
	
	double angulo = atan2((f2-f1),(c2-c1));
	Imagen Irota=Rota(avion,angulo);
	
	I.PutImagen(fila,col,Irota, tp);
	
	angulo = atan2((f2-fila),(c2-col));
	Irota=Rota(avion,angulo);
	    
	I.PutImagen(f2,c2,Irota, tp);
	angulo = atan2((fila-f1),(col-c1));
	Irota=Rota(avion,angulo);
	    
	I.PutImagen(f1,c1,Irota, tp);
	 
      
    }
     
}


/** 
 * @brief Obtiene una nueva imagen que es la versión rotada de una imagen de entrada
 * @param Io: imagen de entrada
 * @param angulo: angulo a rotar la imagen de entrada. Debe estar en radianes.
 * @return Una nueva imagen que es la versión rotada de Io.
 * */

Imagen Rota(const Imagen & Io,double angulo){
    double rads=angulo;
    double coseno = cos(angulo);
    double seno = sin(angulo);
    //Para obtener las dimensiones de la imagen
    int rcorners[4],ccorners[4];
    int newimgrows,newimgcols;
    double new_row_min,new_col_min,new_row_max,new_col_max;
    double inter,inter1;
    rcorners[0]=0;
    rcorners[1]=0;
    ccorners[0]=0;
    ccorners[2]=0;
    rcorners[2]=Io.GetFilas()-1;
    rcorners[3]=Io.GetFilas()-1;
    ccorners[1]=Io.GetCols()-1;
    ccorners[3]=Io.GetCols()-1;
    new_row_min=0;
    new_col_min=0;
    new_row_max=0;
    new_col_max=0;
    newimgrows=0;
    newimgcols=0;
    for(int count=0;count<4;count++)
    {
	    inter=rcorners[count]*coseno+ccorners[count]*seno;
	   
	    if(inter<new_row_min)
		    new_row_min=inter;
	    if(inter>new_row_max)
		    new_row_max=inter;
	    inter1=-rcorners[count]*seno+ccorners[count]*coseno;
	   
	    if(inter1<new_col_min)
		    new_col_min=inter1;	
	    if(inter1>new_col_max)
		    new_col_max=inter1;
   }
    
    newimgrows=(unsigned)ceil((double)new_row_max-new_row_min);
    newimgcols=(unsigned)ceil((double)new_col_max-new_col_min);
   
    Imagen Iout(newimgrows,newimgcols);
    for(int rows=0;rows<newimgrows;rows++)
      {
	for(int cols=0;cols<newimgcols;cols++)
	    {
	    float oldrowcos=((float)rows+new_row_min)*cos(-rads);
	    float oldrowsin=((float)rows+new_row_min)*sin(-rads);
	    float oldcolcos=((float)cols+new_col_min)*cos(-rads);
	    float oldcolsin=((float)cols+new_col_min)*sin(-rads);
	    float old_row=oldrowcos+oldcolsin;
	    float old_col=-oldrowsin+oldcolcos;
	    old_row=ceil((double)old_row);
	    old_col=ceil((double)old_col);
	
	    if((old_row>=0)&&(old_row<Io.GetFilas())&&
		(old_col>=0)&&(old_col<Io.GetCols())){
		    Iout(rows,cols)=Io(old_row,old_col);    
	    }
	    else{
		    Iout(rows,cols).r=Iout(rows,cols).g=Iout(rows,cols).b=255;
		    //TODO añadido
		    Iout(rows,cols).transparencia=0;
	    }
	    
	   
	}
    }
    return Iout;
	  
}


/**
 * Calcula el punto en el mapa
 * @param flag Indica si es 1-longitud,  0-latitud
 * @param img Imagen de fondo, necesaria para calcular la posicion del punto
 */
int calcularPunto(int flag, double valor, const Imagen &img){
    if(flag==1)
	return (img.GetCols()/360.0)*(180+valor);
    else
	return (img.GetFilas()/180.0)*(90-valor);
	
}

/**
 * Muestra por pantalla todas las rutas
 * @param Ar Almacen donde estan las rutas
 */
void mostrarAlmacenRutas(Almacen_Rutas Ar){
    Almacen_Rutas::iterator it;
	for (it=Ar.begin(); it!=Ar.end(); ++it){
	    Ruta::iterator it_r;
	    //por cada ruta su codigo y puntos
	    cout<<"Codigo: "<<(*it).GetCode()<<":"; 
	    for (it_r=(*it).begin(); it_r!=(*it).end(); ++it_r){
	    
	    cout<<(*it_r)<<" ";
	    }  
	    cout<<endl;
	}
}