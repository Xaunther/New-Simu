//Clase para tacticas y enumeracio para hacerlo mas legible
#ifndef tactic_h
#define tactic_h

#include "Simu.h"
#include <string>
using namespace std;

class tactic
{
public:
  //Variables
  double*** effs; //Matriz 3-d con todas eficiencias
  bool isFilled;
  Simu::Ltactic tac;
  //Funciones
  tactic();
  tactic(Simu::Ltactic);
  void Fill();
  double* GetEffs(Simu::Ltactic, Simu::Lposition);
  double* GetEffs(Simu::Lposition);
  void SetTactic(string);
  string symbol(); //Devuelve string con la tactica
  bool Beats(Simu::Ltactic); //Devuelve si le gana o no
  void dump();
};

#endif
