//Implementacion de la clase

#include <string>
#include "Int_array.h"
#include "textmisc.h"
using namespace std;

//Inicializador
Int_array::Int_array(string filename)
{
  Int_array::value = GetLines(string);
}
