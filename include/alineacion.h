//Header files para la alineación
/************************************
Contiene 11 jugadores titulares, con su posicion en el campo
Contiene x jugadores suplentes, con su posicion recomendada en el campo
Contiene roles: Lanzador penalties, capitan, faltas?
Contiene abreviatura equipo
Contiene Tactica inicial
Contiene Estrategia del equipo
*************************************/
#ifndef alineacion_h
#define alineacion_h

#include <string>
#include "jugador.h"
#include "Simu.h"
#include "tactic.h"
#include "position.h"
#include "instruccion.h"
using namespace std;

//Constantes
#define N_titulares 11
#define N_inst 64

class alineacion
{
  //Variables
public:
  string abrev;
  int N_suplentes;
  int N_usedinst;
  tactic tactica;
  jugador** titulares;
  position* pos_titulares; //Array de punteros a los 11 titulares
  jugador** suplentes;
  position* pos_suplentes; //Array de punteros a los x suplentes (Customizable)
  jugador* lanza_pen;
  jugador* capitan;
  instruccion condicion[N_inst];
  //Funciones
  alineacion(); //Constructor por defecto
  alineacion(string); //Constructor para inicializar leyendo un archivo
  void Leer(string); //Leer archivo de alineacion
  void dump(); //Para vomitar todas las variables
};


#endif
