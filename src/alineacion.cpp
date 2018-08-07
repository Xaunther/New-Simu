//Implementacion de la clase alineacion
#include <string>
#include <fstream>
#include "alineacion.h"
#include "textmisc.h"
#include "tactic.h"
using namespace std;

//Constructor por defecto, solo define las arrays
alineacion::alineacion()
{
  //Cuantos suplentes hacen falta
  N_suplentes = GetLeagueDat("Suplentes");
  **this->suplentes = new jugador*[N_suplentes];
  *this->pos_suplentes = new position[N_suplentes];
  //Defino array de titulares
  **this->titulares = new jugador*[N_titulares];
  *this->pos_titulares = new position[N_titulares];
}

//Constructor para leer la alineacion directamente
alineacion::alineacion(string filename)
{
  //Inicializo arrays
  alineacion::alineacion();
  alineacion::Leer(filename);
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
      titulares[i]->Name = basura;
    }
  //X suplentes
  for(int i=0;i<N_suplentes;i++)
    {
      fAli >> basura;
      pos_suplentes[i].SetPos(basura);
      fAli >> basura;
      suplentes[i]->Name = basura;      
    }
  fAli.close();
}
