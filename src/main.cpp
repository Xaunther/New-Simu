//Dummy main, para testear compilacion
#include <string>
#include <iostream>
#include "alineacion.h"
#include "equipo.h"
#include "position.h"
#include "Simu.h"
using namespace std;

int main(void)
{
  string nombre;
  cin >> nombre;
  nombre = "../test/"+nombre+"sht.txt";
  alineacion ali(nombre);
  equipo eq(nombre);
  return 0;
}
