#include "partido.h"
#include "alineacion.h"
using namespace std;

//Funciones de la clase partido

//Initializers: default
partido::partido(alineacion* _local, alineacion* _visitante):
  ali_local(_local),
  ali_visitante(_visitante),
  def_local(0), def_visitante(0),
  med_local(0), med_visitante(0),
  atk_local(0), atk_visitante(0)
{
  //Smth
}

void partido::Update_pts()
{
  //Loop sobre los titulares, y voy sumando...
  for(int i=0;i<N_titulares;i++)
    {
      //Equipo local
      ali_local->pos_titulares[i].SetEff(ali_local->tactica.tac);
      double* eff = ali_local->pos_titulares[i].ability_eff;
      gk_local+=eff[0]*ali_local->titulares[i]->St;
      def_local+=eff[1]*ali_local->titulares[i]->Tk;
      med_local+=eff[2]*ali_local->titulares[i]->Ps;
      atk_local+=eff[3]*ali_local->titulares[i]->Sh;

      //Equipo visitante
      ali_visitante->pos_titulares[i].SetEff(ali_visitante->tactica.tac);
      eff = ali_visitante->pos_titulares[i].ability_eff;
      gk_visitante+=eff[0]*ali_visitante->titulares[i]->St;
      def_visitante+=eff[1]*ali_visitante->titulares[i]->Tk;
      med_visitante+=eff[2]*ali_visitante->titulares[i]->Ps;
      atk_visitante+=eff[3]*ali_visitante->titulares[i]->Sh;
    }
}
