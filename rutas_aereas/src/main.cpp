#include "paises.h"
#include "imagen.h"
#include "Ruta.h"
#include "almacen_rutas.h"
#include <iostream>
#include <fstream>

using namespace std;
/*
int main(int argc, char * argv[]){

  Imagen I;

  I.LeerImagen(argv[1]);
  
  I.EscribirImagen("prueba.ppm");
  
}
*/


int main(int argc, char * argv[]){
    if (argc!=7){
	cout<<"Los parametros son:"<<endl;
	cout<<"1.-Fichero con la informacion de los paises"<<endl;
	cout<<"2.-Nombre de la imagen con el mapa del mundo"<<endl;
	cout<<"3.-Directorio con las banderas"<<endl;
	cout<<"4.-Fichero con el almacen de rutas"<<endl;
	cout<<"5.- Nombre de la imagen con el avion"<<endl;
	cout<<"6.- Nombre de la imagen de la mascara del avion"<<endl;
	
	return 0;
    } 
    
    Paises Pses;
    ifstream f (argv[1]);
    f>>Pses;
    //cout<<Pses;
    Imagen I;
    I.LeerImagen(argv[2]);
    
    //Leemos los aviones 
    Imagen avion;
    avion.LeerImagen(argv[5],argv[6]);
    
    Almacen_Rutas Ar;
    f.close();
    f.open (argv[4]);
    f>>Ar;
    cout<<"Las rutas: "<<endl<<Ar;
    cout<<"Dime el codigo de una ruta"<<endl;
    string a;
    cin>>a;
    Ruta R=Ar.GetRuta(a);
    
}
