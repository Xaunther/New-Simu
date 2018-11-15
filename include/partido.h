#ifndef partido_h
#define partido_h

#include "alineacion.h"
#include "jug_stats.h"
#include <fstream>
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
  int local_boost;
  int N_suplentes;
  ofstream outf;
  //Estadios?! estadio est; int attendance;
  //Arbitro?! arbitro arb;
  
  //Estadisticas del partido globales
  int minuto;
  bool posesion; //false=local, true=visit
  int posesion_local; int posesion_visitante;
  int goles_local; int goles_visitante;
  int chuts_local; int chuts_visitante;
  int chuts_puerta_local; int chuts_puerta_visitante;
  int pases_local; int pases_visitante;
  int tackles_local; int tackles_visitante;
  int corners_local; int corners_visitante;
  int rojas_local; int rojas_visitante;
  int amarillas_local; int amarillas_visitante;
  int cambios_local; int cambios_visitante;
  int faltas_local; int faltas_visitante;
  int agg_local; int agg_visitante;
    //Estadisticas del partido por jugador
  jug_stats* stats_local; jug_stats* stats_visitante;
  
  
  //Funciones
  partido(alineacion*, alineacion*);
  void Update_pts();
  void Stats_Init();
  void SetLocalBoost();
  void Simulate(int);
  void Do_Inst(bool);
  void Do_Inst(bool, int);
  bool Is_Doable(bool, int, int);
  //Funciones de escritura
  void Write_Init();
  void Write_Event(alineacion*, string);
  void Write_HT();
  void Write_FT();
};



#endif
