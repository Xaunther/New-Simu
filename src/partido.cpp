#include "partido.h"
#include "alineacion.h"
#include "textmisc.h"
#include "jug_stats.h"
#include "RandomTools.h"
#include "Commentary.h"
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
  ocasiones_local(0), ocasiones_visitante(0),
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
		gk_local+=eff[0]*ali_local->titulares[i]->St*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado)*this->local_boost;
		def_local+=eff[1]*ali_local->titulares[i]->Tk*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado)*double(ali_local->titulares[i]->Trd/100.)*this->local_boost;
		med_local+=eff[2]*ali_local->titulares[i]->Ps*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado)*double(ali_local->titulares[i]->Trd/100.)*this->local_boost;
		atk_local+=eff[3]*ali_local->titulares[i]->Sh*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado)*double(ali_local->titulares[i]->Trd/100.)*this->local_boost;

		//Equipo visitante
		ali_visitante->pos_titulares[i].SetEff(ali_visitante->tactica.tac);
		eff = ali_visitante->pos_titulares[i].ability_eff;
		gk_visitante+=eff[0]*ali_visitante->titulares[i]->St*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado);
		def_visitante+=eff[1]*ali_visitante->titulares[i]->Tk*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado)*double(ali_visitante->titulares[i]->Trd/100.);
		med_visitante+=eff[2]*ali_visitante->titulares[i]->Ps*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado)*double(ali_visitante->titulares[i]->Trd/100.);
		atk_visitante+=eff[3]*ali_visitante->titulares[i]->Sh*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado)*double(ali_visitante->titulares[i]->Trd/100.);
	}
  //Recuperar bonus para cada equipo
  if(ali_local->tactica.Beats(ali_visitante->tactica.tac))
  {
    gk_local *= Simu::tac_bonus;
    def_local *= Simu::tac_bonus;
    med_local *= Simu::tac_bonus;
    atk_local *= Simu::tac_bonus;
  }
  if(ali_visitante->tactica.Beats(ali_local->tactica.tac))
  {
    gk_visitante *= Simu::tac_bonus;
    def_visitante *= Simu::tac_bonus;
    med_visitante *= Simu::tac_bonus;
    atk_visitante *= Simu::tac_bonus;
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
  local_boost = double(GetLeagueDat("Home_Bonus"))/100.;
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
    /////////////////////Eventos (WIP) ////////////////////////////////
    //Primero, determinar qué ocurre con la posesión. Para ello se enfrentan el pase del equipo atacante y la defensa del equipo defensor.
    if(this->FlipPossesion())//Si se mantiene la posesión
    {
      //Sumar a la estadística correspondiente
      this->AddPossesion();
      if(this->FlipOcasion())//Si ocurre ocasión
      {
        //Ejecutar la ocasión de gol
        ExecuteOcasion(this->DetermineOcasion());
      }
      else //Si no hay ocasión, añadir un pase a alguien random del equipo
      {
        int pass_player = ChoosePlayer(this->posesion, Simu::lPs);
        if(!this->posesion)//Local
        {
          this->pases_local++;
          this->stats_local[pass_player].pases++;
        }
        else //Visitante
        {
          this->pases_visitante++;
          this->stats_visitante[pass_player].pases++;
        }
      }
    }
    else //Si se pierde, cambio de posesión y... contraataque?
    {
      this->posesion = !this->posesion;
      this->AddPossesion();
      if(this->FlipOcasion() && this->FlipOcasion())//Si ocurre ocasión. Menor probabilidad para los contragolpes!
      {
        //Ejecutar la ocasión de gol
        ExecuteOcasion(this->DetermineOcasion());
      }
      else //Si no hay ocasión, añadir un pase a alguien random del equipo
      {
        int pass_player = ChoosePlayer(this->posesion, Simu::lPs);
        if(!this->posesion)//Local
        {
          this->pases_local++;
          this->stats_local[pass_player].pases++;
        }
        else //Visitante
        {
          this->pases_visitante++;
          this->stats_visitante[pass_player].pases++;
        }
      }
    }
    /////////////////////////////////////////////////// Eventos ////////////////////////////////////////////////////
    //Lesiones
    if(RandT::Bingo(1./GetVarFrom("Injury", Simu::Injuries))){this->Make_Injury();}
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
  outf << "Ocasiones de gol    :"  << setw(loc_name.length()-3) << this->ocasiones_local << "     |     " << this->ocasiones_visitante << endl;
  outf << "Disparos a puerta   :"  << setw(loc_name.length()-3) << this->chuts_local << "     |     " << this->chuts_visitante << endl;
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
        Commentary::Write_Tactic(ali, ali->tactica.symbol(), this->minuto, this->outf);
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
        Commentary::Write_Sub(ali, ali->titulares[ali->condicion[k].arg1-1]->Name, ali->suplentes[ali->condicion[k].arg2-N_titulares-1]->Name, ali->pos_titulares[ali->condicion[k].arg1-1].symbol(), this->minuto, this->outf);
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
          Commentary::Write_ChangePos(ali, ali->titulares[ali->condicion[k].arg1-1]->Name, ali->pos_titulares[ali->condicion[k].arg1-1].symbol(), this->minuto, this->outf);
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

//Añadir minuto a la estadística de posesión
void partido::AddPossesion()
{
  if(!this->posesion)
      {
        this->posesion_local++;
      }
      else
      {
        this->posesion_visitante++;
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
    Commentary::Write_Injury(ali, ali->titulares[rN]->Name, this->minuto, this->outf);
  }
  return;
}

//Función que devuelve true si se mantiene la posesión o false si se pierde
bool partido::FlipPossesion()
{
  //Variables que contendrán la habilidad del atacante y defensor
  double atk_ab, def_ab;
  if(!this->posesion)//Posesión del local
  {
    atk_ab = med_local;
    def_ab = def_visitante;
  }
  else //Posesión del visitante
  {
    atk_ab = med_visitante;
    def_ab = def_local;
  }
  //Usar la probabilidad asociada :)
  return RandT::Bingo(atk_ab/(atk_ab+def_ab));
}

//Función que devuelve true si hay ocasión y false si no
bool partido::FlipOcasion()
{
  //Variables que contendrán la habilidad del atacante y defensor
  double atk_ab, def_ab;
  if(!this->posesion)//Posesión del local
  {
    atk_ab = atk_local;
    def_ab = def_visitante;
  }
  else //Posesión del visitante
  {
    atk_ab = atk_visitante;
    def_ab = def_local;
  }
  //Usar la probabilidad asociada :)
  return RandT::Bingo(atk_ab/(atk_ab+(1-2.*Simu::ProbOcasion)/(2.*Simu::ProbOcasion)*def_ab));
}

//Función que devuelve el tipo de ocasión que ocurrirá. Cada una de ellas lleva asociada una probabilidad
int partido::DetermineOcasion()
{
  //El tipo de ocasión no depende de las habilidades de nadie, es un array ya dado
  return RandT::BingoArray(Simu::ArrOcasion, Simu::NOcasion);
}

//Función que REALIZA el tipo de ocasión que se le de
void partido::ExecuteOcasion(int oc_index)
{
  if(!this->posesion)
  {
    this->ocasiones_local++;
  }
  else
  {
    this->ocasiones_visitante++;
  }
  //El tipo de ocasión que ocurre ya ha sido determinado. Ahora falta saber los protagonistas y el resultado de la misma
  //De momento, los protagonistas son el rematador, a veces un asistente, el portero y a veces un defensa.
  int fw_index, mf_index, df_index;
  fw_index = ChoosePlayer(this->posesion, Simu::lSh);
  mf_index = ChoosePlayer(this->posesion, Simu::lPs);
  df_index = ChoosePlayer(!this->posesion, Simu::lTk);
  //Realizamos la ocasion que toque
  switch(oc_index)
  {
    case 0: //Mano a mano vs GK
      Oc_vsGK(fw_index, mf_index);
      break;
    case 1: //Mano a mano vs DF
      Oc_vsDF(fw_index, mf_index, df_index);
      break;
    case 2: //Balon parado (de momento corner)
      Oc_Corner(fw_index, mf_index, df_index);
      break;
    case 3: //Chut cercano
      Oc_ChutCercano(fw_index, df_index);
      break;
    case 4: //Chut lejano
      Oc_ChutLejano(fw_index);
      break;
  }
}

//Functión para elegir un jugador de la lista (a más habilidad más probabilidad)
int partido::ChoosePlayer(bool eq_atk, Simu::Lability skill_index)
{
  double skill[N_titulares-1];
  if(!eq_atk) //Mirar en el equipo local
  {
    //Para cada jugador de campo asignar una probabilidad en base a su habilida
    for(int i=1;i<N_titulares;i++)
    {
      switch(skill_index) //Mirar la skill correspondiente
      {
        case Simu::lTk:
          skill[i] = ali_local->pos_titulares[i].ability_eff[skill_index]*ali_local->titulares[i]->Tk*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado)*double(ali_local->titulares[i]->Trd/100.);
          break;
        case Simu::lPs:
          skill[i] = ali_local->pos_titulares[i].ability_eff[skill_index]*ali_local->titulares[i]->Ps*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado)*double(ali_local->titulares[i]->Trd/100.);
          break;
        case Simu::lSh:
          skill[i] = ali_local->pos_titulares[i].ability_eff[skill_index]*ali_local->titulares[i]->Sh*(!this->stats_local[i].rojas)*(!this->stats_local[i].lesionado)*double(ali_local->titulares[i]->Trd/100.);
          break;
      }
    }
  }
  else //Mirar en el visitante
  {
    //Para cada jugador de campo asignar una probabilidad en base a su habilida
    for(int i=1;i<N_titulares;i++)
    {
      switch(skill_index) //Mirar la skill correspondiente
      {
        case Simu::lTk:
          skill[i] = ali_visitante->pos_titulares[i].ability_eff[skill_index]*ali_visitante->titulares[i]->Tk*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado)*double(ali_visitante->titulares[i]->Trd/100.);
          break;
        case Simu::lPs:
          skill[i] = ali_visitante->pos_titulares[i].ability_eff[skill_index]*ali_visitante->titulares[i]->Ps*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado)*double(ali_visitante->titulares[i]->Trd/100.);
          break;
        case Simu::lSh:
          skill[i] = ali_visitante->pos_titulares[i].ability_eff[skill_index]*ali_visitante->titulares[i]->Sh*(!this->stats_visitante[i].rojas)*(!this->stats_visitante[i].lesionado)*double(ali_visitante->titulares[i]->Trd/100.);
          break;
      }
    }
  }
  //Devolver el jugador de campo seleccionado
  return RandT::BingoArray(skill, N_titulares-1) + 1;
}

//Función para cada tipo de ocasión
void partido::Oc_vsGK(int fw_index, int mf_index)
{
  //Variables que contendrán la habilidad del atacante y defensor
  double atk_ab, def_ab;
  if(!this->posesion)//Posesión del local
  {
    atk_ab = ali_local->titulares[fw_index]->Sh*double(ali_local->titulares[fw_index]->Trd/100.)*this->local_boost; //Delantero
    def_ab = ali_visitante->titulares[0]->St; //Portero
    Commentary::Write_OcvsGK(ali_local, ali_local->titulares[fw_index]->Name, ali_local->titulares[mf_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
  }
  else //Posesión del visitante
  {
    atk_ab = ali_visitante->titulares[fw_index]->Sh*double(ali_visitante->titulares[fw_index]->Trd/100.); //Delantero
    def_ab = ali_local->titulares[0]->St*this->local_boost; //Portero
    Commentary::Write_OcvsGK(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_visitante->titulares[mf_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
  }
  
  //Usar la probabilidad asociada :)
  double Prob = Simu::ArrGol[0]/Simu::ArrOcasion[0];
  if(RandT::Bingo(atk_ab/(atk_ab+(1-Prob)/Prob*def_ab)))//Ha sido gol!
  {
    if(!this->posesion) //Gol local
    {
      //Global stats
      this->chuts_local++;
      this->chuts_puerta_local++;
      this->goles_local++;
      //FW stats
      this->stats_local[fw_index].goles++;
      this->stats_local[fw_index].chuts++;
      this->stats_local[fw_index].chuts_puerta++;
      //MF stats
      this->stats_local[mf_index].asistencias++;
      this->stats_local[mf_index].pases++;
      //GK stats
      this->stats_visitante[0].encajados++;
      Commentary::Write_Goal(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
    }
    else //Gol visitante
    {
      //Global stats
      this->chuts_visitante++;
      this->chuts_puerta_visitante++;
      this->goles_visitante++;
      //FW stats
      this->stats_visitante[fw_index].goles++;
      this->stats_visitante[fw_index].chuts++;
      this->stats_visitante[fw_index].chuts_puerta++;
      //MF stats
      this->stats_visitante[mf_index].asistencias++;
      this->stats_visitante[mf_index].pases++;
      //GK stats
      this->stats_local[0].encajados++;
      Commentary::Write_Goal(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
    }
    Commentary::Write_ResChange(this->ali_local, this->ali_visitante, this->goles_local, this->goles_visitante, this->posesion, this->minuto, this->outf);
    //Cambia la posesión al otro equipo
    this->posesion = !this->posesion;
  }
  else //Falló
  {
    if(!this->posesion) //Fallo local
    {
      //Global stats
      this->chuts_local++;
      this->chuts_puerta_local++;
      //FW stats
      this->stats_local[fw_index].chuts++;
      this->stats_local[fw_index].chuts_puerta++;
      //MF stats
      this->stats_local[mf_index].pases++;
      //GK stats
      this->stats_visitante[0].paradas++;
      Commentary::Write_Save(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
    }
    else //Fallo visitante
    {
      //Global stats
      this->chuts_visitante++;
      this->chuts_puerta_visitante++;
      //FW stats
      this->stats_visitante[fw_index].chuts++;
      this->stats_visitante[fw_index].chuts_puerta++;
      //MF stats
      this->stats_visitante[mf_index].pases++;
      //GK stats
      this->stats_local[0].paradas++;
      Commentary::Write_Save(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
    }
    //Ahora qué ocurre?
    switch(RandT::BingoArray(Simu::ArrOutcome, Simu::NOutcome))
    {
      case 0: //Córner
        //ExecuteOcasion(2); //TODO
        break;
      case 1: //Mantiene posesión
        break;
      case 2: //Pierde posesión
        this->posesion = !this->posesion;
        break;
    }
  }
}
void partido::Oc_vsDF(int fw_index, int mf_index, int df_index)
{
  //Variables que contendrán la habilidad del atacante y defensor
  double atk_ab, gk_ab, def_ab;
  if(!this->posesion)//Posesión del local
  {
    atk_ab = ali_local->titulares[fw_index]->Sh*double(ali_local->titulares[fw_index]->Trd/100.)*this->local_boost; //Delantero
    def_ab = ali_visitante->titulares[df_index]->Tk*double(ali_visitante->titulares[df_index]->Trd/100.); //Defensa
    gk_ab = ali_visitante->titulares[0]->St; //Portero
    Commentary::Write_OcvsDF(ali_local, ali_local->titulares[fw_index]->Name, ali_local->titulares[mf_index]->Name, ali_visitante->titulares[df_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
  }
  else //Posesión del visitante
  {
    atk_ab = ali_visitante->titulares[fw_index]->Sh*double(ali_visitante->titulares[fw_index]->Trd/100.); //Delantero
    def_ab = ali_local->titulares[df_index]->Tk*double(ali_local->titulares[df_index]->Trd/100.)*this->local_boost; //Defensa
    gk_ab = ali_local->titulares[0]->St*this->local_boost; //Portero
    Commentary::Write_OcvsDF(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_visitante->titulares[mf_index]->Name, ali_local->titulares[df_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
  }
  //Usar la probabilidad asociada :)
  double Prob = sqrt(Simu::ArrGol[1]/Simu::ArrOcasion[1]);
  bool res_vs_df = RandT::Bingo(atk_ab/(atk_ab+(1-Prob)/Prob*def_ab));
  bool res_vs_gk = RandT::Bingo(atk_ab/(atk_ab+(1-Prob)/Prob*gk_ab));
  if(res_vs_df && res_vs_gk)//Ha sido gol!
  {
    if(!this->posesion) //Gol local
    {
      //Global stats
      this->chuts_local++;
      this->chuts_puerta_local++;
      this->goles_local++;
      //FW stats
      this->stats_local[fw_index].goles++;
      this->stats_local[fw_index].chuts++;
      this->stats_local[fw_index].chuts_puerta++;
      //MF stats
      this->stats_local[mf_index].asistencias++;
      this->stats_local[mf_index].pases++;
      //GK stats
      this->stats_visitante[0].encajados++;
      Commentary::Write_PassvsDF(ali_local, ali_local->titulares[fw_index]->Name, ali_local->titulares[mf_index]->Name, ali_visitante->titulares[df_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
      Commentary::Write_OcChutLejano(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf, true);
      Commentary::Write_Goal(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
    }
    else //Gol visitante
    {
      //Global stats
      this->chuts_visitante++;
      this->chuts_puerta_visitante++;
      this->goles_visitante++;
      //FW stats
      this->stats_visitante[fw_index].goles++;
      this->stats_visitante[fw_index].chuts++;
      this->stats_visitante[fw_index].chuts_puerta++;
      //MF stats
      this->stats_visitante[mf_index].asistencias++;
      this->stats_visitante[mf_index].pases++;
      //GK stats
      this->stats_local[0].encajados++;
      Commentary::Write_PassvsDF(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_visitante->titulares[mf_index]->Name, ali_local->titulares[df_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
      Commentary::Write_OcChutLejano(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf, true);
      Commentary::Write_Goal(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
    }
    Commentary::Write_ResChange(this->ali_local, this->ali_visitante, this->goles_local, this->goles_visitante, this->posesion, this->minuto, this->outf);
    //Cambia la posesión al otro equipo
    this->posesion = !this->posesion;
    return;
  }
  else if(!res_vs_df) //Falló ante el defensa
  {
    if(!this->posesion) //Fallo local
    {
      //MF stats
      this->stats_local[mf_index].pases++;
      //DF stats
      this->stats_visitante[df_index].tackles++;
      Commentary::Write_FailDFvsDF(ali_local, ali_local->titulares[fw_index]->Name, ali_local->titulares[mf_index]->Name, ali_visitante->titulares[df_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
    }
    else //Fallo visitante
    {
      //MF stats
      this->stats_visitante[mf_index].pases++;
      //DF stats
      this->stats_local[df_index].tackles++;
      Commentary::Write_FailDFvsDF(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_visitante->titulares[mf_index]->Name, ali_local->titulares[df_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
    }
  }
  else //Superó al defensa pero el portero ganó
  {
    if(!this->posesion) //Fallo local
    {
      //Global stats
      this->chuts_local++;
      this->chuts_puerta_local++;
      //FW stats
      this->stats_local[fw_index].chuts++;
      this->stats_local[fw_index].chuts_puerta++;
      //MF stats
      this->stats_local[mf_index].pases++;
      //GK stats
      this->stats_visitante[0].paradas++;
      Commentary::Write_PassvsDF(ali_local, ali_local->titulares[fw_index]->Name, ali_local->titulares[mf_index]->Name, ali_visitante->titulares[df_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
      Commentary::Write_OcChutLejano(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf, true);
      Commentary::Write_Save(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
    }
    else //Fallo visitante
    {
      //Global stats
      this->chuts_visitante++;
      this->chuts_puerta_visitante++;
      //FW stats
      this->stats_visitante[fw_index].chuts++;
      this->stats_visitante[fw_index].chuts_puerta++;
      //MF stats
      this->stats_visitante[mf_index].pases++;
      //GK stats
      this->stats_local[0].paradas++;
      Commentary::Write_PassvsDF(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_visitante->titulares[mf_index]->Name, ali_local->titulares[df_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
      Commentary::Write_OcChutLejano(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf, true);
      Commentary::Write_Save(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
      
    }
  }
  //Ahora qué ocurre?
  switch(RandT::BingoArray(Simu::ArrOutcome, Simu::NOutcome))
  {
    case 0: //Córner
      //ExecuteOcasion(2); //To DO
      break;
    case 1: //Mantiene posesión
      break;
    case 2: //Pierde posesión
      this->posesion = !this->posesion;
      break;
  }
}
void partido::Oc_Corner(int fw_index, int mf_index, int df_index)
{
  //Variables que contendrán la habilidad del atacante y defensor
  double atk_ab, gk_ab, def_ab;
  if(!this->posesion)//Posesión del local
  {
    atk_ab = ali_local->titulares[fw_index]->Sh*double(ali_local->titulares[fw_index]->Trd/100.)*this->local_boost; //Delantero
    def_ab = ali_visitante->titulares[df_index]->Tk*double(ali_visitante->titulares[df_index]->Trd/100.); //Defensa
    gk_ab = ali_visitante->titulares[0]->St; //Portero
    this->corners_local++;
    Commentary::Write_OcCorner(ali_local, ali_local->titulares[fw_index]->Name, ali_local->titulares[mf_index]->Name, ali_visitante->titulares[df_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
  }
  else //Posesión del visitante
  {
    atk_ab = ali_visitante->titulares[fw_index]->Sh*double(ali_visitante->titulares[fw_index]->Trd/100.); //Delantero
    def_ab = ali_local->titulares[df_index]->Tk*double(ali_local->titulares[df_index]->Trd/100.)*this->local_boost; //Defensa
    gk_ab = ali_local->titulares[0]->St*this->local_boost; //Portero
    this->corners_visitante++;
    Commentary::Write_OcCorner(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_visitante->titulares[mf_index]->Name, ali_local->titulares[df_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
  }
  //Usar la probabilidad asociada :)
  double Prob = sqrt(Simu::ArrGol[2]/Simu::ArrOcasion[2]);
  bool res_vs_df = RandT::Bingo(atk_ab/(atk_ab+(1-Prob)/Prob*def_ab));
  bool res_vs_gk = RandT::Bingo(atk_ab/(atk_ab+(1-Prob)/Prob*gk_ab));
  if(res_vs_df && res_vs_gk)//Ha sido gol!
  {
    if(!this->posesion) //Gol local
    {
      //Global stats
      this->chuts_local++;
      this->chuts_puerta_local++;
      this->goles_local++;
      //FW stats
      this->stats_local[fw_index].goles++;
      this->stats_local[fw_index].chuts++;
      this->stats_local[fw_index].chuts_puerta++;
      //MF stats
      this->stats_local[mf_index].asistencias++;
      this->stats_local[mf_index].pases++;
      //GK stats
      this->stats_visitante[0].encajados++;
      Commentary::Write_PassCorner(ali_local, ali_local->titulares[fw_index]->Name, ali_local->titulares[mf_index]->Name, ali_visitante->titulares[df_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
      Commentary::Write_Goal(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
    }
    else //Gol visitante
    {
      //Global stats
      this->chuts_visitante++;
      this->chuts_puerta_visitante++;
      this->goles_visitante++;
      //FW stats
      this->stats_visitante[fw_index].goles++;
      this->stats_visitante[fw_index].chuts++;
      this->stats_visitante[fw_index].chuts_puerta++;
      //MF stats
      this->stats_visitante[mf_index].asistencias++;
      this->stats_visitante[mf_index].pases++;
      //GK stats
      this->stats_local[0].encajados++;
      Commentary::Write_PassCorner(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_visitante->titulares[mf_index]->Name, ali_local->titulares[df_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
      Commentary::Write_Goal(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
    }
    Commentary::Write_ResChange(this->ali_local, this->ali_visitante, this->goles_local, this->goles_visitante, this->posesion, this->minuto, this->outf);
    //Cambia la posesión al otro equipo
    this->posesion = !this->posesion;
    return;
  }
  else if(!res_vs_df) //Falló ante el defensa
  {
    if(!this->posesion) //Fallo local
    {
      //MF stats
      this->stats_local[mf_index].pases++;
      //DF stats
      this->stats_visitante[df_index].tackles++;
      Commentary::Write_FailDFCorner(ali_local, ali_local->titulares[fw_index]->Name, ali_local->titulares[mf_index]->Name, ali_visitante->titulares[df_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
    }
    else //Fallo visitante
    {
      //MF stats
      this->stats_visitante[mf_index].pases++;
      //DF stats
      this->stats_local[df_index].tackles++;
      Commentary::Write_FailDFCorner(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_visitante->titulares[mf_index]->Name, ali_local->titulares[df_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
    }
  }
  else //Superó al defensa pero el portero ganó
  {
    if(!this->posesion) //Fallo local
    {
      //Global stats
      this->chuts_local++;
      this->chuts_puerta_local++;
      //FW stats
      this->stats_local[fw_index].chuts++;
      this->stats_local[fw_index].chuts_puerta++;
      //MF stats
      this->stats_local[mf_index].pases++;
      //GK stats
      this->stats_visitante[0].paradas++;
      Commentary::Write_PassCorner(ali_local, ali_local->titulares[fw_index]->Name, ali_local->titulares[mf_index]->Name, ali_visitante->titulares[df_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
      Commentary::Write_Save(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);

    }
    else //Fallo visitante
    {
      //Global stats
      this->chuts_visitante++;
      this->chuts_puerta_visitante++;
      //FW stats
      this->stats_visitante[fw_index].chuts++;
      this->stats_visitante[fw_index].chuts_puerta++;
      //MF stats
      this->stats_visitante[mf_index].pases++;
      //GK stats
      this->stats_local[0].paradas++;
      Commentary::Write_PassCorner(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_visitante->titulares[mf_index]->Name, ali_local->titulares[df_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
      Commentary::Write_Save(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
    }
  }
  //Ahora qué ocurre?
  switch(RandT::BingoArray(Simu::ArrOutcome, Simu::NOutcome))
  {
    case 0: //Córner
      //ExecuteOcasion(2); //TODO
      break;
    case 1: //Mantiene posesión
      break;
    case 2: //Pierde posesión
      this->posesion = !this->posesion;
      break;
  }
}
void partido::Oc_ChutCercano(int fw_index, int df_index)
{
  //Variables que contendrán la habilidad del atacante y defensor
  double atk_ab, gk_ab, def_ab;
  if(!this->posesion)//Posesión del local
  {
    atk_ab = ali_local->titulares[fw_index]->Sh*double(ali_local->titulares[fw_index]->Trd/100.)*this->local_boost; //Delantero
    def_ab = ali_visitante->titulares[df_index]->Tk*double(ali_visitante->titulares[df_index]->Trd/100.); //Defensa
    gk_ab = ali_visitante->titulares[0]->St; //Portero
    Commentary::Write_OcChutCercano(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[df_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
  }
  else //Posesión del visitante
  {
    atk_ab = ali_visitante->titulares[fw_index]->Sh*double(ali_visitante->titulares[fw_index]->Trd/100.); //Delantero
    def_ab = ali_local->titulares[df_index]->Tk*double(ali_local->titulares[df_index]->Trd/100.)*this->local_boost; //Defensa
    gk_ab = ali_local->titulares[0]->St*this->local_boost; //Portero
    Commentary::Write_OcChutCercano(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[df_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
  }
  //Usar la probabilidad asociada :)
  double Prob = sqrt(Simu::ArrGol[3]/Simu::ArrOcasion[3]);
  bool res_vs_df = RandT::Bingo(atk_ab/(atk_ab+(1-Prob)/Prob*def_ab));
  bool res_vs_gk = RandT::Bingo(atk_ab/(atk_ab+(1-Prob)/Prob*gk_ab));
  if(res_vs_df && res_vs_gk)//Ha sido gol!
  {
    if(!this->posesion) //Gol local
    {
      //Global stats
      this->chuts_local++;
      this->chuts_puerta_local++;
      this->goles_local++;
      //FW stats
      this->stats_local[fw_index].goles++;
      this->stats_local[fw_index].chuts++;
      this->stats_local[fw_index].chuts_puerta++;
      //GK stats
      this->stats_visitante[0].encajados++;
      Commentary::Write_PassChutCercano(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[df_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
      Commentary::Write_OcChutLejano(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf, true);
      Commentary::Write_Goal(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
    }
    else //Gol visitante
    {
      //Global stats
      this->chuts_visitante++;
      this->chuts_puerta_visitante++;
      this->goles_visitante++;
      //FW stats
      this->stats_visitante[fw_index].goles++;
      this->stats_visitante[fw_index].chuts++;
      this->stats_visitante[fw_index].chuts_puerta++;
      //GK stats
      this->stats_local[0].encajados++;
      Commentary::Write_PassChutCercano(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[df_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
      Commentary::Write_OcChutLejano(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf, true);
      Commentary::Write_Goal(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
    }
    Commentary::Write_ResChange(this->ali_local, this->ali_visitante, this->goles_local, this->goles_visitante, this->posesion, this->minuto, this->outf);
    //Cambia la posesión al otro equipo
    this->posesion = !this->posesion;
    return;
  }
  else if(!res_vs_df) //Falló ante el defensa
  {
    if(!this->posesion) //Fallo local
    {
      //DF stats
      this->stats_visitante[df_index].tackles++;
      Commentary::Write_FailDFChutCercano(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[df_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
    }
    else //Fallo visitante
    {
      //DF stats
      this->stats_local[df_index].tackles++;
      Commentary::Write_FailDFChutCercano(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[df_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
    }
  }
  else //Superó al defensa pero el portero ganó
  {
    if(!this->posesion) //Fallo local
    {
      //Global stats
      this->chuts_local++;
      this->chuts_puerta_local++;
      //FW stats
      this->stats_local[fw_index].chuts++;
      this->stats_local[fw_index].chuts_puerta++;
      //GK stats
      this->stats_visitante[0].paradas++;
      Commentary::Write_PassChutCercano(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[df_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
      Commentary::Write_OcChutLejano(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf, true);
      Commentary::Write_Save(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
    }
    else //Fallo visitante
    {
      //Global stats
      this->chuts_visitante++;
      this->chuts_puerta_visitante++;
      //FW stats
      this->stats_visitante[fw_index].chuts++;
      this->stats_visitante[fw_index].chuts_puerta++;
      //GK stats
      this->stats_local[0].paradas++;
      Commentary::Write_PassChutCercano(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[df_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
      Commentary::Write_OcChutLejano(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf, true);
      Commentary::Write_Save(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
    }
  }
  //Ahora qué ocurre?
  switch(RandT::BingoArray(Simu::ArrOutcome, Simu::NOutcome))
  {
    case 0: //Córner
      //ExecuteOcasion(2); //TODO
      break;
    case 1: //Mantiene posesión
      break;
    case 2: //Pierde posesión
      this->posesion = !this->posesion;
      break;
  }
}
void partido::Oc_ChutLejano(int fw_index)
{
  //Variables que contendrán la habilidad del atacante y defensor
  double atk_ab, def_ab;
  if(!this->posesion)//Posesión del local
  {
    atk_ab = ali_local->titulares[fw_index]->Sh*double(ali_local->titulares[fw_index]->Trd/100.)*this->local_boost; //Delantero
    def_ab = ali_visitante->titulares[0]->St; //Portero
    Commentary::Write_OcChutLejano(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
  }
  else //Posesión del visitante
  {
    atk_ab = ali_visitante->titulares[fw_index]->Sh*double(ali_visitante->titulares[fw_index]->Trd/100.); //Delantero
    def_ab = ali_local->titulares[0]->St*this->local_boost; //Portero
    Commentary::Write_OcChutLejano(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
  }
  //Usar la probabilidad asociada :)
  double Prob = Simu::ArrGol[4]/Simu::ArrOcasion[4];
  if(RandT::Bingo(atk_ab/(atk_ab+(1-Prob)/Prob*def_ab)))//Ha sido gol!
  {
    if(!this->posesion) //Gol local
    {
      //Global stats
      this->chuts_local++;
      this->chuts_puerta_local++;
      this->goles_local++;
      //FW stats
      this->stats_local[fw_index].goles++;
      this->stats_local[fw_index].chuts++;
      this->stats_local[fw_index].chuts_puerta++;
      //GK stats
      this->stats_visitante[0].encajados++;
      Commentary::Write_Goal(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
    }
    else //Gol visitante
    {
      //Global stats
      this->chuts_visitante++;
      this->chuts_puerta_visitante++;
      this->goles_visitante++;
      //FW stats
      this->stats_visitante[fw_index].goles++;
      this->stats_visitante[fw_index].chuts++;
      this->stats_visitante[fw_index].chuts_puerta++;
      //GK stats
      this->stats_local[0].encajados++;
      Commentary::Write_Goal(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
    }
    Commentary::Write_ResChange(this->ali_local, this->ali_visitante, this->goles_local, this->goles_visitante, this->posesion, this->minuto, this->outf);
    //Cambia la posesión al otro equipo
    this->posesion = !this->posesion;
  }
  else //Falló
  {
    if(!this->posesion) //Fallo local
    {
      //Global stats
      this->chuts_local++;
      this->chuts_puerta_local++;
      //FW stats
      this->stats_local[fw_index].chuts++;
      this->stats_local[fw_index].chuts_puerta++;
      //GK stats
      this->stats_visitante[0].paradas++;
      Commentary::Write_Save(ali_local, ali_local->titulares[fw_index]->Name, ali_visitante->titulares[0]->Name, this->minuto, this->outf);
    }
    else //Fallo visitante
    {
      //Global stats
      this->chuts_visitante++;
      this->chuts_puerta_visitante++;
      //FW stats
      this->stats_visitante[fw_index].chuts++;
      this->stats_visitante[fw_index].chuts_puerta++;
      //GK stats
      this->stats_local[0].paradas++;
      Commentary::Write_Save(ali_visitante, ali_visitante->titulares[fw_index]->Name, ali_local->titulares[0]->Name, this->minuto, this->outf);
    }
    //Ahora qué ocurre?
    switch(RandT::BingoArray(Simu::ArrOutcome, Simu::NOutcome))
    {
      case 0: //Córner
        //ExecuteOcasion(2); //TODO
        break;
      case 1: //Mantiene posesión
        break;
      case 2: //Pierde posesión
        this->posesion = !this->posesion;
        break;
    }
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