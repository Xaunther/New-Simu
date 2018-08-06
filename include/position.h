#ifndef position_h
#define position_h

#include <string>
#include "Simu.h"
using namespace std;

//Clase posición
class position
{
public:
  //Variables
  double* ability_eff;
  Simu::Lposition pos;
  
  //Funciones
  position();
  position(Simu::Lposition);
  void SetEff(Simu::Lposition, Simu::Ltactic);
  void SetEff(Simu::Ltactic);
};

#endif
