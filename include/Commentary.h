//Funciones de comentarios, empieza a haber muchas y muy parecidas, así que está bien garrapiñarlas en su propio archivo

#ifndef commentary_h
#define commentary_h

#include "alineacion.h"
#include "jug_stats.h"
#include <fstream>
#include <string>
using namespace std;

namespace Commentary
{
  void Write_Sub(alineacion*, string, string, string, int, ofstream&);
  void Write_ChangePos(alineacion*, string, string, int, ofstream&);
  void Write_Tactic(alineacion*, string, int, ofstream&);
  void Write_Injury(alineacion*, string, int, ofstream&);
  void Write_Goal(alineacion*, string, string, int, ofstream&);
  void Write_ResChange(alineacion*, alineacion*, int, int, bool, int, ofstream&);
  void Write_Save(alineacion*, string, string, int, ofstream&);
  void Write_OcvsGK(alineacion*, string, string, string, int, ofstream&);
  void Write_OcvsDF(alineacion*, string, string, string, string, int, ofstream&);
  void Write_PassvsDF(alineacion*, string, string, string, string, int, ofstream&);
  void Write_FailDFvsDF(alineacion*, string, string, string, string, int, ofstream&);
  void Write_OcCorner(alineacion*, string, string, string, string, int, ofstream&);
  void Write_PassCorner(alineacion*, string, string, string, string, int, ofstream&);
  void Write_FailDFCorner(alineacion*, string, string, string, string, int, ofstream&);
  void Write_OcChutCercano(alineacion*, string, string, string, int, ofstream&);
  void Write_PassChutCercano(alineacion*, string, string, string, int, ofstream&);
  void Write_FailDFChutCercano(alineacion*, string, string, string, int, ofstream&);
  void Write_OcChutLejano(alineacion*, string, string, int, ofstream&, bool = false);
  void Write_Event(alineacion*, string, int, ofstream&, bool = false);
}

#endif /* COMMENTARY_H */

