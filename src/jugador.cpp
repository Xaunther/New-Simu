//Funciones de la clase jugador
#include "jugador.h"
#include "textmisc.h"
#include <string>
#include <sstream>
#include <stdlib.h>

void jugador::AddGam(int v){Gam+=v;}
void jugador::AddCond(int v){Cond+=v;}
void jugador::AddMin(int minplayed){Min+=minplayed;}
void jugador::AddMom(int v){Mom+=v;}
void jugador::AddSav(int saved){Sav+=saved;}
void jugador::AddCon(int conceded){Con+=conceded;}
void jugador::AddKtk(int tacklings){Ktk+=tacklings;}
void jugador::AddKps(int passes){Kps+=passes;}
void jugador::AddSht(int shots){Sht+=shots;}
void jugador::AddGls(int goalsscored){Gls+=goalsscored;}
void jugador::AddAss(int assists){Ass+=assists;}
void jugador::AddDP(int pepepoints){DP+=pepepoints;}
void jugador::AddHAb(int bonus, string abrev, string habopt)
{
  int* hab;
  int* habexp;
  //Definimos que habilidad representan los punteros
  if(habopt == "St")
    {
      hab = &St;
      habexp = &KAb;
    }
  else if(habopt == "Tk")
    {
      hab = &Tk;
      habexp = &TAb;
    }
  else if(habopt == "Ps")
    {
      hab = &Ps;
      habexp = &PAb;
    }
  else
    {
      hab = &Sh;
      habexp = &SAb;
    }
  int hab0 = *hab;
  int habexp0 = *habexp;
  int exptotal = hab0*1000+habexp0;
  int add = 0;
  int i;
  double factor1, factor2;
  ostringstream ss, ss2;
  double expbonus_age[6][2]=
    {
      {18,2},
      {21,1.5},
      {25,1},
      {29,0.9},
      {33,0.75},
      {80,0.5}
    };
  //Set age bonus
  for(i=0;Age>expbonus_age[i][0];i++){}
  double expbonus_skill[16]={2,1.5,1.5,1.5,1,1,1,1,0.7,0.6,0.5,0.4,0.3,0.25,0.2,0};
  //Set bonus factors (current skill and next skill level)
  factor1 = expbonus_age[i][1]*expbonus_skill[max(0,(*hab)-15)];
  factor2 = expbonus_age[i][1]*expbonus_skill[min(max(0,(*hab)+1-15),15)];
  if(bonus > 0)
    {
      int add1  = min((1000-(*habexp))/factor1, double(bonus));	
      add = add1*factor1+(bonus-add1)*factor2;
      exptotal += add;
    }
  else
    {
      add = -1*bonus;
      exptotal += bonus;
    }
  *hab = exptotal/1000; //Parte entera
  *habexp = max(1,exptotal%1000);
  ss << (*hab);
  ss2 << add;
  if((*hab)>hab0)//Ha subido media
    {
      AddSkillschTxt(Name + " (" + abrev + ") gana " + ss2.str() + " de XP en " + habopt + " y sube a " + ss.str() + ".");
    }
  else if(bonus > 0)//Gana exp sin subir media
    {
      AddSkillschTxt(Name + " (" + abrev + ") gana " + ss2.str() + " de XP en " + habopt + ".");
    }
  else if((*hab)<hab0)//Ha bajado media
    {
      AddSkillschTxt(Name + " (" + abrev + ") pierde " + ss2.str() + " de XP en " + habopt + " y baja a " + ss.str() + ".");
    }
  else if(bonus < 0)//Pierde exp sin bajar media
    {
      AddSkillschTxt(Name + " (" + abrev + ") pierde " + ss2.str() + " de XP en " + habopt + ".");
    }

}

void jugador::SetInj(string abrev)
{
	//Obtener valor aleatorio entre 1 y 100
	int randominj = (rand() % 100) + 1;
	ostringstream ss;
	int rangos[7] = {1,21,41,55,68,80,91};
	int penalty[3] = {15,16,17};
	int agerange[3] = {23,29,80};
	//Mirar la tabla
	for(int i=0;i<7;i++)
	{
		if(randominj >= rangos[i])
		{
			Inj = i;
		}
	}
	ss << Inj;
	if(Inj == 0)
	{
		int rangestep;
		for(rangestep=0;Age>agerange[rangestep];rangestep++){}
		Fit -= penalty[rangestep];
		AddInjuryTxt(Name + " (" + abrev + ") no se ha lesionado, pero está dolorido para el próximo partido");
		hasPain = true;
	}
	else
	{
		AddInjuryTxt(Name + " (" + abrev + ") está lesionado " + ss.str() + " semanas con " + GetInjuryType());
	}
}
void jugador::ReduceInj(string abrev)
{
	if (Inj == 1)
	{
		AddInjuryTxt(Name + " (" + abrev + ") vuelve de la lesión");
	}
	Inj=max(0,Inj-1);
}
void jugador::ReduceSus(string abrev)
{
	if (Sus == 1)
	{
		AddSuspendTxt(Name + " (" + abrev + ") vuelve de la sanción");
	}
	Sus=max(0,Sus-1);
}
void jugador::SetTrd()
{
	int sta0, sta90, extra;
	//Leo los valores de stamina establecidos a 0 y 90 minutos
	sta0 = GetLeagueDat("FitFactor_0");
	sta90 = GetLeagueDat("FitFactor_90");
	//Bonus de edad
	if(Age >= 33)
	{
		extra = -4;
	}
	else if(Age >= 30)
	{
		extra = -2;
	}
	else if(Age >= 26)
	{
		extra = -1;
	}
	else if(Age >= 22)
	{
		extra = 1;
	}
	else if(Age >= 20)
	{
		extra = 2;
	}
	else
	{
		extra = 4;
	}
	Trd += (sta0+extra) + (MinHoy*(sta90-sta0))/90;
	Trd = max(70,min(Trd, 100));
	if(isGK())
	{
		Trd = 100;
	}
}
void jugador::AddTrd(int trdbonus)
{
	//Tabla para sumar dependiendo de la edad
	int extra;
	if(Age >= 33)
	{
		extra = -4;
	}
	else if(Age >= 30)
	{
		extra = -2;
	}
	else if(Age >= 26)
	{
		extra = -1;
	}
	else if(Age >= 22)
	{
		extra = 1;
	}
	else if(Age >= 20)
	{
		extra = 2;
	}
	else
	{
		extra = 4;
	}
	Trd=max(70,min(100,Trd+trdbonus+extra));
	if(isGK()) //Porteros siempre descansados
	{
		Trd=100;
	}
}
void jugador::SetCond()
{
 	//Función desde T13 para establecer la condicion del jugador
 	//Version igual para todos
  //Si no ha jugado
  	if(MinHoy==0)
	{	
	   	Cond+=-2;
	}
  	//Si no ha jugado mas de 15
  	else if(MinHoy<=15)
	{	
	   	Cond+=-1;
	}
	//Si no ha jugado mas de 30
  	else if(MinHoy<=30)
	{	
	   	Cond+=1;
	}
	//Si no ha jugado mas de 45
  	else if(MinHoy<=45)
	{	
	   	Cond += 2;
	}
	//Si no ha jugado mas de 60
  	else if(MinHoy<=60)
	{	
	   	Cond += 3;
	}
	//Si juega mas de 60
  	else
	{	
	   	Cond += 4;
	}
    //Si ha recibido un golpe se le resta condicion fisica
  if(hasPain)
    {
      Cond-=5;
    }
  Cond = min(100,max(70,Cond));
}
void jugador::ComputeFit()
{
	Fit = max(70, Cond*Trd/100);
	if(isGK() && hasPain==false)
	{
		Fit = 100;
	}
	else if(isGK() && hasPain)
	{
		Fit = 95;
	}
}
void jugador::DetSus(int DPtoday, string abrev)
{
	//Necesitamos saber el ciclo de tarjetas (normalmente son 10 DPs)
	int Suspension_margin = GetLeagueDat("Suspension_margin");
	ostringstream ss;
	//Si ha recibido roja, solo se cuenta la roja
	if(DPtoday==Suspension_margin)
	{
		Sus = 1;
	}
	else if(DP%Suspension_margin == 0) //Multiplo de suspension_margin
	{
		Sus = 1; //1 Jornada de sanción
	}
	ss << Sus;
	if(Sus>=1)
	{
		AddSuspendTxt(Name + " (" + abrev + ") está suspendido para " + ss.str() + " partido");
	}
}
bool jugador::isGK()
{
	if(St>Tk && St>Ps && St>Sh)
	{
		return true;
	}
	else
	{
		return false;
	}
}

