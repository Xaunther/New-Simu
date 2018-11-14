#ifndef position_h
#define position_h

#include <string>
#include "Simu.h"
using namespace std;

//Clase posici�n
class position
{
public:
  //Variables
  double* ability_eff;
  Simu::Lposition pos;
  
  //Funciones
  position();
  position(Simu::Lposition);
  void SetPos(string);
  void SetEff(Simu::Lposition, Simu::Ltactic);
  void SetEff(Simu::Ltactic);
  string symbol();
  void dump();
};

#endif
