#include "Commentary.h"
#include "Simu.h"
#include "alineacion.h"
#include "textmisc.h"
#include "jug_stats.h"
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

//Escribir un cambio
void Commentary::Write_Sub(alineacion* ali, string entra, string sale, string pos, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::Cambios_lang);
  //Hay 3 cosas que sustituir: {entra} {sale} {pos}
  Substitute(format_string, "{entra}", entra);
  Substitute(format_string, "{sale}", sale);
  Substitute(format_string, "{pos}", pos);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf);
  return;
}
void Commentary::Write_Tactic(alineacion* ali, string tactica, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::Tacticas_lang);
  //Hay 2 cosas que sustituir: {tactica} y {equipo}
  Substitute(format_string, "{tactica}", tactica);
  Substitute(format_string, "{equipo}", GetStringVarFrom(ali->abrev, Simu::Teams));
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf);
  return;
}
void Commentary::Write_ChangePos(alineacion* ali, string jug, string pos, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::Chpos_lang);
  //Hay 2 cosas que sustituir: {jugador} y {pos}
  Substitute(format_string, "{jugador}", jug);
  Substitute(format_string, "{pos}", pos);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf);
  return;
}
void Commentary::Write_Injury(alineacion* ali, string jug, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::Injury_lang);
  //Hay 1 cosa que sustituir: {jugador}
  Substitute(format_string, "{jugador}", jug);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf);
  return;
}
void Commentary::Write_Goal(alineacion* ali, string shooter, string gk, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::Goal_lang);
  //Hay hasta 2 cosas que sustituir: {goleador} y {portero}
  Substitute(format_string, "{goleador}", shooter);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf, true);
}
void Commentary::Write_Save(alineacion* ali, string shooter, string gk, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::Save_lang);
  //Hay hasta 4 cosas que sustituir: {delantero}, {pasador}, {defensa} y {portero}
  Substitute(format_string, "{delantero}", shooter);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf, true);
}
void Commentary::Write_OcvsGK(alineacion* ali, string shooter, string passer, string gk, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::OcasionGK_lang);
  //Hay hasta 3 cosas que sustituir: {delantero}, {pasador} y {portero}
  Substitute(format_string, "{delantero}", shooter);
  Substitute(format_string, "{pasador}", passer);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf);
}

void Commentary::Write_OcvsDF(alineacion* ali, string shooter, string passer, string deffender, string gk, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::OcasionDF_lang);
  //Hay hasta 4 cosas que sustituir: {delantero}, {pasador}, {defensa} y {portero}
  Substitute(format_string, "{delantero}", shooter);
  Substitute(format_string, "{pasador}", passer);
  Substitute(format_string, "{defensa}", deffender);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf);
}
void Commentary::Write_PassvsDF(alineacion* ali, string shooter, string passer, string deffender, string gk, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::PassDF_lang);
  //Hay hasta 4 cosas que sustituir: {delantero}, {pasador}, {defensa} y {portero}
  Substitute(format_string, "{delantero}", shooter);
  Substitute(format_string, "{pasador}", passer);
  Substitute(format_string, "{defensa}", deffender);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf, true);
}
void Commentary::Write_FailDFvsDF(alineacion* ali, string shooter, string passer, string deffender, string gk, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::FailDFDF_lang);
  //Hay hasta 4 cosas que sustituir: {delantero}, {pasador}, {defensa} y {portero}
  Substitute(format_string, "{delantero}", shooter);
  Substitute(format_string, "{pasador}", passer);
  Substitute(format_string, "{defensa}", deffender);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf, true);
}

void Commentary::Write_OcCorner(alineacion* ali, string shooter, string passer, string deffender, string gk, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::OcasionCorner_lang);
  //Hay hasta 4 cosas que sustituir: {delantero}, {pasador}, {defensa} y {portero}
  Substitute(format_string, "{delantero}", shooter);
  Substitute(format_string, "{pasador}", passer);
  Substitute(format_string, "{defensa}", deffender);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf);
}
void Commentary::Write_PassCorner(alineacion* ali, string shooter, string passer, string deffender, string gk, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::PassCorner_lang);
  //Hay hasta 4 cosas que sustituir: {delantero}, {pasador}, {defensa} y {portero}
  Substitute(format_string, "{delantero}", shooter);
  Substitute(format_string, "{pasador}", passer);
  Substitute(format_string, "{defensa}", deffender);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf, true);
}
void Commentary::Write_FailDFCorner(alineacion* ali, string shooter, string passer, string deffender, string gk, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::FailDFCorner_lang);
  //Hay hasta 4 cosas que sustituir: {delantero}, {pasador}, {defensa} y {portero}
  Substitute(format_string, "{delantero}", shooter);
  Substitute(format_string, "{pasador}", passer);
  Substitute(format_string, "{defensa}", deffender);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf, true);
}

void Commentary::Write_OcChutCercano(alineacion* ali, string shooter, string deffender, string gk, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::OcasionChutCercano_lang);
  //Hay hasta 4 cosas que sustituir: {delantero}, {pasador}, {defensa} y {portero}
  Substitute(format_string, "{delantero}", shooter);
  Substitute(format_string, "{defensa}", deffender);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf);
}
void Commentary::Write_PassChutCercano(alineacion* ali, string shooter, string deffender, string gk, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::PassChutCercano_lang);
  //Hay hasta 4 cosas que sustituir: {delantero}, {pasador}, {defensa} y {portero}
  Substitute(format_string, "{delantero}", shooter);
  Substitute(format_string, "{defensa}", deffender);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf, true);
}
void Commentary::Write_FailDFChutCercano(alineacion* ali, string shooter, string deffender, string gk, int minuto, ofstream& outf)
{
  string format_string = GetRandomText(Simu::FailDFChutCercano_lang);
  //Hay hasta 4 cosas que sustituir: {delantero}, {pasador}, {defensa} y {portero}
  Substitute(format_string, "{delantero}", shooter);
  Substitute(format_string, "{defensa}", deffender);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf, true);
}

void Commentary::Write_OcChutLejano(alineacion* ali, string shooter, string gk, int minuto, ofstream& outf, bool cont)
{
  string format_string = GetRandomText(Simu::OcasionChutLejano_lang);
  //Hay hasta 4 cosas que sustituir: {delantero}, {pasador}, {defensa} y {portero}
  Substitute(format_string, "{delantero}", shooter);
  Substitute(format_string, "{portero}", gk);
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali, format_string, minuto, outf, cont);
}

void Commentary::Write_ResChange(alineacion* ali_local, alineacion* ali_visitante, int goles_local, int goles_visitante, bool who_scored, int minuto, ofstream& outf)
{
  string format_string;
  //Información extra (si se ha empatado, gol de consolación, etc.)
  if(goles_local == goles_visitante) //Alguien ha empatado el partido
  {
    if(minuto < 80)
    {
      format_string = GetRandomText(Simu::Equalizer_lang);
    }
    else //Empató al final
    {
      format_string = GetRandomText(Simu::LateEqualizer_lang);
    }
    if(!who_scored)//Empató el local
    {
      Substitute(format_string, "{equipo1}", GetStringVarFrom(ali_local->abrev, Simu::Teams));
      Substitute(format_string, "{equipo2}", GetStringVarFrom(ali_visitante->abrev, Simu::Teams));
    }
    else//Empató el visitante
    {
      Substitute(format_string, "{equipo1}", GetStringVarFrom(ali_visitante->abrev, Simu::Teams));
      Substitute(format_string, "{equipo2}", GetStringVarFrom(ali_local->abrev, Simu::Teams));
    }
    Substitute(format_string, "\\n", "\n          ...  ");
    Commentary::Write_Event(ali_local, format_string, minuto, outf, true);
  }
  //Alguien se ha adelantado en el marcador
  if((goles_local == goles_visitante+1 && !who_scored) || (goles_local == goles_visitante-1 && who_scored))
  {
    if(minuto < 80)
    {
      format_string = GetRandomText(Simu::Winner_lang);
    }
    else //Empató al final
    {
      format_string = GetRandomText(Simu::LateWinner_lang);
    }
    if(!who_scored)//Empató el local
    {
      Substitute(format_string, "{equipo1}", GetStringVarFrom(ali_local->abrev, Simu::Teams));
      Substitute(format_string, "{equipo2}", GetStringVarFrom(ali_visitante->abrev, Simu::Teams));
    }
    else//Empató el visitante
    {
      Substitute(format_string, "{equipo1}", GetStringVarFrom(ali_visitante->abrev, Simu::Teams));
      Substitute(format_string, "{equipo2}", GetStringVarFrom(ali_local->abrev, Simu::Teams));
    }
    Substitute(format_string, "\\n", "\n          ...  ");
    Commentary::Write_Event(ali_local, format_string, minuto, outf, true);
  }
  //Si se ponen a un gol de distancia
  if((goles_local == goles_visitante-1 && !who_scored) || (goles_local == goles_visitante+1 && who_scored))
  {
    format_string = GetRandomText(Simu::Hope_lang);
    if(!who_scored)//Empató el local
    {
      Substitute(format_string, "{equipo1}", GetStringVarFrom(ali_local->abrev, Simu::Teams));
      Substitute(format_string, "{equipo2}", GetStringVarFrom(ali_visitante->abrev, Simu::Teams));
    }
    else//Empató el visitante
    {
      Substitute(format_string, "{equipo1}", GetStringVarFrom(ali_visitante->abrev, Simu::Teams));
      Substitute(format_string, "{equipo2}", GetStringVarFrom(ali_local->abrev, Simu::Teams));
    }
    Substitute(format_string, "\\n", "\n          ...  ");
    Commentary::Write_Event(ali_local, format_string, minuto, outf, true);
  }
  //Si marcan pero quedan a 3 o más goles
  if((goles_local <= goles_visitante-3 && !who_scored) || (goles_local >= goles_visitante+3 && who_scored))
  {
    format_string = GetRandomText(Simu::Consolation_lang);
    if(!who_scored)//Empató el local
    {
      Substitute(format_string, "{equipo1}", GetStringVarFrom(ali_local->abrev, Simu::Teams));
      Substitute(format_string, "{equipo2}", GetStringVarFrom(ali_visitante->abrev, Simu::Teams));
    }
    else//Empató el visitante
    {
      Substitute(format_string, "{equipo1}", GetStringVarFrom(ali_visitante->abrev, Simu::Teams));
      Substitute(format_string, "{equipo2}", GetStringVarFrom(ali_local->abrev, Simu::Teams));
    }
    Substitute(format_string, "\\n", "\n          ...  ");
    Commentary::Write_Event(ali_local, format_string, minuto, outf, true);
  }
    
  
  //Información del resultado
  format_string = "({abrev_local}) {local} {goles_local} - {goles_visitante} {visitante} ({abrev_visitante})";
  //Convertir el número de goles a strings
  stringstream g_local; g_local << goles_local;
  stringstream g_visitante; g_visitante << goles_visitante;
  //Hay hasta 4 cosas que sustituir: {delantero}, {pasador}, {defensa} y {portero}
  Substitute(format_string, "{local}", GetStringVarFrom(ali_local->abrev, Simu::Teams));
  Substitute(format_string, "{visitante}", GetStringVarFrom(ali_visitante->abrev, Simu::Teams));
  Substitute(format_string, "{abrev_local}", ali_local->abrev);
  Substitute(format_string, "{abrev_visitante}", ali_visitante->abrev);
  Substitute(format_string, "{goles_local}", g_local.str());
  Substitute(format_string, "{goles_visitante}", g_visitante.str());
  Substitute(format_string, "\\n", "\n          ...  ");
  Commentary::Write_Event(ali_local, format_string, minuto, outf, true);
}
//Escribir un evento en cierto minuto
void Commentary::Write_Event(alineacion* ali, string cosa, int minuto, ofstream& outf, bool cont)
{
  //Minuto
  if(!cont)
  {
    outf << "Min. " << minuto << setw(4-int(log10(minuto))) << ":(" << ali->abrev << ") ";
  }
  //O si se continua el evento
  else
  {
    outf << "          ...  ";
  }
  //Cosa
  outf << cosa.c_str() << endl;
}
