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
  chuts_puerta(0),
  pases(0),
  tackles(0),
  paradas(0),
  rojas(false),
  amarillas(false),
  minutos(0),
  faltas(0),
  hajugado(false),
  lesionado(false),
  trd_inicial(0),
  St(0),
  Tk(0),
  Ps(0),
  Sh(0)
{
}

//Condicional para saber si el jugador esta en condiciones de jugar
bool jug_stats::Is_Playable()
{
  if (!this->lesionado && !this->rojas)
  {
    return true;
  }
  else
  {
    return false;
  }
}

//Calculadores de habilidad
void jug_stats::Update()
{
  //Habilidad de portero
  this->St += GetVarFrom("AB_Sav", Simu::Hab_bonus) * this->paradas;
  this->St += GetVarFrom("AB_OG", Simu::Hab_bonus) * this->goles_propia;
  this->St += GetVarFrom("AB_Concede", Simu::Hab_bonus) * this->encajados;
  this->St += GetVarFrom("AB_Yellow", Simu::Hab_bonus) * this->encajados;
  this->St += GetVarFrom("AB_Red", Simu::Hab_bonus) * this->encajados;
  //Habilidad de defensa
  this->Tk += GetVarFrom("AB_Ktk", Simu::Hab_bonus) * this->tackles;
  this->Tk += GetVarFrom("AB_OG", Simu::Hab_bonus) * this->goles_propia;
  this->Tk += GetVarFrom("AB_Yellow", Simu::Hab_bonus) * this->encajados;
  this->Tk += GetVarFrom("AB_Red", Simu::Hab_bonus) * this->encajados;
  //Habilidad de pase
  this->Ps += GetVarFrom("AB_Kps", Simu::Hab_bonus) * this->pases;
  this->Ps += GetVarFrom("AB_Assist", Simu::Hab_bonus) * this->asistencias;
  this->Ps += GetVarFrom("AB_OG", Simu::Hab_bonus) * this->goles_propia;
  this->Ps += GetVarFrom("AB_Yellow", Simu::Hab_bonus) * this->encajados;
  this->Ps += GetVarFrom("AB_Red", Simu::Hab_bonus) * this->encajados;
  //Habilidad de chut
  this->Sh += GetVarFrom("AB_Goal", Simu::Hab_bonus) * this->goles;
  this->Sh += GetVarFrom("AB_Sht", Simu::Hab_bonus) * this->chuts;
  this->Sh += GetVarFrom("AB_OG", Simu::Hab_bonus) * this->goles_propia;
  this->Sh += GetVarFrom("AB_Yellow", Simu::Hab_bonus) * this->encajados;
  this->Sh += GetVarFrom("AB_Red", Simu::Hab_bonus) * this->encajados;
  return;
}