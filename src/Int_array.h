//Clase para definir el tamaño del array
//se puede hacer con simples ints pero así el tamano del array no sera absurdamente grande
//Se inicializa al numero de lineas que tenga el archivo requerido

#ifndef Int_array_h
#define Int_array_h

#include <string>
using namespace std;

class Int_array
{
  //Variables
public:
  int value;

  //Inicializador
  Int_array(string filename);
};


#endif
