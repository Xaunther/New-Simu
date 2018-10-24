#ifndef partido_h
#define partido_h

#include "alineacion.h"
using namespace std;

//Clase partido
class partido
{
  //Variables
 public:
  alineacion* ali_local;
  alineacion* ali_visitante;
  double gk_local;  double gk_visitante;
  double def_local; double def_visitante;
  double med_local; double med_visitante;
  double atk_local; double atk_visitante;
  
  //Funciones
  partido(alineacion*, alineacion*);
  void Update_pts();
};



#endif
