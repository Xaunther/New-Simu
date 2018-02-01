//Namespace del simulador
//Aqui van enumeraciones y alguna que otra constante que deba ser considerada

#ifndef Simu_h
#define Simu_h
//Enumeraciones
namespace Simu
{
  enum Ltactic{lA, lD, lN, lL, lC, lP, lE};
  enum Lability{lSt, lTk, lPs, lSh};
  enum Lposition{lGK, lDF, lDM, lMF, lAM, lFW};
  
  //Constantes
  static const int NTactics = 7;
  static const int NAbilities = 4;
  static const int NPositions = 6;
};

#endif
