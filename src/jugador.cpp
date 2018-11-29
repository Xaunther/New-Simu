//Funciones de la clase jugador
#include "jugador.h"
#include "textmisc.h"
#include "arraytools.h"
#include "Simu.h"
#include <string>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
using namespace std;

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
  *hab = exptotal/1000; //Parte entera
  *habexp = max(1,exptotal%1000);
  //No se puede pasar de un maximo ni un minimo establecidos en League.dat
  if(this->isPos(habopt))
  {
    if(*hab > GetLeagueDat("Max_Skill"))
    {
      *hab = GetLeagueDat("Max_Skill");
      *habexp = 999;
    }
    else if(*hab < GetLeagueDat("Min_Skill"))
    {
      *hab = GetLeagueDat("Min_Skill");
      *habexp = 1;
    }
  }
  stringstream ss;
  ss << (*hab);
  if((*hab)>hab0)//Ha subido media
    {
      AddSkillschTxt(Name + " (" + abrev + ") sube a " + ss.str() + ".");
    }
  else if((*hab)<hab0)//Ha bajado media
    {
      AddSkillschTxt(Name + " (" + abrev + ") baja a " + ss.str() + ".");
    }
}

void jugador::SetInj(string abrev)
{
	//Obtengo el maximo de jornadas de lesion y la probabilidad de cada una
	int max_inj = GetVarFrom("Max_injury_length", Simu::Injuries);
	int* rangos = GetArrayFrom("Injury_", Simu::Injuries, max_inj+1);
	//Obtengo penalty si la lesion es de 0
	int penalty = GetVarFrom("Fit_Penalty", Simu::Injuries);
	//Cuanto vale la suma
	int sum = SumArray(rangos, max_inj+1);
	//Obtener valor aleatorio entre 1 y la suma
	int randominj = (rand() % sum) + 1;
	stringstream ss;
	//Mirar la tabla
	int part_sum=0;
	for(int i=0;i<7;i++)
	{
		if(randominj > part_sum)
		{
			Inj = i+1;
		}
		part_sum+=rangos[i];
	}
	//Lesion o golpe?
	ss << Inj-1;
	if(Inj == 1)
	{
		Fit = max(0, Fit-penalty);
		AddInjuryTxt(Name + " (" + abrev + ") no se ha lesionado, pero esta dolorido para el proximo partido");
		hasPain = true;
	}
	else
	{
		//Random text y sustituciones
		string inj_report = GetRandomText(Simu::injury_report_lang);
	  Substitute(inj_report, "{jugador}", this->Name);
		Substitute(inj_report, "{abrev}", abrev);
		Substitute(inj_report, "{jornadas}", ss.str());
		//Enchufar al reports
		AddInjuryTxt(inj_report);
	}
}
void jugador::ReduceInj(string abrev)
{
	if (Inj == 1)
	{
		AddInjuryTxt(Name + " (" + abrev + ") vuelve de la lesi�n");
	}
	Inj=max(0,Inj-1);
}
void jugador::ReduceSus(string abrev)
{
	if (Sus == 1)
	{
		AddSuspendTxt(Name + " (" + abrev + ") vuelve de la sanci�n");
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
	//Todos por igual, el cansancio por edad se aplicara durante el partido
	Trd = min(100,Trd+trdbonus);
	if(isGK()) //Porteros siempre descansados
	{
		Trd=100;
	}
}
void jugador::SetCond()
{
 	//Funci�n desde T13 para establecer la condicion del jugador
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
	int Suspension_margin = GetLeagueDat("DP_for_Suspension");
	ostringstream ss;
	//Si ha recibido roja y amarilla
  if(DPtoday>Suspension_margin)
  {
    //Si cumple ciclo de amarillas
    if(this->DP%Suspension_margin == 0)
    {
      Sus = 3;
    }
    else
    {
      Sus = 2;
    }
  }
  //Si es roja sólo
	else if(DPtoday==Suspension_margin)
	{
		Sus = 2;
	}
  //Ciclo de amarillas
	else if(DP%Suspension_margin == 0 && DPtoday > 0) //Multiplo de suspension_margin
	{
		Sus = 2; //1 Jornada de sanci�n
	}
	ss << Sus;
	if(Sus>=2)
	{
		AddSuspendTxt(Name + " (" + abrev + ") est� suspendido para " + ss.str() + " partido");
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
bool jugador::isDF()
{
	if(Tk>=St && Tk>=Ps && St>=Sh)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool jugador::isMF()
{
	if(Ps>=Tk && Ps>=St && Ps>=Sh)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool jugador::isFW()
{
	if(Sh>=Tk && Sh>=Ps && Sh>=St)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool jugador::isPos(string spos)
{
  if(spos == "GK" || spos == "St")
  {
    return this->isGK();
  }
  else if(spos == "DF" || spos == "Tk")
  {
    return this->isDF();
  }
  else if(spos == "MF" || spos == "Ps")
  {
    return this->isMF();
  }
  else if(spos == "FW" || spos == "Sh")
  {
    return this->isFW();
  }
  else
  {
    return false;
  }
}

void jugador::dump()
{
  cout << "-------------------------------------" << endl;
  cout << "string Name " << Name << endl;
  cout << "int Age " << Age << endl;
  cout << "string Nat " << Nat << endl;
  cout << "int St " << St << endl;
  cout << "int Tk " << Tk << endl;
  cout << "int Ps " << Ps << endl;
  cout << "int Sh " << Sh << endl;
  cout << "int Ag " << Ag << endl;
  cout << "int KAb " << KAb << endl;
  cout << "int TAb " << TAb << endl;
  cout << "int PAb " << PAb << endl;
  cout << "int SAb " << SAb << endl;
  cout << "int Gam " << Gam << endl;
  cout << "int Trd " << Trd << endl;
  cout << "int Min " << Min << endl;
  cout << "int Mom " << Mom << endl;
  cout << "int Sav " << Sav << endl;
  cout << "int Con " << Con << endl;
  cout << "int Ktk " << Ktk << endl;
  cout << "int Kps " << Kps << endl;
  cout << "int Sht " << Sht << endl;
  cout << "int Gls " << Gls << endl;
  cout << "int Ass " << Ass << endl;
  cout << "int DP " << DP << endl;
  cout << "int Inj " << Inj << endl;
  cout << "int Sus " << Sus << endl;
  cout << "int Fit " << Fit << endl;
  cout << "int MinHoy " << MinHoy << endl;
  cout << "bool hasPain " << hasPain << endl;
  cout << "int Cond " << Cond << endl;
  cout << "-------------------------------------" << endl;
  
}
