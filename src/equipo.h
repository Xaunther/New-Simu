#ifndef equipo_h
#define equipo_h

#include "jugador.h"
#include "constantes.h"
#include <string>
using namespace std;

//Clase equipo
class equipo
{
	//Variables
	public:
	jugador jug[TAM];
	int Njugadores;
	string abreviatura;
	string nombre;
	//Funciones
	equipo(string abrev);
	void Load();
	void Save();
	void Clear();//Digamos que "vacia" el equipo para volver a ser usado
	int Search(string nombre);
	void ReduceSus();
	void ReduceInj();
	void AddTrd(int fitbonus);
	void SetTrd();
	void SetCond();
	void ComputeFit();
	string VetoedGK();
};

#endif
