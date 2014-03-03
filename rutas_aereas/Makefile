SRC = src
INC = include
OBJ = obj
BIN = bin
LIB = lib
CXX = g++
CPPFLAGS = -Wall -g  -I$(INC) -c


	
#************* Definir objetivos principales *******
all:$(BIN)/rutaAerea

# ************ Compilacion de archivos **********************

$(OBJ)/rutaAerea.o:$(SRC)/ruta_aerea.cpp 
	$(CXX) $(CPPFLAGS) $(SRC)/ruta_aerea.cpp -o $(OBJ)/rutaAerea.o

$(OBJ)/pruebaRota.o:$(SRC)/pruebarotacion.cpp 
	$(CXX) $(CPPFLAGS) $(SRC)/pruebarotacion.cpp -o $(OBJ)/pruebaRota.o
	
$(OBJ)/pruebaPega.o:$(SRC)/pruebapegado.cpp 
	$(CXX) $(CPPFLAGS) $(SRC)/pruebapegado.cpp -o $(OBJ)/pruebaPega.o

$(OBJ)/imagen.o:$(SRC)/imagen.cpp $(INC)/imagen.h $(OBJ)/imagenES.o
	$(CXX) $(CPPFLAGS) $(SRC)/imagen.cpp -o $(OBJ)/imagen.o 

$(OBJ)/imagenES.o:$(SRC)/imagenES.cpp $(INC)/imagenES.h
	$(CXX) $(CPPFLAGS) $(SRC)/imagenES.cpp -o $(OBJ)/imagenES.o
	
# *********** Enlazado de archivos y creacion de ejecutables****

$(BIN)/pruebaRota:$(OBJ)/pruebaRota.o $(OBJ)/imagen.o $(OBJ)/imagenES.o
	$(CXX) $(OBJ)/pruebaRota.o $(OBJ)/imagen.o $(OBJ)/imagenES.o  -o $(BIN)/pruebaRota
	
$(BIN)/pruebaPega:$(OBJ)/pruebaPega.o $(OBJ)/imagen.o $(OBJ)/imagenES.o
	$(CXX) $(OBJ)/pruebaPega.o $(OBJ)/imagen.o $(OBJ)/imagenES.o  -o $(BIN)/pruebaPega
	
$(BIN)/rutaAerea:$(OBJ)/rutaAerea.o $(OBJ)/imagen.o $(OBJ)/imagenES.o
	$(CXX) $(OBJ)/rutaAerea.o $(OBJ)/imagen.o $(OBJ)/imagenES.o  -o $(BIN)/rutaAerea

# ************ Generación de documentación ******************
documentacion:
	doxygen doc/doxys/Doxyfile
	
# ************ Limpieza ************
clean :
	-rm $(OBJ)/* $(SRC)/*~ $(INC)/*~ ./*~

mrproper : clean
	-rm $(BIN)/* doc/html/*
