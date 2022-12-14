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
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  /* using nano-seconds instead of seconds */
  srand((time_t)ts.tv_nsec);

  string nombre, nombre2;
  string op;
  cout << "Equipo local: ";
  //cin >> nombre;
  cout << endl; nombre = "ros";
  cout << "Equipo visitante: ";
  //cin >> nombre2;
  cout << endl; nombre2 = "ros";
  alineacion ali(nombre + "sht.txt");
  alineacion ali2(nombre2 + "sht.txt");
  equipo eq(nombre);
  equipo eq2(nombre2);
  eq.Load(); ali.Link(&eq); ali.Check();
  eq2.Load(); ali2.Link(&eq2); ali2.Check();

  //Partido
  partido match(&ali, &ali2);
  match.Simulate(90);
  //Preguntar por prorroga
  if (GetVarFrom("Extra_Time", Simu::League))
  {
    match.Print_Result();
    cout << "Quieres empezar una prorroga? [S/N]: ";
    cin >> op;
    if (op == "S" || op == "s")
    {
      match.Simulate(30);
    }
  }
  if (GetVarFrom("Tanda_Pen", Simu::League))
  {
    match.Print_Result();
    cout << "Quieres empezar una tanda de penalties? [S/N]: ";
    cin >> op;
    if (op == "S" || op == "s")
    {
      //match.Tanda_Pen(); Sin implementar
    }
  }
  //Al final, printear stats
  match.Write_End();
  //Updatear skills? (St, Tk, Ps, Sh)
  if (GetVarFrom("Update_Skills", Simu::League))
  {
    match.Update_Skills();
  }
  //Updatear stats? pases, goles etc.
  if (GetVarFrom("Update_Stats", Simu::League))
  {
    match.Update_Stats();
  }
  //Updatear fit?
  if (GetVarFrom("Update_Fitness", Simu::League) == 0)
  {
    match.Reset_Fitness();
  }
  else
  {
    eq.AddTrd(GetVarFrom("Fit_recuperado", Simu::League));
    eq2.AddTrd(GetVarFrom("Fit_recuperado", Simu::League));
  }
  //Updatear lesiones?
  if (GetVarFrom("Update_Injuries", Simu::League))
  {
    match.Update_Injuries();
    eq.ReduceInj();
    eq2.ReduceInj();
  }
  //Updatear sanciones?
  if (GetVarFrom("Update_Suspensions", Simu::League))
  {
    match.Update_Suspensions();
    eq.ReduceSus();
    eq2.ReduceSus();
  }

  return 0;
}
