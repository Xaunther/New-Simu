#ifndef position_h
#define position_h

#include <string>
#include "tactic.h"
using namespace std;

enum Lposition{lGK, lDF, lDM, lMF, lAM, lFW};

//Clase posición
class position
{
public:
  //Constantes
  static const int NPositions;
  //Variables
  double* ability_eff;
  Lposition pos;
  
  //Funciones
  position();
  void SetEff(Lposition, Ltactic);
  void SetEff(Ltactic);
};

#endif
