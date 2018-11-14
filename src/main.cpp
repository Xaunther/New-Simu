//Dummy main, para testear compilacion
#include <string>
#include <iostream>
#include "alineacion.h"
#include "equipo.h"
#include "position.h"
#include "partido.h"
#include "Simu.h"
using namespace std;

int main(void)
{
  string nombre, nombre2;
  cout << "Equipo local: ";
  //cin >> nombre;
  cout << endl; nombre = "ros";
  cout << "Equipo visitante: ";
  //cin >> nombre2;
  cout << endl; nombre2 = "ros";
  alineacion ali(nombre+"sht.txt");
  alineacion ali2(nombre2+"sht.txt");
  equipo eq(nombre);
  equipo eq2(nombre2);
  eq.Load();
  eq2.Load();
  ali.Link(&eq);
  ali2.Link(&eq2);
  ali.Check();
  ali2.Check();
  //Partido
  partido match(&ali, &ali2);
  match.Simulate(90);
  cout << "Test correcto" << endl;
  return 0;
}
