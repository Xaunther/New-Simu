#include "partido.h"
#include "alineacion.h"
#include "textmisc.h"
#include "jug_stats.h"
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
  stats_local(new jug_stats[N_titulares+N_suplentes]), stats_visitante(new jug_stats[N_titulares+N_suplentes])
{
  //Funciones de inicializacion
  SetLocalBoost();
  Stats_Init();
  Update_pts();
  //Inicializar archivo nuevo
  outf.open((ali_local->abrev+"_"+ali_visitante->abrev+".txt").c_str());
  outf.close();
}

//Funcion para obtener los puntos de rendimiento de cada equipo en cada posicion
void partido::Update_pts()
{
  gk_local = 0;
  def_local = 0;
  med_local = 0;
  atk_local = 0;
  gk_visitante = 0;
  def_visitante = 0;
  med_visitante = 0;
  atk_visitante = 0;
	//Loop sobre los titulares, y voy sumando...
	for(int i=0;i<N_titulares;i++)
	{ 
		//Equipo local
		ali_local->pos_titulares[i].SetEff(ali_local->tactica.tac);
		double* eff = ali_local->pos_titulares[i].ability_eff;
		gk_local+=eff[0]*ali_local->titulares[i]->St*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado);
		def_local+=eff[1]*ali_local->titulares[i]->Tk*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado);
		med_local+=eff[2]*ali_local->titulares[i]->Ps*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado);
		atk_local+=eff[3]*ali_local->titulares[i]->Sh*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado);

		//Equipo visitante
		ali_visitante->pos_titulares[i].SetEff(ali_visitante->tactica.tac);
		eff = ali_visitante->pos_titulares[i].ability_eff;
		gk_visitante+=eff[0]*ali_visitante->titulares[i]->St*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado);
		def_visitante+=eff[1]*ali_visitante->titulares[i]->Tk*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado);
		med_visitante+=eff[2]*ali_visitante->titulares[i]->Ps*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado);
		atk_visitante+=eff[3]*ali_visitante->titulares[i]->Sh*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado);
	}
}

//Inicializar stats a alguna cosa no por defecto (por ejemplo los titulares han jugado)
void partido::Stats_Init()
{
  for(int i=0;i<N_titulares;i++)
  {
    this->stats_local[i].hajugado = true;
    this->stats_visitante[i].hajugado = true;
  }
}

//Funcion para obtener el boost local a partir del archivo de parametros
void partido::SetLocalBoost()
{
  local_boost = GetLeagueDat("Home_Bonus");
}

//Funcion para simular el partido. El tiempo sera siermpre dividido en primera y segunda parte. Y detectara si es tiempo reglamentario (=90) o prorroga (=30)
void partido::Simulate(int tiempo)
{
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
  this->Do_Inst(false);
  //Visitante
  this->Do_Inst(true);
  //Pasado el previo, llegamos al minuto 1 (o 91)
  minuto++;
  int minuto_init = this->minuto;
  //Loop. Cada minuto una accion
  for(minuto=minuto;minuto<minuto_init+tiempo;minuto++)
  {
    //Descanso
    if(minuto-minuto_init == tiempo/2){this->Write_HT();}
    //Chequear las instrucciones
    //Local
    this->Do_Inst(false);
    //Visitante
    this->Do_Inst(true);
    //Añade un minuto de juego a todos los que hay en el campo
    this->AddMin();
    //Reduce el fit a los que estan jugando, si toca...
    this->ReduceFit();
    //Actualizar stats
    this->Update_pts();
    //Eventos (Sin implementar)
    
    //outf << "Min. " << minuto << setw(3-int(log10(minuto))) << ":" << endl;
  }
  //Final
  this->Write_FT();
}

//Escribe el previo
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
  //Formaciones
  outf << setw(col_w) << ali_local->Formation() << "  |  " << ali_visitante->Formation() << endl;
  //Entrenadores (busca abrev_DM)
  outf << setw(col_w) << GetLeagueDatString(ali_local->abrev+"_DM") << "  |  " << GetLeagueDatString(ali_visitante->abrev+"_DM") << endl;
  outf << setw(col_w) << " " << "  |  " << endl;
  //Titulares
  for(int i=0;i<N_titulares;i++)
  {
    outf << setw(col_w-3) << ali_local->titulares[i]->Name << " " << ali_local->pos_titulares[i].symbol() << "  |  " << ali_visitante->pos_titulares[i].symbol() << " " << ali_visitante->titulares[i]->Name << endl;
  }
  outf << setw(col_w) << " " << "  |  " << endl;
  //Suplentes
  for(int i=0;i<ali_local->N_suplentes;i++)
  {
    outf << setw(col_w-3) << ali_local->suplentes[i]->Name << " SUB | SUB " << ali_visitante->suplentes[i]->Name << endl;
  }
  outf << endl << endl << endl << endl << endl;
  return;
}

//Ejecuta, si cumple los criterios, la condición k del local (false) o visitante (true)
void partido::Do_Inst(bool side)
{
  for(int i=0;i<ali_visitante->N_usedinst;i++)
  {
    this->Do_Inst(side, i);
  }
}
void partido::Do_Inst(bool side, int k)
{
  stringstream ss;
  bool cumple = true;
  alineacion* ali;
  if(!side)
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
        ss << "tactic: " << ali->condicion[k].tactic;
        this->Write_Event(ali, ss.str());
      }
      ali->tactica.tac = ali->condicion[k].tactic;
      return;
    case Simu::lSUB:
      if(!side)
      {
        if(this->cambios_local < GetLeagueDat("Cambios") && !this->stats_local[ali->condicion[k].arg2-1].hajugado && !this->stats_local[ali->condicion[k].arg1-1].rojas)
        {
          ss << "cambio: " << ali->condicion[k].arg1 << ", " << ali->condicion[k].arg2;
          this->Write_Event(ali, ss.str());
          //Intercambio jugadores
          jugador* temp;
          temp = ali->titulares[ali->condicion[k].arg1-1];
          ali->titulares[ali->condicion[k].arg1-1] = ali->suplentes[ali->condicion[k].arg2-N_titulares-1];
          ali->suplentes[ali->condicion[k].arg2-N_titulares-1] = temp;
          //Intercambio stats
          jug_stats temp2;
          temp2 = this->stats_local[ali->condicion[k].arg1-1];
          this->stats_local[ali->condicion[k].arg1-1] = this->stats_local[ali->condicion[k].arg2-1];
          this->stats_local[ali->condicion[k].arg2-1] = temp2;
          //+1 cambio hecho
          this->cambios_local++;
          //jugador que entra ha jugado :)
          this->stats_local[ali->condicion[k].arg1-1].hajugado = true;
        }
      }
      else
      {
        if(this->cambios_visitante < GetLeagueDat("Cambios") && !this->stats_visitante[ali->condicion[k].arg2-1].hajugado && !this->stats_local[ali->condicion[k].arg1-1].rojas)
        {
          ss << "cambio: " << ali->condicion[k].arg1 << ", " << ali->condicion[k].arg2;
          this->Write_Event(ali, ss.str());
          //Intercambio jugadores
          jugador* temp;
          temp = ali->titulares[ali->condicion[k].arg1-1];
          ali->titulares[ali->condicion[k].arg1-1] = ali->suplentes[ali->condicion[k].arg2-N_titulares-1];
          ali->suplentes[ali->condicion[k].arg2-N_titulares-1] = temp;
          //Intercambio stats
          jug_stats temp2;
          temp2 = this->stats_visitante[ali->condicion[k].arg1-1];
          this->stats_visitante[ali->condicion[k].arg1-1] = this->stats_visitante[ali->condicion[k].arg2-1];
          this->stats_visitante[ali->condicion[k].arg2-1] = temp2;
          //+1 cambio hecho
          this->cambios_visitante++;
          //jugador que entra ha jugado :)
          this->stats_visitante[ali->condicion[k].arg1-1].hajugado = true;
        }
      }
      //Posicion del jugador que entra
      if(ali->pos_titulares[ali->condicion[k].arg1-1].pos != Simu::lGK)
      {
        ali->pos_titulares[ali->condicion[k].arg1-1].pos =ali->condicion[k].pos;
      }
      return;
    case Simu::lCHANGEPOS:
      if(ali->pos_titulares[ali->condicion[k].arg1-1].pos == Simu::lGK || ali->condicion[k].pos == Simu::lGK)
      {
        return; //El portero no se puede tocar!
      }
      if(ali->pos_titulares[ali->condicion[k].arg1-1].pos != ali->condicion[k].pos)
      {
        ss << "changepos: " << ali->condicion[k].tactic;
        this->Write_Event(ali, ss.str());
      }
      ali->pos_titulares[ali->condicion[k].arg1-1].pos = ali->condicion[k].pos;
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
  if(!side){ali = ali_local;}else{ali = ali_visitante;}
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
    case Simu::lYELLOW:
      varvalue = this->stats_local[ali->condicion[k].cond_value[j]-1].amarillas*ali->condicion[k].cond_value[j];
      break;
    case Simu::lRED:
      varvalue = this->stats_local[ali->condicion[k].cond_value[j]-1].rojas*ali->condicion[k].cond_value[j];
      break;
    case Simu::lINJ:
      varvalue = this->stats_local[ali->condicion[k].cond_value[j]-1].lesionado*ali->condicion[k].cond_value[j];
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

//Escribir un evento en cierto minuto
void partido::Write_Event(alineacion* ali, string cosa)
{
  //Minuto
  outf << "Min. " << minuto << setw(3-int(log10(minuto))) << ":(" << ali->abrev << ") ";
  //Cosa
  outf << cosa << endl;
}

//Escribir descanso
void partido::Write_HT()
{
  outf << endl;
  outf << "*************  :primeraparte:  ****************" << endl;
  outf << "Resultado al descanso: " << GetLeagueDatString(ali_local->abrev) << " " << goles_local << "-" << goles_visitante << " " << GetLeagueDatString(ali_visitante->abrev) << endl;
  outf << endl;
}

//Escribir segunda parte
void partido::Write_FT()
{
  outf << endl;
  outf << "*************  :segundaparte:  ****************" << endl;
  outf << "Resultado final: " << GetLeagueDatString(ali_local->abrev) << " " << goles_local << "-" << goles_visitante << " " << GetLeagueDatString(ali_visitante->abrev) << endl;
  outf << endl;
}

//Añadir minuto a los jugadores que estan sobre el campo
void partido::AddMin()
{
  for(int i=0;i<N_titulares;i++)
  {
    if(!this->stats_local[i].lesionado && !this->stats_local[i].rojas)
    {
      stats_local[i].minutos++;
    }
    if(!this->stats_visitante[i].lesionado && !this->stats_visitante[i].rojas)
    {
      stats_visitante[i].minutos++;
    }
  }
}

void partido::ReduceFit()
{
  for(int i=0;i<N_titulares;i++)
  {
    if(!this->stats_local[i].lesionado && !this->stats_local[i].rojas && this->stats_local[i].minutos % GetLeagueDat("Fit_perdido") == 0)
    {
      this->ali_local->titulares[i]->Fit--;
    }
    if(!this->stats_visitante[i].lesionado && !this->stats_visitante[i].rojas && this->stats_visitante[i].minutos % GetLeagueDat("Fit_perdido") == 0)
    {
      this->ali_visitante->titulares[i]->Fit--;
    }
  }
}