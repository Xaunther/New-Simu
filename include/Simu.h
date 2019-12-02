//Namespace del simulador
//Aqui van enumeraciones y alguna que otra constante que deba ser considerada

#ifndef Simu_h
#define Simu_h
#include <string>
//Enumeraciones
namespace Simu
{
enum Ltactic {lA, lD, lN, lL, lC, lP, lE};
enum Lability {lSt, lTk, lPs, lSh};
enum Lposition {lGK, lDF, lDM, lMF, lAM, lFW};
enum Linstruct {lPK, lCPN, lAGG, lTACTIC, lSUB, lCHANGEPOS}; //Tipos de insturcción
enum Lcondition {lMIN, lSCORE, lSHOTS, lYELLOW, lRED, lINJ}; //Tipos de condiciones
enum Lsymbol {lLT, lEQ, lGT}; //Menor o igual, igual, mayor o igual

//Constantes
static const int NTactics = 7;
static const int NAbilities = 4;
static const int NPositions = 6;
static const int NInstructions = 6;
static const int NConditions = 6;
static const int NSymbols = 3;

//Probabilidades
static const int NOcasion = 5;
static const double ArrOcasion[NOcasion] = {2., 2.85, 3.46, 6.62, 6.98};
static const double ProbOcasion = (ArrOcasion[0] + ArrOcasion[1] + ArrOcasion[2] + ArrOcasion[3] + ArrOcasion[4]) / 90.;
static const double ArrGol[NOcasion] = {0.8, 0.52, 0.41, 0.8, 0.29};
static const int NOutcome = 3;
static const double ArrOutcome[NOutcome] = {0.25, 0.25, 0.5};

//Longitud maxima del nombre
static const int w_name = 17;

//Bonus por ventaja táctica
static const double tac_bonus = 1.1;

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
static const std::string Injury_lang = "../config/injury.lang";
static const std::string injury_report_lang = "../config/injury_report.lang";
static const std::string Goal_lang = "../config/goal.lang";
static const std::string OcasionGK_lang = "../config/chance_vsGK.lang";
static const std::string Save_lang = "../config/save.lang";
static const std::string OcasionDF_lang = "../config/chance_vsDF.lang";
static const std::string PassDF_lang = "../config/pass_vsDF.lang";
static const std::string FailDFDF_lang = "../config/failDF_vsDF.lang";
static const std::string OcasionCorner_lang = "../config/chance_Corner.lang";
static const std::string PassCorner_lang = "../config/pass_Corner.lang";
static const std::string FailDFCorner_lang = "../config/failDF_Corner.lang";
static const std::string OcasionChutCercano_lang = "../config/chance_ChutCercano.lang";
static const std::string PassChutCercano_lang = "../config/pass_vsDF.lang";
static const std::string FailDFChutCercano_lang = "../config/failDF_vsDF.lang";
static const std::string OcasionChutLejano_lang = "../config/chance_ChutLejano.lang";
static const std::string Equalizer_lang = "../config/equalizer.lang";
static const std::string LateEqualizer_lang = "../config/lateequalizer.lang";
static const std::string Winner_lang = "../config/winner.lang";
static const std::string LateWinner_lang = "../config/latewinner.lang";
static const std::string Hope_lang = "../config/hope.lang";
static const std::string Consolation_lang = "../config/consolation.lang";
};

#endif
