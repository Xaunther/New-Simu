//Dummy main, para testear compilacion
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "alineacion.h"
#include "equipo.h"
#include "position.h"
#include "partido.h"
#include "Simu.h"
#include "textmisc.h"
using namespace std;

int main(void)
{
  //Inicializa random seed
  srand (time(NULL));
  string nombre, nombre2;
  string op;
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
  //Preguntar por prorroga
  if(GetVarFrom("Extra_Time", Simu::League))
  {
    match.Print_Result();
    cout << "Quieres empezar una prorroga? [S/N]: ";
    cin >> op;
    if(op == "S" || op == "s")
    {
      match.Simulate(30);
    }
  }
  if(GetVarFrom("Tanda_Pen", Simu::League))
  {
    match.Print_Result();
    cout << "Quieres empezar una tanda de penalties? [S/N]: ";
    cin >> op;
    if(op == "S" || op == "s")
    {
      //match.Tanda_Pen(); Sin implementar
    }
  }
  //Al final, printear stats
  match.Write_End();
  return 0;
}
