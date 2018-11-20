#include "jug_stats.h"
#include "textmisc.h"
#include "Simu.h"
using namespace std;

//Inicializador por defecto, todo a cero
jug_stats::jug_stats():
  goles(0),
  goles_propia(0),
  encajados(0), 
  asistencias(0),
  chuts(0),
  pases(0),
  tackles(0),
  paradas(0),
  rojas(false),
  amarillas(false),
  minutos(0),
  faltas(0),
  hajugado(false),
  lesionado(false),
  St(0),
  Tk(0),
  Ps(0),
  Sh(0)
{
}
//Calculadores de habilidad
void jug_stats::Update()
{
  //Habilidad de portero
  this->St+=GetVarFrom(Simu::Hab_bonus, "AB_Sav")*this->paradas;
  this->St+=GetVarFrom(Simu::Hab_bonus, "AB_OG")*this->goles_propia;
  this->St+=GetVarFrom(Simu::Hab_bonus, "AB_Concede")*this->encajados;
  this->St+=GetVarFrom(Simu::Hab_bonus, "AB_Yellow")*this->encajados;
  this->St+=GetVarFrom(Simu::Hab_bonus, "AB_Red")*this->encajados;
  //Habilidad de defensa
  this->Tk+=GetVarFrom(Simu::Hab_bonus, "AB_Ktk")*this->tackles;
  this->Tk+=GetVarFrom(Simu::Hab_bonus, "AB_OG")*this->goles_propia;
  this->Tk+=GetVarFrom(Simu::Hab_bonus, "AB_Yellow")*this->encajados;
  this->Tk+=GetVarFrom(Simu::Hab_bonus, "AB_Red")*this->encajados;
  //Habilidad de pase
  this->Ps+=GetVarFrom(Simu::Hab_bonus, "AB_Kps")*this->pases;
  this->Ps+=GetVarFrom(Simu::Hab_bonus, "AB_Assist")*this->asistencias;
  this->Ps+=GetVarFrom(Simu::Hab_bonus, "AB_OG")*this->goles_propia;
  this->Ps+=GetVarFrom(Simu::Hab_bonus, "AB_Yellow")*this->encajados;
  this->Ps+=GetVarFrom(Simu::Hab_bonus, "AB_Red")*this->encajados;
  //Habilidad de chut
  this->Sh+=GetVarFrom(Simu::Hab_bonus, "AB_Goal")*this->goles;
  this->Sh+=GetVarFrom(Simu::Hab_bonus, "AB_Sht")*this->chuts;
  this->Sh+=GetVarFrom(Simu::Hab_bonus, "AB_OG")*this->goles_propia;
  this->Sh+=GetVarFrom(Simu::Hab_bonus, "AB_Yellow")*this->encajados;
  this->Sh+=GetVarFrom(Simu::Hab_bonus, "AB_Red")*this->encajados;
  return;
}