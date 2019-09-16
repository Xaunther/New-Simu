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

//Devuelve qué opción sale elegida, de entre un array que dice qué probabilidad hay asignada a cada una
int RandT::BingoArray(const double* p, int N)
{
  //Primero, sumar el array para normalizar
  double sumTot = 0;
  for(int i=0;i<N;i++)
  {
    sumTot += p[i];
  }
  //Sacar numero al azar y normalizarlo al total
  double randf = double(rand())/double(RAND_MAX)*sumTot;
  //Buscar cuál ha salido
  int i = 0;
  while(randf>p[i])
  {
    randf -= p[i];
    i++;
  }
  return i;
}