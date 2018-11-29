//Implementacion de la clase alineacion
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "alineacion.h"
#include "equipo.h"
#include "position.h"
#include "Simu.h"
#include "instruccion.h"
#include "arraytools.h"
#include "textmisc.h"
#include "tactic.h"
#include <iostream>
using namespace std;

//Constructor por defecto, solo define las arrays
alineacion::alineacion():
  pos_titulares(new position[N_titulares]),
  names_titulares(new string[N_titulares]),
  titulares(new jugador*[N_titulares]),
  N_suplentes(GetLeagueDat("Suplentes")),
  pos_suplentes(new position[N_suplentes]),
  names_suplentes(new string[N_suplentes]),
  suplentes(new jugador*[N_suplentes])
{
}

//Constructor para leer la alineacion directamente
alineacion::alineacion(string filename):
  pos_titulares(new position[N_titulares]),
  names_titulares(new string[N_titulares]),
  titulares(new jugador*[N_titulares]),
  N_suplentes(GetLeagueDat("Suplentes")),
  pos_suplentes(new position[N_suplentes]),
  names_suplentes(new string[N_suplentes]),
  suplentes(new jugador*[N_suplentes])
{
  this->Leer(filename);
}

//Funcion para leer la alineacion
void alineacion::Leer(string filename)
{
  string basura;
  //Archivo de alis
  ifstream fAli;
  fAli.open(filename.c_str());
  //Primero, abreviatura y tactica
  fAli >> this->abrev;
  fAli >> basura;
  this->tactica.SetTactic(basura);
  //11 titulares
  bool hay_GK = false;
  for(int i=0;i<N_titulares;i++)
    {
      fAli >> basura;
      pos_titulares[i].SetPos(basura);
      if(pos_titulares[i].pos == Simu::lGK)
      {
        if(hay_GK)
        {
          cout << "No puede haber mas de 1 portero!" << endl;
          exit(1);
        }
        hay_GK = true;
      }
      fAli >> basura;
      names_titulares[i] = basura;
    }
  if(!hay_GK)
  {
    cout << "Debe haber 1 portero asignado" << endl;
    exit(1);
  }
  //Para facilitar las cosas, el portero ira siempre en la primera posicion del array
  this->GK_First();
  //X suplentes
  for(int i=0;i<N_suplentes;i++)
    {
      fAli >> basura;
      pos_suplentes[i].SetPos(basura);
      fAli >> basura;
      names_suplentes[i] = basura;      
    }
  //Toca leer las lineas
  //Primero se limpia la ultima linea leida
  getline(fAli, basura);
  //Y ahora se lee hasta llegar al final o quedarnos sin hueco
  int i = 0;
  while(!fAli.eof() && i < N_inst)
    {
      basura = "";;
      getline(fAli, basura);
      if(basura != "")
	{
	  //Y añadimos la condicion al array!
	  this->condicion[i].Fill(basura);
	  i++;
	}
    }
  this->N_usedinst = i;
  fAli.close();
}

//Funcion para linkear los punteros de los jugadores a jugadores de un equipo
void alineacion::Link(equipo* eq)
{
  //Titulares
  for(int i=0;i<N_titulares;i++)
    {
      titulares[i] = &(eq->jug[eq->Search(names_titulares[i])]);
    }
  //Suplentes
  for(int i=0;i<N_suplentes;i++)
    {
      suplentes[i] = &(eq->jug[eq->Search(names_suplentes[i])]);
    }
}

//Funcion para comprobar: Duplicados, lesionados, sancionados
void alineacion::Check()
{
  //Chequeo duplicados
  string* namesAll = new string[N_titulares+this->N_suplentes];
  copy(this->names_titulares, this->names_titulares+N_titulares, namesAll);
  copy(this->names_suplentes, this->names_suplentes+N_suplentes, namesAll+N_titulares);
  if(TieneRepetidos(namesAll, N_titulares+this->N_suplentes))
    {
      cout << "Hay jugadores repetidos en la alineacion" << endl;
      exit(1);
    }
  //Chequeo lesionados/sancionados
  for(int i=0;i<N_titulares;i++)
    {
      //cout << this->titulares[i]->Nat << endl;
      if(this->titulares[i]->Sus > 0 || this->titulares[i]->Inj > 0)
	{
	  cout << "Hay jugadores sancionados/lesionados en la alineacion" << endl;
	  exit(1);
	}
    }
  for(int i=0;i<this->N_suplentes;i++)
    {
      if(this->suplentes[i]->Sus > 0 || this->suplentes[i]->Inj > 0)
	{
	  cout << "Hay jugadores sancionados/lesionados en la alineacion" << endl;
	  exit(1);
	}
    }
  //Chequeo min/max
  if(!this->In_Range())
  {
    cout << "Jugadores por posicion fuera de rango" << endl;
    exit(1);
  }
}

//Funcion que devuelve si el nº de jugadores x posicion esta todo correcto o no
bool alineacion::In_Range()
{
  int** range = this->Pos_Range(); //Rangos
  //Numero de jugadores por posicion
  int N[Simu::NPositions] = {0};
  for(int i=0;i<N_titulares;i++)
  {
    N[(int)pos_titulares[i].pos]++;
  }
  //Trato especial a los MF, que cuenta los 3 tipos dee centrocampista
  N[(int)Simu::lMF] += N[(int)Simu::lAM] + N[(int)Simu::lDM];
  //Si no está en rango, salir
  for(int i=0;i<Simu::NPositions-1;i++)
  {
    if(!(range[i][0] <= N[i+1] && N[i+1] <= range[i][1]))
    {
      return false;
    }
  }
  return true;
}
int** alineacion::Pos_Range()
{
  //Array con los limites minimo y maximo
  int** range = new int*[Simu::NPositions-1];
  for(int i=0;i<Simu::NPositions-1;i++)
  {
    //Inicializo
    range[i] = new int[2];
    position temppos((Simu::Lposition)(i+1));
    //Obtengo min y max, usando el symbol correspondiente :)
    range[i][0] = GetLeagueDat("Min_"+temppos.symbol());
    range[i][1] = GetLeagueDat("Max_"+temppos.symbol());
  }
  return range;
}
//Funcion que devuelve la formacion usada (4-4-2 P por ejemplo)
string alineacion::Formation()
{
  stringstream formation;
  bool usado = false;
  int N[Simu::NPositions] = {0};
  for(int i=0;i<N_titulares;i++)
  {
    N[(int)pos_titulares[i].pos]++;
  }
  for(int i=1;i<Simu::NPositions;i++)
  {
    if(N[i]>0)
    {
      if(!usado)
      {
        formation << N[i];
        usado = true;
      }
      else
      {
        formation << "-" << N[i]; 
      }
    }
  }
  formation << " " << this->tactica.symbol();
  return formation.str();
}

void alineacion::dump()
{
  cout << "-------------------------------------" << endl;
  cout << "string abrev: " << abrev << endl;
  cout << "int N_suplentes: " << N_suplentes << endl;
  cout << "int N_usedinst: " << N_usedinst << endl;
  cout << "tactic tactica: " << endl;
  tactica.dump();
  cout << "jugador** titulares: " << titulares << endl;
  cout << "string* names_titulares: " << names_titulares << endl;
  for(int i=0;i<N_titulares;i++)
    {
      cout << names_titulares[i] << endl;
    }
  cout << "position* pos_titulares: " << pos_titulares << endl;
  for(int i=0;i<N_titulares;i++)
    {
      pos_titulares[i].dump();
    }
  cout << "jugador** suplentes: " << suplentes << endl;
  cout << "string* names_suplentes: " << names_suplentes << endl;
  for(int i=0;i<N_suplentes;i++)
    {
      cout << names_suplentes[i] << endl;
    }
  cout << "position* pos_suplentes: " << pos_suplentes << endl;
  for(int i=0;i<N_suplentes;i++)
    {
      pos_suplentes[i].dump();
    }
  cout << "jugador* lanza_pen: " << lanza_pen << endl;
  cout << "jugador* capitan: " << capitan << endl;
  cout << "instruccion condicion[N_inst]: " << condicion << endl;
  for(int i=0;i<N_usedinst;i++)
    {
      condicion[i].dump();
    }
  cout << "-------------------------------------" << endl;
}

void alineacion::GK_First()
{
  for(int i=0;i<N_titulares;i++)
  {
    if(this->pos_titulares[i].pos == Simu::lGK)
    {
      if(i!=0)
      {
        string tempname;
        Simu::Lposition temppos;
        tempname = this->names_titulares[i];
        temppos = Simu::lGK;
        this->names_titulares[i] = this->names_titulares[0];
        this->pos_titulares[i].pos = this->pos_titulares[0].pos;
        this->names_titulares[0] = tempname;
        this->pos_titulares[0].pos = temppos;
      }
      i = N_titulares;
    }
  }
  return;
}