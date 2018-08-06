#ifndef jugador_h
#define jugador_h

#include <string>
using namespace std;

//Clase jugador
class jugador
{
	//Variables
 public:
  string Name;
  int Age;
  string Nat;
  int St;
  int Tk;
  int Ps;
  int Sh;
  int Ag;
  int KAb;
  int TAb;
  int PAb;
  int SAb;
  int Gam;
  int Trd;
  int Min;
  int Mom;
  int Sav;
  int Con;
  int Ktk;
  int Kps;
  int Sht;
  int Gls;
  int Ass;
  int DP;
  int Inj;
  int Sus;
  int Fit;
  int MinHoy;
  bool hasPain;
  int Cond; //Physical condition
  //Funciones
  //Añadir exp, minutos... etc
  void AddHAb(int bonus, string abrev, string habopt);
  void AddGam(int v = 0);
  void AddCond(int v = 0);
  void AddMin(int minplayed);
  void AddMom(int v = 0);
  void AddSav(int saved);
  void AddCon(int conceded);
  void AddKtk(int tacklings);
  void AddKps(int passes);
  void AddSht(int shots);
  void AddGls(int goalsscored);
  void AddAss(int assists);
  void AddDP(int pepepoints);
  void SetInj(string abrev);
  void ReduceInj(string abrev);
  void SetSus(int susweeks);
  void ReduceSus(string abrev);
  void SetTrd();
  void AddTrd(int trdbonus);
  void SetCond();
  void ComputeFit();
  void DetSus(int DPtoday, string abrev);
  bool isGK();
};

#endif
