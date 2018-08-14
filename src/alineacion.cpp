//Implementacion de la clase alineacion
#include <string>
#include <fstream>
#include "alineacion.h"
#include "instruccion.h"
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
  for(int i=0;i<N_titulares;i++)
    {
      fAli >> basura;
      pos_titulares[i].SetPos(basura);
      fAli >> basura;
      names_titulares[i] = basura;
    }
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
  lanza_pen->dump();
  cout << "jugador* capitan: " << capitan << endl;
  capitan->dump();
  cout << "instruccion condicion[N_inst]: " << condicion << endl;
  for(int i=0;i<N_usedinst;i++)
    {
      condicion[i].dump();
    }
  cout << "-------------------------------------" << endl;
}
