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
  enum Linstruct{lPK, lCPN, lAGG, lTACTIC, lSUB, lCHANGEPOS}; //Tipos de insturcción
  enum Lcondition{lMIN, lSCORE, lSHOTS, lYELLOW, lRED, lINJ}; //Tipos de condiciones
  enum Lsymbol{lLT, lEQ, lGT}; //Menor o igual, igual, mayor o igual
  
  //Constantes
  static const int NTactics = 7;
  static const int NAbilities = 4;
  static const int NPositions = 6;
  static const int NInstructions = 6;
  static const int NConditions = 6;
  static const int NSymbols = 3;
};

#endif
