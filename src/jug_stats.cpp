#include "jug_stats.h"
using namespace std;

//Inicializador por defecto, todo a cero
jug_stats::jug_stats():
  goles(0),
  asistencias(0),
  chuts(0),
  pases(0),
  tackles(0),
  paradas(0),
  rojas(false),
  amarillas(false),
  minutos(0),
  faltas(0),
  hajugado(false)
{
}