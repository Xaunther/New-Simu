//Funciones de la clase jugador
#include <string>
#include "position.h"
#include "ability.h"
#include "tactic.h"
using namespace std;

//Inicializar constante
const int position::NPositions = 6;

position::position()
{
  //Inicializar el array con las eficiencias por habilidad
  ability_eff = new double[ability::NAbilities];
  //Posicion sin determinar
  pos = this->NPositions;
}

void position::SetEff(Lposition _pos, Ltactic _tac)
{
  this->pos = _pos; //Establecer posicion
  this->SetEff(_tac); //Establecer eficiencias para dicha tactica
}

void position::SetEff(Ltactic _tac)
{
  tactic tac;
  this->ability_eff = tac.GetEffs(_tac, this->pos);
}
