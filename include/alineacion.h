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
#include "equipo.h"
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
  string* names_titulares;
  position* pos_titulares; //Array de punteros a los 11 titulares
  jugador** suplentes;
  string* names_suplentes;
  position* pos_suplentes; //Array de punteros a los x suplentes (Customizable)
  jugador* lanza_pen;
  jugador* capitan;
  instruccion condicion[N_inst];
  //Funciones
  alineacion(); //Constructor por defecto
  alineacion(string); //Constructor para inicializar leyendo un archivo
  void Leer(string); //Leer archivo de alineacion
  void Link (equipo*); //Para linkear los nombres a jugadores de verdah
  void Check(); //Para comprobar que no haya repes ni lesionados, sancionados
  int** Pos_Range(); //Devuelve la matriz con los limites por posicion
  bool In_Range(); //Comprueba que el numero de jug x posicion respeta los limites
  string Formation(); //Devuelve la formación elegida (4-4-2 P por ejemplo)
  void GK_First(); //Pone al GK en primer lugar del array
  int FindPlayer(Simu::Lposition, bool, int = 0); //Buscar un jugador de una posición concreta, bien titular (true) o suplente (false)
  int FindPlayer_Titular(Simu::Lposition, int = 0); //Buscar en titulares
  int FindPlayer_Suplente(Simu::Lposition, int = 0); //Buscar en suplentes
  void dump(); //Para vomitar todas las variables
};


#endif
