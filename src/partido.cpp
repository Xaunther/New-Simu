#include "partido.h"
#include "alineacion.h"
#include "textmisc.h"
#include "jug_stats.h"
#include "RandomTools.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdlib.h>
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

//Destructor
partido::~partido()
{
  if(this->outf)
  {
    outf.close();
  }
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
    //Factor de fit: 0 de fit equivale a jugar con habilidad 0
		gk_local+=eff[0]*ali_local->titulares[i]->St*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado)*double(ali_local->titulares[i]->Trd/100.);
		def_local+=eff[1]*ali_local->titulares[i]->Tk*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado)*double(ali_local->titulares[i]->Trd/100.);
		med_local+=eff[2]*ali_local->titulares[i]->Ps*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado)*double(ali_local->titulares[i]->Trd/100.);
		atk_local+=eff[3]*ali_local->titulares[i]->Sh*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado)*double(ali_local->titulares[i]->Trd/100.);

		//Equipo visitante
		ali_visitante->pos_titulares[i].SetEff(ali_visitante->tactica.tac);
		eff = ali_visitante->pos_titulares[i].ability_eff;
		gk_visitante+=eff[0]*ali_visitante->titulares[i]->St*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado)*double(ali_visitante->titulares[i]->Trd/100.);
		def_visitante+=eff[1]*ali_visitante->titulares[i]->Tk*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado)*double(ali_visitante->titulares[i]->Trd/100.);
		med_visitante+=eff[2]*ali_visitante->titulares[i]->Ps*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado)*double(ali_visitante->titulares[i]->Trd/100.);
		atk_visitante+=eff[3]*ali_visitante->titulares[i]->Sh*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado)*double(ali_visitante->titulares[i]->Trd/100.);
	}
}

void partido::Update_exp()
{
  this->Update_exp(this->stats_local);
  this->Update_exp(this->stats_visitante);
}
//Funcion para rellenar la exp ganada por cada jugador
void partido::Update_exp(jug_stats* stats)
{
  //Exp random para el equipo que pierda
  int rN = rand() % N_titulares;
  if((stats == this->stats_local && this->goles_local < this->goles_visitante) ||
     (stats == this->stats_visitante && this->goles_visitante < this->goles_local))
  {
    stats[rN].St+=GetVarFrom("AB_Defeat_Random", Simu::Hab_bonus);
    stats[rN].Tk+=GetVarFrom("AB_Defeat_Random", Simu::Hab_bonus);
    stats[rN].Ps+=GetVarFrom("AB_Defeat_Random", Simu::Hab_bonus);
    stats[rN].Sh+=GetVarFrom("AB_Defeat_Random", Simu::Hab_bonus);
  }
  //Exp random para el equipo que gane
  else if((stats == this->stats_local && this->goles_local > this->goles_visitante) ||
          (stats == this->stats_visitante && this->goles_visitante > this->goles_local))
  {
    stats[rN].St+=GetVarFrom("AB_Victory_Random", Simu::Hab_bonus);
    stats[rN].Tk+=GetVarFrom("AB_Victory_Random", Simu::Hab_bonus);
    stats[rN].Ps+=GetVarFrom("AB_Victory_Random", Simu::Hab_bonus);
    stats[rN].Sh+=GetVarFrom("AB_Victory_Random", Simu::Hab_bonus);
  }
  //Exp random para el empate
  else
  {
    stats[rN].St+=GetVarFrom("AB_Draw_Random", Simu::Hab_bonus);
    stats[rN].Tk+=GetVarFrom("AB_Draw_Random", Simu::Hab_bonus);
    stats[rN].Ps+=GetVarFrom("AB_Draw_Random", Simu::Hab_bonus);
    stats[rN].Sh+=GetVarFrom("AB_Draw_Random", Simu::Hab_bonus);
  }
  //Exp para el portero por porteria a cero
  if((stats == this->stats_local && this->goles_local == 0) ||
     (stats == this->stats_visitante && this->goles_visitante == 0))
  {
    stats[0].St+=GetVarFrom("AB_Clean_Sheet", Simu::Hab_bonus);
  }
  
  //Exp por stats individuales (loop)
  for(int i=0;i<N_titulares+N_suplentes;i++)
  {
    stats[i].Update();
  }
}

//Inicializar stats a alguna cosa no por defecto (por ejemplo los titulares han jugado)
void partido::Stats_Init()
{
  for(int i=0;i<N_titulares;i++)
  {
    this->stats_local[i].hajugado = true;
    this->stats_visitante[i].hajugado = true;
    //Guardar el fit inicial tambien
    this->stats_local[i].trd_inicial = this->ali_local->titulares[i]->Fit;
    this->stats_visitante[i].trd_inicial = this->ali_visitante->titulares[i]->Fit;
  }
  for(int i=0;i<N_suplentes;i++)
  {
    this->stats_local[N_titulares+i].trd_inicial = this->ali_local->suplentes[i]->Fit;
    this->stats_visitante[N_titulares+i].trd_inicial = this->ali_visitante->suplentes[i]->Fit;
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
    if(!outf.is_open())
    {
      outf.open((ali_local->abrev+"_"+ali_visitante->abrev+".txt").c_str());
    }
    Write_Init();
  }
  else if(!outf.is_open())
  {
    outf.open((ali_local->abrev+"_"+ali_visitante->abrev+".txt").c_str(), std::ios_base::app);
  }
  //Sorteo de quien inicia la posesion
  //Ajustes de minuto 0
  this->Do_Inst();
  //Pasado el previo, llegamos al minuto 1 (o 91)
  this->minuto++;
  int minuto_init = this->minuto;
  //Loop. Cada minuto una accion
  for(this->minuto=this->minuto;this->minuto<minuto_init+tiempo;this->minuto++)
  {
    //Descanso
    if(this->minuto-minuto_init == tiempo/2){this->Write_HT();}
    //Chequear las instrucciones
    this->Do_Inst();
    //Añade un minuto de juego a todos los que hay en el campo
    this->AddMin();
    //Reduce el fit a los que estan jugando, si toca...
    this->ReduceFit();
    //Actualizar stats
    this->Update_pts();
    //Eventos (Sin implementar)
    //Primero, determinar qué ocurre con la posesión
    
    //Lesiones
    if(RandT::Bingo(GetVarFrom("Injury", Simu::Injuries))){this->Make_Injury();}
    //Re-checkear en caso de cambios en el resultado, lesiones...
    this->Do_Inst();
    //Cambios forzados (Sin implementar)
  }
  //Final
  this->Write_FT();
}

//Escribe el previo
void partido::Write_Init()
{
  string loc_name = GetStringVarFrom(ali_local->abrev, Simu::Teams);
  string visit_name = GetStringVarFrom(ali_visitante->abrev, Simu::Teams);
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
  //Entrenadores (busca abrev_DT)
  outf << setw(col_w) << GetLeagueDatString(ali_local->abrev+"_DT") << "  |  " << GetLeagueDatString(ali_visitante->abrev+"_DT") << endl;
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

//Ejecuta tanto la local como la visitante
void partido::Do_Inst()
{
  this->Do_Inst(true);
  this->Do_Inst(false);
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
    cumple = cumple*bool(Is_Doable(side, k, i));
  }
  //Si no cumple, nada
  if(!cumple){return;}
  cumple = false;
  //Chequear que el archivo este abierto
  if(!outf.is_open()){outf.open((ali_local->abrev+"_"+ali_visitante->abrev+".txt").c_str(), std::ios_base::app);}
  Simu::Lposition oldpos;
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
        ali->tactica.tac = ali->condicion[k].tactic;
        this->Write_Tactic(ali, ali->tactica.symbol());
      }
      return;
    case Simu::lSUB:
      if(!side)
      {
        if(this->cambios_local < GetLeagueDat("Cambios") && !this->stats_local[ali->condicion[k].arg2-1].hajugado && !this->stats_local[ali->condicion[k].arg1-1].rojas)
        {
          cumple = true;
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
          cumple = true;
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
      oldpos = ali->pos_titulares[ali->condicion[k].arg1-1].pos;
      if(ali->pos_titulares[ali->condicion[k].arg1-1].pos != Simu::lGK)
      {
        ali->pos_titulares[ali->condicion[k].arg1-1].pos =ali->condicion[k].pos;
      }
      //Chequear que cumple los criterios. Si no, revertir
      if(!ali->In_Range())
      {
        ali->pos_titulares[ali->condicion[k].arg1-1].pos = oldpos;
      }
      //Escribe evento
      if(cumple)
      {
        this->Write_Sub(ali, ali->titulares[ali->condicion[k].arg1-1]->Name, ali->suplentes[ali->condicion[k].arg2-N_titulares-1]->Name, ali->pos_titulares[ali->condicion[k].arg1-1].symbol());
      }
      return;
    case Simu::lCHANGEPOS:
      if(ali->pos_titulares[ali->condicion[k].arg1-1].pos == Simu::lGK || ali->condicion[k].pos == Simu::lGK)
      {
        return; //El portero no se puede tocar!
      }
      if(ali->pos_titulares[ali->condicion[k].arg1-1].pos != ali->condicion[k].pos)
      {
        ali->pos_titulares[ali->condicion[k].arg1-1].pos = ali->condicion[k].pos;
        oldpos = ali->pos_titulares[ali->condicion[k].arg1-1].pos;
        //Si no, no se hace el cambio!
        if(ali->In_Range())
        {
          this->Write_ChangePos(ali, ali->titulares[ali->condicion[k].arg1-1]->Name, ali->pos_titulares[ali->condicion[k].arg1-1].symbol());
        }
        else
        {
          ali->pos_titulares[ali->condicion[k].arg1-1].pos = oldpos;
        }
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
    default:
      return(false);
  }
}

//Escribir un cambio //SIN IMPLEMENTAR
void partido::Write_Sub(alineacion* ali, string entra, string sale, string pos)
{
  string format_string = GetRandomText(Simu::Cambios_lang);
  //Hay 3 cosas que sustituir: {entra} {sale} {pos}
  Substitute(format_string, "{entra}", entra);
  Substitute(format_string, "{sale}", sale);
  Substitute(format_string, "{pos}", pos);
  Substitute(format_string, "\\n", "\n          ...  ");
  Write_Event(ali, format_string);
  return;
}

void partido::Write_Tactic(alineacion* ali, string tactica)
{
  string format_string = GetRandomText(Simu::Tacticas_lang);
  //Hay 2 cosas que sustituir: {tactica} y {equipo}
  Substitute(format_string, "{tactica}", tactica);
  Substitute(format_string, "{equipo}", GetStringVarFrom(ali->abrev, Simu::Teams));
  Substitute(format_string, "\\n", "\n          ...  ");
  Write_Event(ali, format_string);
  return;
}

void partido::Write_ChangePos(alineacion* ali, string jug, string pos)
{
  string format_string = GetRandomText(Simu::Chpos_lang);
  //Hay 2 cosas que sustituir: {jugador} y {pos}
  Substitute(format_string, "{jugador}", jug);
  Substitute(format_string, "{pos}", pos);
  Substitute(format_string, "\\n", "\n          ...  ");
  Write_Event(ali, format_string);
  return;
}
void partido::Write_Injury(alineacion* ali, string jug)
{
  string format_string = GetRandomText(Simu::Injury_lang);
  //Hay 1 cosa que sustituir: {jugador}
  Substitute(format_string, "{jugador}", jug);
  Substitute(format_string, "\\n", "\n          ...  ");
  Write_Event(ali, format_string);
  return;
}

//Escribir un evento en cierto minuto
void partido::Write_Event(alineacion* ali, string cosa, bool cont)
{
  //Minuto
  if(!cont)
  {
    outf << "Min. " << minuto << setw(4-int(log10(minuto))) << ":(" << ali->abrev << ") ";
  }
  //O si se continua el evento
  else
  {
    outf << "          ...  ";
  }
  //Cosa
  outf << cosa.c_str() << endl;
}

//Escribir descanso
void partido::Write_HT()
{
  outf << endl;
  outf << "*************  :primeraparte:  ****************" << endl;
  outf << "Resultado al descanso: " << GetStringVarFrom(ali_local->abrev, Simu::Teams) << " " << goles_local << "-" << goles_visitante << " " << GetStringVarFrom(ali_visitante->abrev, Simu::Teams) << endl;
  outf << endl;
}

//Escribir segunda parte
void partido::Write_FT()
{
  outf << endl;
  outf << "*************  :segundaparte:  ****************" << endl;
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

//Quitar fit, el portero no pierde y de momento no depende de la edad
void partido::ReduceFit()
{
  for(int i=1;i<N_titulares;i++)
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

//Eventos random
void partido::Make_Injury()
{
  //Puntero a la alineacion que deba sumarse la lesion
  alineacion* ali;
  jug_stats* stats;
  //Numero aleatorio entre 0 y 2*N_titulares-1
  int rN = rand() % (2*N_titulares);
  //0-10: jug local; 11-21: jug_visitante
  if(bool(rN/N_titulares))
  {
    ali = this->ali_visitante;
    stats = this->stats_visitante;
  }
  else
  {
    ali = this->ali_local;
    stats = this->stats_local;
  }
  //Una vez sabido, se toma el numero del 0 a N_titulares que corresponde
  rN = rN % N_titulares;
  //Se lesiona sólo si está jugando
  if(stats[rN].Is_Playable())
  {
    stats[rN].lesionado = true;
    //Escribir el comentario
    this->Write_Injury(ali, ali->titulares[rN]->Name);
  }
  return;
}

//Printear stats (al final del partido)
void partido::Write_End()
{
  string loc_name = GetStringVarFrom(ali_local->abrev, Simu::Teams);
  string visit_name = GetStringVarFrom(ali_visitante->abrev, Simu::Teams);
  //Separo
  outf << endl << endl;
  //Estadio
  outf << "Detalles del partido" << endl;
  outf << Simu::stat_headline << endl;
  outf << "Campo               " << ": " << GetStringVarFrom(this->ali_local->abrev, Simu::Estadios) << endl;
  outf << Simu::stat_headline << endl;
  //Info local
  outf << "Informacion " << loc_name << " (" << this->ali_local->abrev << ")" << endl;
  outf << Simu::stat_headline << endl;
  //Sin implementar
  outf << "Mejor jugador       : " << "-" << endl;
  outf << "Goleadores          : " << "-" << endl;
  outf << "Expulsados          : " << "-" << endl;
  outf << "Amonestados         : " << "-" << endl;
  outf << "Lesionados          : " << "-" << endl;
  outf << Simu::stat_headline << endl;
  //Info visitante
  outf << "Informacion " << visit_name << " (" << this->ali_visitante->abrev << ")" << endl;
  outf << Simu::stat_headline << endl;
  //Sin implementar
  outf << "Mejor jugador       : " << "-" << endl;
  outf << "Goleadores          : " << "-" << endl;
  outf << "Expulsados          : " << "-" << endl;
  outf << "Amonestados         : " << "-" << endl;
  outf << "Lesionados          : " << "-" << endl;
  outf << Simu::stat_headline << endl;
  //Stats
  outf << "Estadisticas del partido" << endl;
  outf << Simu::stat_headline << endl;
  outf << "                     " <<  loc_name << "  |  " << visit_name << endl;
  outf << "Ocasiones de gol    :"  << setw(loc_name.length()-3) << this->chuts_local << "     |     " << this->chuts_visitante << endl;
  outf << "Disparos a puerta   :"  << setw(loc_name.length()-3) << this->chuts_puerta_local << "     |     " << this->chuts_puerta_visitante << endl;
  outf << "Posesion            :"  << setw(loc_name.length()-3) << this->posesion_local << "     |     " << this->posesion_visitante << endl;
  outf << "Corners             :"  << setw(loc_name.length()-3) << this->corners_local << "     |     " << this->corners_visitante << endl;
  outf << "Amarillas           :"  << setw(loc_name.length()-3) << this->amarillas_local << "     |     " << this->amarillas_visitante << endl;
  outf << "Rojas               :"  << setw(loc_name.length()-3) << this->rojas_local << "     |     " << this->rojas_visitante << endl;
  outf << "Cambios             :"  << setw(loc_name.length()-3) << this->cambios_local << "     |     " << this->cambios_visitante << endl;
  outf << "Faltas              :"  << setw(loc_name.length()-3) << this->faltas_local << "     |     " << this->faltas_visitante << endl;
  outf << "Entradas            :"  << setw(loc_name.length()-3) << this->tackles_local << "     |     " << this->tackles_visitante << endl;
  outf << Simu::stat_headline << endl;
  outf << "Resultado final: " << loc_name << " " << goles_local << "-" << goles_visitante << " " << visit_name << endl;
  outf << Simu::stat_headline << endl;
  outf << endl << endl;
  //Hay que calcular primero la exp ganada
  this->Update_exp(this->stats_local);
  this->Update_exp(this->stats_visitante);
  //Stats individuales local
  WriteStats(this->ali_local, this->stats_local);
  outf << endl;
  //Stats individuales visitante
  WriteStats(this->ali_visitante, this->stats_visitante);
}

void partido::WriteStats(alineacion* ali, jug_stats* stats)
{
  outf << "Estadisticas individuales - " << GetStringVarFrom(ali->abrev, Simu::Teams) << " (" << ali->abrev << ")" << endl;
  outf << Simu::stat_header << endl;
  outf << Simu::stat_headline << endl;
  for(int i=0;i<N_titulares;i++)
  {
    //Habilidades
    outf << ali->titulares[i]->Name << setw(1 + Simu::w_name - ali->titulares[i]->Name.length()) << " "; //Nombre
    outf << setw(3) << ali->pos_titulares[i].symbol() << " "; //Posicion
    outf << setw(3) << ali->titulares[i]->St << " "; //St
    outf << setw(3) << ali->titulares[i]->Tk << " "; //Tk
    outf << setw(3) << ali->titulares[i]->Ps << " "; //Ps
    outf << setw(3) << ali->titulares[i]->Sh << " "; //Sh
    outf << "| ";
    //Llegan los stats
    outf << setw(3) << stats[i].minutos << " "; //Minutos
    outf << setw(3) << stats[i].paradas << " "; //Paradas
    outf << setw(3) << stats[i].tackles << " "; //Tackles
    outf << setw(3) << stats[i].pases << " "; //Pases
    outf << setw(3) << stats[i].asistencias << " "; //Asistencias
    outf << setw(3) << stats[i].chuts << " "; //Disparos
    outf << setw(3) << stats[i].goles << " "; //Goles
    outf << setw(3) << stats[i].amarillas << " "; //Amarillas
    outf << setw(3) << stats[i].rojas << " "; //Rojas
    //Hay que implementar
    outf << setw(3) << stats[i].St << " "; //GK Hab
    outf << setw(3) << stats[i].Tk << " "; //DF Hab
    outf << setw(3) << stats[i].Ps << " "; //MF Hab
    outf << setw(3) << stats[i].Sh << " "; //FW Hab
    outf << endl;
  }
  for(int i=0;i<N_suplentes;i++)
  {
    //Habilidades
    outf << ali->suplentes[i]->Name << setw(1 + Simu::w_name - ali->suplentes[i]->Name.length()) << " "; //Nombre
    outf << setw(3) << ali->pos_suplentes[i].symbol() << " "; //Posicion
    outf << setw(3) << ali->suplentes[i]->St << " "; //St
    outf << setw(3) << ali->suplentes[i]->Tk << " "; //Tk
    outf << setw(3) << ali->suplentes[i]->Ps << " "; //Ps
    outf << setw(3) << ali->suplentes[i]->Sh << " "; //Sh
    outf << "| ";
    //Llegan los stats
    outf << setw(3) << stats[N_titulares+i].minutos << " "; //Minutos
    outf << setw(3) << stats[N_titulares+i].paradas << " "; //Paradas
    outf << setw(3) << stats[N_titulares+i].tackles << " "; //Tackles
    outf << setw(3) << stats[N_titulares+i].pases << " "; //Pases
    outf << setw(3) << stats[N_titulares+i].asistencias << " "; //Asistencias
    outf << setw(3) << stats[N_titulares+i].chuts << " "; //Disparos
    outf << setw(3) << stats[N_titulares+i].goles << " "; //Goles
    outf << setw(3) << stats[N_titulares+i].amarillas << " "; //Amarillas
    outf << setw(3) << stats[N_titulares+i].rojas << " "; //Rojas
    //Hay que implementar
    outf << setw(3) << stats[N_titulares+i].St << " "; //GK Hab
    outf << setw(3) << stats[N_titulares+i].Tk << " "; //DF Hab
    outf << setw(3) << stats[N_titulares+i].Ps << " "; //MF Hab
    outf << setw(3) << stats[N_titulares+i].Sh << " "; //FW Hab
    outf << endl;
  }
}

//Para printear el resultado final
void partido::Print_Result()
{
  cout << "Resultado: " << GetStringVarFrom(ali_local->abrev, Simu::Teams) << " " << goles_local << "-" << goles_visitante << " " << GetStringVarFrom(ali_visitante->abrev, Simu::Teams) << endl;
}

//Update de las plantillas
void partido::Update_Skills(alineacion* ali, jug_stats* stats)
{
  for(int i=0;i<N_titulares;i++)
  {
    ali->titulares[i]->AddHAb(stats[i].St, ali->abrev, "St");
    ali->titulares[i]->AddHAb(stats[i].Tk, ali->abrev, "Tk");
    ali->titulares[i]->AddHAb(stats[i].Ps, ali->abrev, "Ps");
    ali->titulares[i]->AddHAb(stats[i].Sh, ali->abrev, "Sh");
  }
  for(int i=0;i<N_suplentes;i++)
  {
    ali->suplentes[i]->AddHAb(stats[N_titulares+i].St, ali->abrev, "St");
    ali->suplentes[i]->AddHAb(stats[N_titulares+i].Tk, ali->abrev, "Tk");
    ali->suplentes[i]->AddHAb(stats[N_titulares+i].Ps, ali->abrev, "Ps");
    ali->suplentes[i]->AddHAb(stats[N_titulares+i].Sh, ali->abrev, "Sh");
  }
}
//Función para actualizar stats
void partido::Update_Stats(alineacion* ali, jug_stats* stats)
{
  for(int i=0;i<N_titulares;i++)
  {
    //Partido jugado
    ali->titulares[i]->AddGam(int(stats[i].hajugado));
    //Minutos
    ali->titulares[i]->AddMin(int(stats[i].minutos));
    //Man of the match (sin implementar)
    //Paradas
    ali->titulares[i]->AddSav(int(stats[i].paradas));
    //Encajados
    ali->titulares[i]->AddCon(int(stats[i].encajados));
    //Tackles
    ali->titulares[i]->AddKtk(int(stats[i].tackles));
    //Pases
    ali->titulares[i]->AddKps(int(stats[i].pases));
    //Chuts
    ali->titulares[i]->AddSht(int(stats[i].chuts));
    //Goles
    ali->titulares[i]->AddGls(int(stats[i].goles));
    //Asistencias
    ali->titulares[i]->AddAss(int(stats[i].asistencias));
  }
  for(int i=0;i<N_suplentes;i++)
  {
    //Partido jugado
    ali->suplentes[i]->AddGam(int(stats[N_titulares+i].hajugado));
    //Minutos
    ali->suplentes[i]->AddMin(int(stats[N_titulares+i].minutos));
    //Man of the match (sin implementar)
    //Paradas
    ali->suplentes[i]->AddSav(int(stats[N_titulares+i].paradas));
    //Encajados
    ali->suplentes[i]->AddCon(int(stats[N_titulares+i].encajados));
    //Tackles
    ali->suplentes[i]->AddKtk(int(stats[N_titulares+i].tackles));
    //Pases
    ali->suplentes[i]->AddKps(int(stats[N_titulares+i].pases));
    //Chuts
    ali->suplentes[i]->AddSht(int(stats[N_titulares+i].chuts));
    //Goles
    ali->suplentes[i]->AddGls(int(stats[N_titulares+i].goles));
    //Asistencias
    ali->suplentes[i]->AddAss(int(stats[N_titulares+i].asistencias));
  }
}
//Función para resetear el fit al inicial
void partido::Reset_Fitness(alineacion* ali, jug_stats* stats)
{
  //Resetea el fit al fit inicial
  for(int i=0;i<N_titulares;i++)
  {
    ali->titulares[i]->Trd = stats[i].trd_inicial;
  }
  for(int i=0;i<N_suplentes;i++)
  {
    ali->suplentes[i]->Trd = stats[N_titulares+i].trd_inicial;
  }
}
//Para actualizar las lesiones
void partido::Update_Injuries(alineacion* ali, jug_stats* stats)
{
  for(int i=0;i<N_titulares;i++)
  {
    if(stats[i].lesionado)
    {
      ali->titulares[i]->SetInj(ali->abrev);
    }
  }
  for(int i=0;i<N_suplentes;i++)
  {
    if(stats[N_titulares+i].lesionado)
    {
      ali->suplentes[i]->SetInj(ali->abrev);
    }
  }
}
//Para actualizar las sanciones
void partido::Update_Suspensions(alineacion* ali, jug_stats* stats)
{
  for(int i=0;i<N_titulares;i++)
  {
    //Puntos de sancion
    int DPtoday = int(stats[i].amarillas)*GetVarFrom("DP_for_Yellow", Simu::League)+int(stats[i].rojas)*GetVarFrom("DP_for_Suspension", Simu::League);
    ali->titulares[i]->AddDP(DPtoday);
    //Sanciones
    ali->titulares[i]->DetSus(DPtoday, ali->abrev);
  }
  for(int i=0;i<N_suplentes;i++)
  {
    //Puntos de sancion
    int DPtoday = int(stats[N_titulares+i].amarillas)*GetVarFrom("DP_for_Yellow", Simu::League)+int(stats[N_titulares+i].rojas)*GetVarFrom("DP_for_Suspension", Simu::League);
    ali->suplentes[i]->AddDP(DPtoday);
    //Sanciones
    ali->suplentes[i]->DetSus(DPtoday, ali->abrev);
  }
}

//Overloaded :)
void partido::Update_Skills()
{
  this->Update_Skills(this->ali_local, this->stats_local);
  this->Update_Skills(this->ali_visitante, this->stats_visitante);
}
void partido::Update_Stats()
{
  this->Update_Stats(this->ali_local, this->stats_local);
  this->Update_Stats(this->ali_visitante, this->stats_visitante);
}
void partido::Reset_Fitness()
{
  this->Reset_Fitness(this->ali_local, this->stats_local);
  this->Reset_Fitness(this->ali_visitante, this->stats_visitante);
}
void partido::Update_Injuries()
{
  this->Update_Injuries(this->ali_local, this->stats_local);
  this->Update_Injuries(this->ali_visitante, this->stats_visitante);
}
void partido::Update_Suspensions()
{
  this->Update_Suspensions(this->ali_local, this->stats_local);
  this->Update_Suspensions(this->ali_visitante, this->stats_visitante);
}