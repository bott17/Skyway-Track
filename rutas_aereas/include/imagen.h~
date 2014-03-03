#ifndef __IMAGEN__H
#define __IMAGEN_H
#include <string>
#include "imagenES.h"
using namespace std;

enum Tipo_Pegado {OPACO, BLENDING};

struct Pixel{
    unsigned char r,g,b;
    unsigned char transparencia;
};

class Imagen{
    
    private:
    Pixel **datos; //donde se almacena la información de la imagen. Otra posible representación
    //Pixel*datos
    int nf,nc;
    
    public:
    Imagen(); //contructor por defecto
    Imagen(const Imagen &I); //constructor de copia
    Imagen (int nf,int nc); //constructor con parámetros
    ~Imagen();
    Imagen & operator =(const Imagen & I);
    int GetFilas()const; //devuelve el numero de filas nf
    int GetCols()const; //devuelve el numero de filas nc
    Pixel & operator()(int i,int j); // devuelve el pixel en la posicion i,j
    const Pixel & operator()(int i,int j)const; // devuelve el pixel en la posicion i,j
    void EscribirImagen(const char * nombre); //escribe en disco la imagen
    void LeerImagen(const char *nimagen,const string& nombre_mascara="");//Leer una imagen de disco.
    void PutImagen(int posi,int posj, const Imagen &I,Tipo_Pegado tippegado);
    
};
#endif