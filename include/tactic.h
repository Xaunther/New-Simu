//Clase para tacticas y enumeracio para hacerlo mas legible
#ifndef tactic_h
#define tactic_h

#include "Simu.h"
using namespace std;

class tactic
{
public:
  //Variables
  double*** effs; //Matriz 3-d con todas eficiencias
  bool isFilled;
  //Funciones
  tactic();
  void Fill();
  double* GetEffs(Simu::Ltactic, Simu::Lposition);
};

#endif
