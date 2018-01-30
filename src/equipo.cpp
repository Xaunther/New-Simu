#include "equipo.h"
#include "jugador.h"
#include "Int_array.h"
#include "textmisc.h"
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

//Funciones de la clase equipo

//El inicializador requiere de un archivo asociado
equipo::equipo(string abrev):
  Njugadores(abrev),
  jug(new jugador[Njugadores.value-2])
{
  Njugadores.value = Njugadores.value-2; //El número de jugadores es el numero de lineas menos 2
  abreviatura = abrev;
  nombre = GetLeagueDatString(abreviatura);
}
void equipo::Clear(){Njugadores.value = 0;}
void equipo::Load()
{
  string filename = abreviatura + ".txt";
  string basura;
  ifstream fichero;
  
  fichero.open(filename.c_str());
  if(!fichero)
    {
      return;
    }
  //Bloque de cálculo (lectura de fichero)
  getline(fichero,basura);
  getline(fichero,basura); //Las dos primeras lineas son encabezados
  //Ya sabemos cuantos jugadores hay
  for(int i=0;i<Njugadores.value;i++)
    {
      //Bloque visible
      fichero >> jug[i].Name;
      fichero >> jug[i].Age;
      fichero >> jug[i].Nat;
      fichero >> jug[i].St;
      fichero >> jug[i].Tk;
      fichero >> jug[i].Ps;
      fichero >> jug[i].Sh;
      fichero >> jug[i].Ag;
      fichero >> jug[i].KAb;
      fichero >> jug[i].TAb;
      fichero >> jug[i].PAb;
      fichero >> jug[i].SAb;
      fichero >> jug[i].Gam;
      fichero >> jug[i].Trd;
      fichero >> jug[i].Min;
      fichero >> jug[i].Mom;
      fichero >> jug[i].Sav;
      fichero >> jug[i].Con;
      fichero >> jug[i].Ktk;
      fichero >> jug[i].Kps;
      fichero >> jug[i].Sht;
      fichero >> jug[i].Gls;
      fichero >> jug[i].Ass;
      fichero >> jug[i].DP;
      fichero >> jug[i].Inj;
      fichero >> jug[i].Sus;
      fichero >> jug[i].Fit;
      //Cosas extras que se anadieron a LMV
      jug[i].MinHoy = 0;
      jug[i].hasPain = false;
      jug[i].Cond = jug[i].Fit*100/max(1,jug[i].Trd);
    }
  fichero.close();
}
void equipo::Save()
{
  //Inicialización de variables
  ofstream fichero;
  string filename = abreviatura + ".txt";
  int i;
  //Apertura del fichero
  fichero.open (filename.c_str());
  
  //Encabezados
  fichero << "Name         Age Nat St Tk Ps Sh Ag KAb TAb PAb SAb Gam Fit  Min Mom Sav Con Ktk Kps Sht Gls Ass  DP Inj Sus Sta" << endl;
  fichero << "----------------------------------------------------------------------------------------------------------------" << endl;
  //Volcado de la información en el fichero
  for (i=0;i<Njugadores.value;i++)
    {
      fichero << left << setfill(' ') << setw(13) << jug[i].Name;
      fichero << right << setfill(' ') << setw(3) << jug[i].Age;
      fichero << setfill(' ') << setw(4) << jug[i].Nat;
      fichero << setfill(' ') << setw(3) << jug[i].St;
      fichero << setfill(' ') << setw(3) << jug[i].Tk;
      fichero << setfill(' ') << setw(3) << jug[i].Ps;
      fichero << setfill(' ') << setw(3) << jug[i].Sh;
      fichero << setfill(' ') << setw(3) << jug[i].Ag;
      fichero << setfill(' ') << setw(4) << jug[i].KAb;
      fichero << setfill(' ') << setw(4) << jug[i].TAb;
      fichero << setfill(' ') << setw(4) << jug[i].PAb;
      fichero << setfill(' ') << setw(4) << jug[i].SAb;
      fichero << setfill(' ') << setw(4) << jug[i].Gam;
      fichero << setfill(' ') << setw(4) << jug[i].Trd;
      fichero << setfill(' ') << setw(5) << jug[i].Min;
      fichero << setfill(' ') << setw(4) << jug[i].Mom;
      fichero << setfill(' ') << setw(4) << jug[i].Sav;
      fichero << setfill(' ') << setw(4) << jug[i].Con;
      fichero << setfill(' ') << setw(4) << jug[i].Ktk;
      fichero << setfill(' ') << setw(4) << jug[i].Kps;
      fichero << setfill(' ') << setw(4) << jug[i].Sht;
      fichero << setfill(' ') << setw(4) << jug[i].Gls;
      fichero << setfill(' ') << setw(4) << jug[i].Ass;
      fichero << setfill(' ') << setw(4) << jug[i].DP;
      fichero << setfill(' ') << setw(4) << jug[i].Inj;
      fichero << setfill(' ') << setw(4) << jug[i].Sus;
      fichero << setfill(' ') << setw(4) << jug[i].Fit << endl;
    }
  
  //Cierre de fichero
  fichero.close ();
}

int equipo::Search(string nombre)
{
  for(int i=0;i<Njugadores.value;i++)
    {
      if(nombre == jug[i].Name)
	{
	  return i;
	}
    }
}

void equipo::ReduceInj()
{
  for(int i=0;i<Njugadores.value;i++)
    {
      jug[i].ReduceInj(abreviatura);
    }
}
void equipo::ReduceSus()
{
  for(int i=0;i<Njugadores.value;i++)
    {
      jug[i].ReduceSus(abreviatura);
    }
}
void equipo::AddTrd(int trdbonus)
{
  for(int i=0;i<Njugadores.value;i++)
    {
      jug[i].AddTrd(trdbonus);
    }
}
void equipo::SetCond()
{
  for(int i=0;i<Njugadores.value;i++)
    {
      jug[i].SetCond();
    }
}
void equipo::SetTrd()
{
  for(int i=0;i<Njugadores.value;i++)
    {
      jug[i].SetTrd();
    }
}
void equipo::ComputeFit()
{
  for(int i=0;i<Njugadores.value;i++)
    {
      jug[i].ComputeFit();
    }
}
string equipo::VetoedGK()
{
  int posbest=0;
  int pos2best=1;
  for(int i=0;i<Njugadores.value;i++)
    {
      if(jug[i].isGK())
	{
	  if(jug[i].St > jug[posbest].St || (jug[i].St == jug[posbest].St && jug[i].Min > jug[posbest].Min))
	    {
	      pos2best = posbest;
	      posbest = i;
	    }
	  else if((jug[i].St > jug[pos2best].St || (jug[i].St == jug[pos2best].St && jug[i].Min > jug[pos2best].Min)) && i!=posbest)
	    {
	      pos2best = i;
	    }
	}
    }
  //Tengo el portero suplente, hay que mirar que esté disponible
  if(jug[pos2best].Sus>0 || jug[pos2best].Inj>0)
    {
      return "";
    }
  else
    {
      return jug[posbest].Name;
    }
}
