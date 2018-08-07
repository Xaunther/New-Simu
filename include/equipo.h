#ifndef equipo_h
#define equipo_h

#include "jugador.h"
#include <string>
using namespace std;

//Clase equipo
class equipo
{
  //Variables
public:
  jugador* jug; //Array de jugadores
  int Njugadores; //Tamaño del array 
  string abreviatura;
  string nombre;
  //Funciones
  equipo(string);
  void Load();
  void Save();
  void Clear();//Digamos que "vacia" el equipo para volver a ser usado
  int Search(string);
  void ReduceSus();
  void ReduceInj();
  void AddTrd(int);
  void SetTrd();
  void SetCond();
  void ComputeFit();
  string VetoedGK();
};

#endif
