//Funciones de la clase jugador
#include <string>
#include <iostream>
#include <stdlib.h> //Para la funcion exit
#include "position.h"
#include "tactic.h"
#include "Simu.h"
using namespace std;

//Inicializar sin posicion (posicion invalida)
position::position()
{
  //Inicializar el array con las eficiencias por habilidad
  ability_eff = new double[Simu::NAbilities];
  //Posicion sin determinar
  pos = (Simu::Lposition)Simu::NPositions;
}

//Inicializar a una posicion
position::position(Simu::Lposition _pos)
{
  //Inicializar el array con las eficiencias por habilidad
  ability_eff = new double[Simu::NAbilities];
  //Posicion sin determinar
  pos = _pos;
}

//Traducir string a posicion
void position::SetPos(string posname)
{
  if (posname == "GK")
  {
    this->pos = Simu::lGK;
  }
  else if (posname == "DF")
  {
    this->pos = Simu::lDF;
  }
  else if (posname == "DM")
  {
    this->pos = Simu::lDM;
  }
  else if (posname == "MF")
  {
    this->pos = Simu::lMF;
  }
  else if (posname == "AM")
  {
    this->pos = Simu::lAM;
  }
  else if (posname == "FW")
  {
    this->pos = Simu::lFW;
  }
  else
  {
    cout << "Posicion invalida en alineacion" << endl;
    exit(1);
  }
}

void position::SetEff(Simu::Lposition _pos, Simu::Ltactic _tac)
{
  this->pos = _pos; //Establecer posicion
  this->SetEff(_tac); //Establecer eficiencias para dicha tactica
}

void position::SetEff(Simu::Ltactic _tac)
{
  tactic tac;
  this->ability_eff = tac.GetEffs(_tac, this->pos);
}

string position::symbol()
{
  switch (this->pos)
  {
  case Simu::lGK:
    return "GK";
  case Simu::lDF:
    return "DF";
  case Simu::lDM:
    return "DM";
  case Simu::lMF:
    return "MF";
  case Simu::lAM:
    return "AM";
  case Simu::lFW:
    return "FW";
  default:
    return "";
  }
}

void position::dump()
{
  cout << "-------------------------------------" << endl;
  cout << "double* ability_eff " << ability_eff << endl;
  cout << "Simu::Lposition pos " << pos << endl;;
  cout << "-------------------------------------" << endl;
}
