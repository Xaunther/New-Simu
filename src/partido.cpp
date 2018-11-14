#include "partido.h"
#include "alineacion.h"
#include "textmisc.h"
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

#define col_w 40

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
  minuto(0),
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
  agg_local(50), agg_visitante(50),
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
  //Inicializar archivo nuevo
  outf.open((ali_local->abrev+"_"+ali_visitante->abrev+".txt").c_str());
  outf.close();
}

//Funcion para obtener los puntos de rendimiento de cada equipo en cada posicion
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

//Funcion para simular el partido. El tiempo sera siermpre dividido en primera y segunda parte. Y detectara si es tiempo reglamentario (=90) o prorroga (=30)
void partido::Simulate(int tiempo)
{
  string loc_name = GetLeagueDatString(ali_local->abrev);
  string visit_name = GetLeagueDatString(ali_visitante->abrev);
  //Escribir previo del partido (alis tactica etc)
  if(minuto==0)
  {
    //Abramos el archivo para escribir
    outf.open((ali_local->abrev+"_"+ali_visitante->abrev+".txt").c_str());
    Write_Init();
  }
  else
  {
    outf.open((ali_local->abrev+"_"+ali_visitante->abrev+".txt").c_str(), std::ios_base::app);
  }
  //Sorteo de quien inicia la posesion
  //Ajustes de minuto 0
  //Local
  for(int i=0;i<ali_local->N_usedinst;i++)
  {
    this->Do_Inst(false, i);
  }
  //Visitante
  for(int i=0;i<ali_visitante->N_usedinst;i++)
  {
    this->Do_Inst(true, i);
  }
  //Pasado el previo, llegamos al minuto 1 (o 91)
  minuto++;
  int minuto_init = this->minuto;
  //Loop. Cada minuto una accion
  for(minuto=minuto;minuto<minuto_init+tiempo;minuto++)
  {
    //Descanso
    if(minuto-minuto_init == tiempo/2)
    {
      outf << endl;
      outf << "*************  :primeraparte:  ****************" << endl;
      outf << "Resultado al descanso: " << loc_name << " " << goles_local << "-" << goles_visitante << " " << visit_name << endl;
      outf << endl;
    }
    //Chequear las instrucciones
    //Local
    for(int i=0;i<ali_local->N_usedinst;i++)
    {
      this->Do_Inst(false, i);
    }
    //Visitante
    for(int i=0;i<ali_visitante->N_usedinst;i++)
    {
      this->Do_Inst(true, i);
    }
    //outf << "Min. " << minuto << setw(3-int(log10(minuto))) << ":" << endl;
  }
  
}

void partido::Write_Init()
{
  string loc_name = GetLeagueDatString(ali_local->abrev);
  string visit_name = GetLeagueDatString(ali_visitante->abrev);
  //Encabezado: Nombre de la competicion y equipos enfrentados
  outf << GetLeagueDatString("Games") << ", " << loc_name << " vs. " << visit_name;
  //Fecha de simulacion
  outf << " (" << GetTime() << ")" << endl;
  outf << endl << endl;
  //Empezamos con 11s iniciales:
  //Equipos
  outf << setw(col_w) <<  loc_name << "  |  " << visit_name << endl;
  //Formatciones
  outf << setw(col_w) << ali_local->Formation() << "  |  " << ali_visitante->Formation() << endl;
  //Entrenadores (busca abrev_DM)
  outf << setw(col_w) << GetLeagueDatString(ali_local->abrev+"_DM") << "  |  " << GetLeagueDatString(ali_visitante->abrev+"_DM") << endl;
  outf << setw(col_w) << " " << "  |  " << endl;
  //Titulares
  for(int i=0;i<N_titulares;i++)
  {
    outf << setw(col_w-3) << ali_local->names_titulares[i] << " " << ali_local->pos_titulares[i].symbol() << "  |  " << ali_visitante->pos_titulares[i].symbol() << " " << ali_visitante->names_titulares[i] << endl;
  }
  outf << setw(col_w) << " " << "  |  " << endl;
  //Suplentes
  for(int i=0;i<ali_local->N_suplentes;i++)
  {
    outf << setw(col_w-3) << ali_local->names_suplentes[i] << " SUB | SUB " << ali_visitante->names_suplentes[i] << endl;
  }
  outf << endl << endl << endl << endl << endl;
  return;
}

//Ejecuta, si cumple los criterios, la condición k del local (false) o visitante (true)
void partido::Do_Inst(bool side, int k)
{
  bool cumple = true;
  alineacion* ali;
  if(side)
  {
    ali = ali_local;
  }
  else
  {
    ali = ali_visitante;
  }
  for(int i=0;i<ali->condicion[k].N_usedcond;i++)
  {
    cumple *= Is_Doable(side, k, i);
  }
  //Si no cumple, nada
  if(!cumple){return;}
  //Chequear que el archivo este abierto
  if(!outf.is_open()){outf.open((ali_local->abrev+"_"+ali_visitante->abrev+".txt").c_str(), std::ios_base::app);}
  switch(ali->condicion[k].type)
  {
    case Simu::lPK: //No tiene output
      ali->lanza_pen = ali->titulares[ali->condicion[k].arg1-1];
      return;
    case Simu::lCPN: //No tiene output
      ali->capitan = ali->titulares[ali->condicion[k].arg1-1];
      return;
    case Simu::lAGG: //No tiene output
      agg_local = ali->condicion[k].arg1;
      return;
    case Simu::lTACTIC:
      if(ali->tactica.tac != ali->condicion[k].tactic)
      {
        outf << "Min. " << minuto << setw(3-int(log10(minuto))) << ":(" << ali->abrev << ") ";
        outf << "tactic: " << ali->condicion[k].tactic << endl;
      }
      ali->tactica.tac = ali->condicion[k].tactic;
      return;
    case Simu::lSUB: //Not implemented
      return;
    case Simu::lCHANGEPOS:
      if(ali->condicion[k].arg1-1 < N_titulares)
      {
        if(ali->pos_titulares[ali->condicion[k].arg1-1].pos != ali->condicion[k].pos)
        {
          outf << "Min. " << minuto << setw(3-int(log10(minuto))) << ":(" << ali->abrev << ") ";
          outf << "changepos: " << ali->condicion[k].tactic << endl;
        }
      }
      else if(ali->pos_suplentes[ali->condicion[k].arg1-1].pos != ali->condicion[k].pos)
      {
        outf << "Min. " << minuto << setw(3-int(log10(minuto))) << ":(" << ali->abrev << ") ";
        outf << "changepos: " << ali->condicion[k].tactic << endl;
      }
      if(ali->condicion[k].arg1-1 < N_titulares)
      {
        ali->pos_titulares[ali->condicion[k].arg1-1].pos = ali->condicion[k].pos;
      }
      else
      {
        ali->pos_suplentes[ali->condicion[k].arg1-1].pos = ali->condicion[k].pos;
      }
      return;
  }
}

//Dice si la condicion  j de la instruccion k del equipo local (false) o visitante (true) se cumple o no
bool partido::Is_Doable(bool side, int k, int j)
{
  //Valor de la variable ira almacenada aqui
  int varvalue;
  //Puntero a la ali que se esta mirando
  alineacion* ali;
  if(side){ali = ali_local;}else{ali = ali_visitante;}
  //Que tipo de instruccion es?
  switch(ali->condicion[k].cond[j])
  {
    case Simu::lMIN:
      varvalue = minuto;
      break;
    case Simu::lSCORE:
      varvalue = (goles_visitante - goles_local)*(2*side-1);
      break;
    case Simu::lSHOTS:
      varvalue = (chuts_visitante - chuts_local)*(2*side-1);
      break;
      //Sin implementar hasta que se hagan las estadisticas live (devuelven false)
    case Simu::lYELLOW: return false;
      break;
    case Simu::lRED: return false;
      break;
    case Simu::lINJ: return false;
      break;
  }
  //Ques es? >=, = o <=?
  switch(ali->condicion[k].symbol[j])
  {
    case Simu::lLT:
      return(varvalue <= ali->condicion[k].cond_value[j]);
    case Simu::lEQ:
      return(varvalue == ali->condicion[k].cond_value[j]);
    case Simu::lGT:
      return(varvalue >= ali->condicion[k].cond_value[j]);
  }
}