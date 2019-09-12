//Implementacion de la clase de tacticas
#include "tactic.h"
#include "Simu.h"
#include <stdlib.h> //Para la funcion exit
#include <iostream>

using namespace std;
using namespace Simu; //Para no tener que escribir TODO EL PUTO RATO Simu::

//Inicializador, para la matriz 3-d
tactic::tactic()
{
  isFilled = false;
  
  effs = new double**[Simu::NTactics];
  for(int i=0;i<Simu::NTactics;i++)
    {
      effs[i] = new double*[Simu::NPositions];
      for(int j=0;j<Simu::NPositions;j++)
	{
	  effs[i][j] = new double[Simu::NAbilities];
	}
    }
}
tactic::tactic(Ltactic _tac)
{
  tactic();
  this->tac = _tac;
}
//Rellenar con los numeritos!
void tactic::Fill()
{
  if(!this->isFilled)
    {
      //Lista de bonus para editar, para no tener que editar tooooda la matriz
      double bonus_list[Simu::NTactics][Simu::NPositions];
      /////////////// Tactica A //////////////
      bonus_list[lA][lDF]=-0.5;bonus_list[lA][lDM]=-0.2;bonus_list[lA][lMF]=-0.2;bonus_list[lA][lAM]=-0.2; bonus_list[lA][lFW]=0.7;
      /////////////// Tactica D //////////////
      bonus_list[lD][lDF]=0.7;bonus_list[lD][lDM]=-0.2;bonus_list[lD][lMF]=-0.2;bonus_list[lD][lAM]=-0.2; bonus_list[lD][lFW]=-0.5;
      /////////////// Tactica N //////////////
      bonus_list[lN][lDF]=0;bonus_list[lN][lDM]=0;bonus_list[lN][lMF]=0;bonus_list[lN][lAM]=0; bonus_list[lN][lFW]=0;
      /////////////// Tactica L //////////////
      bonus_list[lL][lDF]=0.3;bonus_list[lL][lDM]=-0.6;bonus_list[lL][lMF]=-0.6;bonus_list[lL][lAM]=-0.6; bonus_list[lL][lFW]=0.3;
      /////////////// Tactica C //////////////
      bonus_list[lC][lDF]=0.5;bonus_list[lC][lDM]=-0.8;bonus_list[lC][lMF]=-0.8;bonus_list[lC][lAM]=-0.8; bonus_list[lC][lFW]=0.3;
      /////////////// Tactica P //////////////
      bonus_list[lP][lDF]=-0.2;bonus_list[lP][lDM]=0.6;bonus_list[lP][lMF]=0.6;bonus_list[lP][lAM]=0.6; bonus_list[lP][lFW]=-0.4;
      /////////////// Tactica E //////////////
      bonus_list[lE][lDF]=-0.3;bonus_list[lE][lDM]=0.2;bonus_list[lE][lMF]=0.2;bonus_list[lE][lAM]=0.2; bonus_list[lE][lFW]=0.1;
      //Loop over tactics
      for(int i=0;i<Simu::NTactics;i++)
      {
        //Loop over positions
        for(int j=0;j<Simu::NPositions;j++)
        {
          switch(j)
          {
            case lGK:
              effs[i][j][lSt] = 1; effs[i][j][lTk] = 0; effs[i][j][lPs] = 0; effs[i][j][lSh] = 0;
              break;
            case lDF:
              effs[i][j][lSt] = 0; effs[i][j][lTk] = bonus_list[i][j]/3.+1.6; effs[i][j][lPs] = bonus_list[i][j]/3.+0.8; effs[i][j][lSh] = bonus_list[i][j]/3.+0.6;
              break;
            case lDM:
              effs[i][j][lSt] = 0; effs[i][j][lTk] = bonus_list[i][j]/3.+1.0; effs[i][j][lPs] = bonus_list[i][j]/3.+1.3; effs[i][j][lSh] = bonus_list[i][j]/3.+0.7;
              break;
            case lMF:
              effs[i][j][lSt] = 0; effs[i][j][lTk] = bonus_list[i][j]/3.+0.8; effs[i][j][lPs] = bonus_list[i][j]/3.+1.4; effs[i][j][lSh] = bonus_list[i][j]/3.+0.8;
              break;
            case lAM:
              effs[i][j][lSt] = 0; effs[i][j][lTk] = bonus_list[i][j]/3.+0.7; effs[i][j][lPs] = bonus_list[i][j]/3.+1.3; effs[i][j][lSh] = bonus_list[i][j]/3.+1.0;
              break;
            case lFW:
              effs[i][j][lSt] = 0; effs[i][j][lTk] = bonus_list[i][j]/3.+0.6; effs[i][j][lPs] = bonus_list[i][j]/3.+0.8; effs[i][j][lSh] = bonus_list[i][j]/3.+1.6;
              break;
          }
        }
      }  
    }
  this->isFilled = true;
}

double* tactic::GetEffs(Ltactic _tac, Lposition _pos)
{
  if(_tac < 0 || _tac > Simu::NTactics || _pos < 0 || _pos > Simu::NPositions)
    {
      cout << "Posicion o tactica invalidas" << endl;
      exit(1);
    }
  //Comprobar que se haya rellenado la matriz
  if(!this->isFilled)
    {
      this->Fill();
    }
  return effs[_tac][_pos];
}

double* tactic::GetEffs(Lposition _pos)
{
  return GetEffs(this->tac, _pos);
}

void tactic::SetTactic(string tacname)
{
  if(tacname=="A")
    {
      this->tac = lA;
    }
  else if(tacname=="D")
    {
      this->tac = lD;
    }
  else if(tacname=="N")
    {
      this->tac = lN;
    }
  else if(tacname=="L")
    {
      this->tac = lL;
    }
  else if(tacname=="C")
    {
      this->tac = lC;
    }
  else if(tacname=="P")
    {
      this->tac = lP;
    }
  else if(tacname=="E")
    {
      this->tac = lE;
    }
  else
    {
      cout << "Tactica no valida en alineacion" << endl;
      exit(1);
    }
}

string tactic::symbol()
{
  switch(this->tac)
  {
    case lA:
      return "A";
    case lN:
      return "N";
    case lL:
      return "L";
    case lC:
      return "C";
    case lP:
      return "P";
    case lE:
      return "E";
    default:
      return "";
    
  }
}

void tactic::dump()
{
  cout << "-------------------------------------" << endl;
  cout << "bool isFilled " << isFilled << endl;
  cout << "Simu::Ltactic tac " << tac << endl;
  cout << "-------------------------------------" << endl;  
}
