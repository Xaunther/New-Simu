//Clase para tacticas y enumeracio para hacerlo mas legible
#ifndef tactic_h
#define tactic_h

using namespace std;

//Enumeracion
enum Ltactic{lA, lD, lN, lL, lC, lP, lE};

class tactic
{
public:
  //Variables
  double*** effs; //Matriz 3-d con todas eficiencias
  bool isFilled;
  //Constantes
  static const int NTactics;
  //Funciones
  void Fill();
};

#endif
