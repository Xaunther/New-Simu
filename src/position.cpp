//Funciones de la clase jugador
#include <string>
#include "position.h"
#include "ability.h"
using namespace std;

position::position()
{
  //Inicializar el array de 2 dimensiones con las eficiencias por hab y tactica
  ability_eff = new double*[ability_eff];
  for(int i=0;i<ability::NAbilities;i++)
    {
      ability_eff[i] = new double[tactic::NTactics];
    }
}
