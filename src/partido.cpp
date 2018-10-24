#include "partido.h"
#include "alineacion.h"
#include "textmisc.h"
using namespace std;

//Funciones de la clase partido

//Initializers: default
partido::partido(alineacion* _local, alineacion* _visitante):
  //Alis
  ali_local(_local),
  ali_visitante(_visitante),
  //Fuerza
  def_local(0), def_visitante(0),
  med_local(0), med_visitante(0),
  atk_local(0), atk_visitante(0),
  //Stats globales
  posesion_local(0), posesion_visitante(0),
  goles_local(0), goles_visitante(0),
  chuts_local(0), chuts_visitante(0),
  chuts_puerta_local(0), chuts_puerta_visitante(0),
  pases_local(0), pases_visitante(0),
  tackles_local(0), tackles_visitante(0),
  corners_local(0), corners_visitante(0),
  rojas_local(0), rojas_visitante(0),
  amarillas_local(0), amarillas_visitante(0),
  cambios_local(0), cambios_visitante(0),
  faltas_local(0), faltas_visitante(0),
  //Otros
  N_suplentes(GetLeagueDat("Suplentes")),
  //Stats por jugador
  goles_jug_local(new int[N_titulares+N_suplentes]), goles_jug_visitante(new int[N_titulares+N_suplentes]),
  asistencias_jug_local(new int[N_titulares+N_suplentes]), asistencias_jug_visitante(new int[N_titulares+N_suplentes]),
  chuts_jug_local(new int[N_titulares+N_suplentes]), chuts_jug_visitante(new int[N_titulares+N_suplentes]),
  pases_jug_local(new int[N_titulares+N_suplentes]), pases_jug_visitante(new int[N_titulares+N_suplentes]),
  tackles_jug_local(new int[N_titulares+N_suplentes]), tackles_jug_visitante(new int[N_titulares+N_suplentes]),
  paradas_jug_local(new int[N_titulares+N_suplentes]), paradas_jug_visitante(new int[N_titulares+N_suplentes]),
  rojas_jug_local(new bool[N_titulares+N_suplentes]), rojas_jug_visitante(new bool[N_titulares+N_suplentes]),
  amarillas_jug_local(new bool[N_titulares+N_suplentes]), amarillas_jug_visitante(new bool[N_titulares+N_suplentes]),
  minutos_jug_local(new int[N_titulares+N_suplentes]), minutos_jug_visitante(new int[N_titulares+N_suplentes]),
  faltas_jug_local(new int[N_titulares+N_suplentes]), faltas_jug_visitante(new int[N_titulares+N_suplentes])
{
  //Funciones de inicializacion
  SetLocalBoost();
  Update_pts();
  Init_stats();	
}

//Funcion para obtener los puntos de rendimiento de cada equipo en cada posicion
void partido::Update_pts()
{
	//Loop sobre los titulares, y voy sumando...
	for(int i=0;i<N_titulares+N_suplentes;i++)
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

//Funcion para obtener el boost local a partir del archivo de parametros
void partido::SetLocalBoost()
{
  local_boost = GetLeagueDat("Home_Bonus");
}

//Funcion para inicializar a cero las estadisticas de los jugadores
void partido::Init_stats()
{
  //Loop sobre todo
  for(int i=0;i<N_titulares+N_suplentes;i++)
  {
    goles_jug_local[i] = 0; goles_jug_visitante = 0;
    asistencias_jug_local[i] = 0; asistencias_jug_visitante = 0;
    chuts_jug_local[i] = 0; chuts_jug_visitante = 0;
    pases_jug_local[i] = 0; pases_jug_visitante = 0;
    tackles_jug_local[i] = 0; tackles_jug_visitante = 0;
    paradas_jug_local[i] = 0; paradas_jug_visitante = 0;
    rojas_jug_local[i] = 0; rojas_jug_visitante = 0;
    amarillas_jug_local[i] = 0; amarillas_jug_visitante = 0;
    minutos_jug_local[i] = 0; minutos_jug_visitante = 0;
    faltas_jug_local[i] = 0; faltas_jug_visitante = 0;
  }
}