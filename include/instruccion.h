//Clase para las instrucciones tacticas
#ifndef instruccion_h
#define instruccion_h

#include "alineacion.h"
#include "Simu.h"
#include <string>
using namespace std;

//Numero maximo de condiciones por instruccion, pongo 8 por poner argo...
#define N_cond 8

class instruccion
{
 public:
  //Variables
  Simu::Linstruct type; //Tipo de instruccion
  //Lista de posibles argumentos, dependiendo de la instruccion se usan unos u otros
  int arg1;
  int arg2;
  Simu::Ltactic tactic;
  Simu::Lposition pos;
  //Lista de posibles condiciones
  int N_usedcond; //Cuantas condiciones se usan
  Simu::Lcondition cond[N_cond]; //Saber que tipo de condicion es (minutos, roja, resultado...)
  Simu::Lsymbol symbol[N_cond]; //Saber si es mayor que, menor...
  int cond_value[N_cond]; //Saber el valor que se ha dado
  
  //Funciones
  instruccion(); //Constructor por defecto
  instruccion(string);
  void Exit();
  void Fill(string);
  void SetType(string);
  void SetTactic(string);
  void SetPos(string);
  void SetCondition(int, string);
  void SetSymbol(int, string);
  void Check();
  void dump();
};

#endif
