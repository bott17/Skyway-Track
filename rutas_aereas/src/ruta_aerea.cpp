/**/
#include <stdio.h>  
#include<stdlib.h>
#include<errno.h>
/***/

#include "imagen.h"
#include "paises.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include "almacen_rutas.h"
#include <string.h>

#define rutaSalida "rutaFINAL"

using namespace std;

void mostrarAlmacenRutas(Almacen_Rutas Ar, Paises paises);
void Pintar( int f1,int f2,int c1,int c2,Imagen &I, const Imagen &avion,int mindisf,int mindisc);
Imagen Rota(const Imagen & Io,double angulo);
int calcularPunto(int flag, double valor, const Imagen &img);
Imagen colocaBanderas(Imagen IMAP, Paises paises, Tipo_Pegado tp, Ruta ruta, string rutaBandera);

/**
 * Muestra por pantalla algunos datos relevantes de los integrantes de la practica
 */
void creditosIniciales(){
    system("clear");
    cout<<"\n*********************** Práctica final: Rutas Aéreas ***********************"<<endl;
    cout<<"\tEstructuras de Datos. Titulacion: GII Profesor: Joaquin Fdez-Valdivia  Grupo: Martes 16:30-17:30"<<endl;
    cout<<"Intregrantes: Álvaro Martínez Campos \tCarlos Sánchez Martínez"<<endl;
    cout<<"***********************\t***********************"<<endl;
    
    cout<<"Presionar cualquier tecla para continuar...."<<endl;
    cin.get();
}

int main(int argc, char * argv[]){
    
    if(argc !=7){
	
	cout<<"Parametros del programa: "<<endl;
	cout<<"1-Fichero donde se ecnuentra los paises"<<endl;
	cout<<"2-Mapa"<<endl;
	cout<<"3-Directorio de las banderas"<<endl;
	cout<<"4-Almacen de rutas"<<endl;
	cout<<"5-Tipo de avion"<<endl;
	cout<<"6-Mascara del avion"<<endl;
	
	return 0;
    }
    creditosIniciales();
    
    ifstream f(argv[4]);
    if (!f){
	cout<<"No puedo abrir el fichero "<<argv[4]<<endl;
	return 0;
    }
	
    ///Almacen de rutas
    Almacen_Rutas Ar;
    f>>Ar; //leemos las rutas y los puntos de interes	
    
    ///Paises
    ifstream g(argv[1]);
    if (!g){
	cout<<"No puedo abrir el fichero "<<argv[1]<<endl;
	return 0;
    }
    Paises paises;
    Pais p;
    g>>paises;
    //cout<<paises<<endl;
	
    //Para mostrar el almacen de rutas
    mostrarAlmacenRutas(Ar, paises);
    //Seleccion de la ruta por parte del usuario
    cout<<"Selecciona el codigo de la ruta (formato R1, R2..): " <<endl;
    string rutaSeleccionada;
    cin>>rutaSeleccionada;
    
	
    Ruta rutaActual = Ar.GetRuta(rutaSeleccionada);
    
    ///Obtencion del avion
    Imagen IAV;
    IAV.LeerImagen(argv[5],argv[6]);
    ///Obtencion del mapa
    Imagen IMAP;
    IMAP.LeerImagen(argv[2]);
    
    //Definimos el tipo de pegado
    Tipo_Pegado tp=OPACO;
    
    
    //IMAP = colocaBanderas(IMAP, paises, tp, rutaActual);
    
    
    /// Pintar ruta
    tp=BLENDING;
    for(Ruta::iterator it = rutaActual.begin(), itDestino; it!=rutaActual.end(); ++it){	
	
	itDestino = it;
	++itDestino;
	if(itDestino != rutaActual.end()){
	    int f1 = (*it).GetLatitud(), c1 =(*it).GetLongitud();
	    
	    int f2 = (*itDestino).GetLatitud(), c2 =(*itDestino).GetLongitud();
	    
	    Pintar((calcularPunto(0,f1, IMAP))-(IAV.GetFilas()/2), (calcularPunto(0,f2, IMAP))-(IAV.GetFilas()/2), //Centrado de la imagen 
		(calcularPunto(1,c1, IMAP))-(IAV.GetCols()/2),(calcularPunto(1,c2,IMAP))-(IAV.GetCols()/2), 
		IMAP, IAV, 50, 50);
	    
	}

    }
    
    /// Pintar banderas
    IMAP = colocaBanderas(IMAP, paises, tp, rutaActual, argv[3]);
    
    
    IMAP.EscribirImagen(rutaSalida);
	
    //Abre la imagen de salida
    if(system("eog rutaFINAL &")){}
    else exit (EXIT_FAILURE);
    
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
Imagen colocaBanderas(Imagen IMAP, Paises paises, Tipo_Pegado tp, Ruta rutaActual, string rutaBandera){
    /// Dibujando las banderas en el mapa
    string ruta = rutaBandera;
    Imagen bandera;
    for(Paises::iterator itP = paises.begin(); itP!=paises.end();++itP){
	
	Pais pais = *itP;
	bool continuar = true;
	for(Ruta::iterator itR = rutaActual.begin(); itR!= rutaActual.end() && continuar; ++itR){
	    
	    Punto punto = *itR;
	    
	    if(pais.GetPunto() == punto){
	    
	    
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
	    
	
	//TODO añadido, necesario para usar putImagen
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
 * @return Posicion del punto dentro del mapa
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
void mostrarAlmacenRutas(Almacen_Rutas Ar, Paises paises){
    Almacen_Rutas::iterator it;
	for (it=Ar.begin(); it!=Ar.end(); ++it){
	    Ruta::iterator it_r;
	    int contador =0;
	    //por cada ruta su codigo y puntos
	    cout<<"Codigo de ruta: "<<(*it).GetCode()<<":"<<endl; 
	    for (it_r=(*it).begin(); it_r!=(*it).end(); ++it_r, contador++){
		
		cout<<(*it_r)<<" ";
		for(Paises::iterator it_Pa = paises.begin(); it_Pa != paises.end(); ++it_Pa){
		    if((*it_Pa).GetPunto() == (*it_r))
			cout<<(*it_Pa).GetPais()<<endl;
		    }
	    }  
	    cout<<endl;
	}
}