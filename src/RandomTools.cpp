//Implementación de las funciones de RandomTools
#include "RandomTools.h"
#include <stdlib.h>
using namespace std;

//Devuelve si hay premio o no, con probabilidad p
bool RandT::Bingo(double p)
{
  double randf = double(rand())/double(RAND_MAX);
  if(randf<p)
  {
    return true;
  }
  else
  {
    return false;
  }
}