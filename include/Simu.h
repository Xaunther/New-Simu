//Namespace del simulador
//Aqui van enumeraciones y alguna que otra constante que deba ser considerada

#ifndef Simu_h
#define Simu_h
#include <string>
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
  
  //Longitud maxima del nombre
  static const int w_name = 17;
  //Headers
  static const std::string header        = "";
  static const std::string stat_header   = "Name              Pos  St  Tk  Ps  Sh | Min Sav Ktk Kps Ass Sht Gls Yel Red KAb TAb PAb SAb";
  static const std::string headline      = "";
  static const std::string stat_headline = "-------------------------------------------------------------------------------------------";
  
  //Nombres de archivos
  static const std::string League = "../config/League.dat";
  static const std::string Teams = "../config/Teams.dat";
  static const std::string Hab_bonus = "../config/Hab_bonus.dat";
  static const std::string Estadios = "../config/Estadios.dat";
  static const std::string Injuries = "../config/Injuries.dat";
  //Lang files
  static const std::string Cambios_lang = "../config/cambios.lang";
  static const std::string Chpos_lang = "../config/chpos.lang";
  static const std::string Tacticas_lang = "../config/tacticas.lang";
  static const std::string injury_report_lang = "../config/injury_report.lang";
};

#endif
