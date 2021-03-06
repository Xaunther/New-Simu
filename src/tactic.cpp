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
      /////////////// Tactica A //////////////
      effs[lA][lGK][lSt] = 1; effs[lA][lGK][lTk] = 0; effs[lA][lGK][lPs] = 0; effs[lA][lGK][lSh] = 0;
      effs[lA][lDF][lSt] = 0; effs[lA][lDF][lTk] = 1; effs[lA][lDF][lPs] = 1; effs[lA][lDF][lSh] = 1;
      effs[lA][lDM][lSt] = 0; effs[lA][lDM][lTk] = 1; effs[lA][lDM][lPs] = 1; effs[lA][lDM][lSh] = 1;
      effs[lA][lMF][lSt] = 0; effs[lA][lMF][lTk] = 1; effs[lA][lMF][lPs] = 1; effs[lA][lMF][lSh] = 1;
      effs[lA][lAM][lSt] = 0; effs[lA][lAM][lTk] = 1; effs[lA][lAM][lPs] = 1; effs[lA][lAM][lSh] = 1;
      effs[lA][lFW][lSt] = 0; effs[lA][lFW][lTk] = 1; effs[lA][lFW][lPs] = 1; effs[lA][lFW][lSh] = 1;
      /////////////// Tactica D //////////////
      effs[lD][lGK][lSt] = 1; effs[lD][lGK][lTk] = 0; effs[lD][lGK][lPs] = 0; effs[lD][lGK][lSh] = 0;
      effs[lD][lDF][lSt] = 0; effs[lD][lDF][lTk] = 1; effs[lD][lDF][lPs] = 1; effs[lD][lDF][lSh] = 1;
      effs[lD][lDM][lSt] = 0; effs[lD][lDM][lTk] = 1; effs[lD][lDM][lPs] = 1; effs[lD][lDM][lSh] = 1;
      effs[lD][lMF][lSt] = 0; effs[lD][lMF][lTk] = 1; effs[lD][lMF][lPs] = 1; effs[lD][lMF][lSh] = 1;
      effs[lD][lAM][lSt] = 0; effs[lD][lAM][lTk] = 1; effs[lD][lAM][lPs] = 1; effs[lD][lAM][lSh] = 1;
      effs[lD][lFW][lSt] = 0; effs[lD][lFW][lTk] = 1; effs[lD][lFW][lPs] = 1; effs[lD][lFW][lSh] = 1;
      /////////////// Tactica N //////////////
      effs[lN][lGK][lSt] = 1; effs[lN][lGK][lTk] = 0; effs[lN][lGK][lPs] = 0; effs[lN][lGK][lSh] = 0;
      effs[lN][lDF][lSt] = 0; effs[lN][lDF][lTk] = 1; effs[lN][lDF][lPs] = 1; effs[lN][lDF][lSh] = 1;
      effs[lN][lDM][lSt] = 0; effs[lN][lDM][lTk] = 1; effs[lN][lDM][lPs] = 1; effs[lN][lDM][lSh] = 1;
      effs[lN][lMF][lSt] = 0; effs[lN][lMF][lTk] = 1; effs[lN][lMF][lPs] = 1; effs[lN][lMF][lSh] = 1;
      effs[lN][lAM][lSt] = 0; effs[lN][lAM][lTk] = 1; effs[lN][lAM][lPs] = 1; effs[lN][lAM][lSh] = 1;
      effs[lN][lFW][lSt] = 0; effs[lN][lFW][lTk] = 1; effs[lN][lFW][lPs] = 1; effs[lN][lFW][lSh] = 1;
      /////////////// Tactica L //////////////
      effs[lL][lGK][lSt] = 1; effs[lL][lGK][lTk] = 0; effs[lL][lGK][lPs] = 0; effs[lL][lGK][lSh] = 0;
      effs[lL][lDF][lSt] = 0; effs[lL][lDF][lTk] = 1; effs[lL][lDF][lPs] = 1; effs[lL][lDF][lSh] = 1;
      effs[lL][lDM][lSt] = 0; effs[lL][lDM][lTk] = 1; effs[lL][lDM][lPs] = 1; effs[lL][lDM][lSh] = 1;
      effs[lL][lMF][lSt] = 0; effs[lL][lMF][lTk] = 1; effs[lL][lMF][lPs] = 1; effs[lL][lMF][lSh] = 1;
      effs[lL][lAM][lSt] = 0; effs[lL][lAM][lTk] = 1; effs[lL][lAM][lPs] = 1; effs[lL][lAM][lSh] = 1;
      effs[lL][lFW][lSt] = 0; effs[lL][lFW][lTk] = 1; effs[lL][lFW][lPs] = 1; effs[lL][lFW][lSh] = 1;
      /////////////// Tactica C //////////////
      effs[lC][lGK][lSt] = 1; effs[lC][lGK][lTk] = 0; effs[lC][lGK][lPs] = 0; effs[lC][lGK][lSh] = 0;
      effs[lC][lDF][lSt] = 0; effs[lC][lDF][lTk] = 1; effs[lC][lDF][lPs] = 1; effs[lC][lDF][lSh] = 1;
      effs[lC][lDM][lSt] = 0; effs[lC][lDM][lTk] = 1; effs[lC][lDM][lPs] = 1; effs[lC][lDM][lSh] = 1;
      effs[lC][lMF][lSt] = 0; effs[lC][lMF][lTk] = 1; effs[lC][lMF][lPs] = 1; effs[lC][lMF][lSh] = 1;
      effs[lC][lAM][lSt] = 0; effs[lC][lAM][lTk] = 1; effs[lC][lAM][lPs] = 1; effs[lC][lAM][lSh] = 1;
      effs[lC][lFW][lSt] = 0; effs[lC][lFW][lTk] = 1; effs[lC][lFW][lPs] = 1; effs[lC][lFW][lSh] = 1;
      /////////////// Tactica P //////////////
      effs[lP][lGK][lSt] = 1; effs[lP][lGK][lTk] = 0; effs[lP][lGK][lPs] = 0; effs[lP][lGK][lSh] = 0;
      effs[lP][lDF][lSt] = 0; effs[lP][lDF][lTk] = 1; effs[lP][lDF][lPs] = 1; effs[lP][lDF][lSh] = 1;
      effs[lP][lDM][lSt] = 0; effs[lP][lDM][lTk] = 1; effs[lP][lDM][lPs] = 1; effs[lP][lDM][lSh] = 1;
      effs[lP][lMF][lSt] = 0; effs[lP][lMF][lTk] = 1; effs[lP][lMF][lPs] = 1; effs[lP][lMF][lSh] = 1;
      effs[lP][lAM][lSt] = 0; effs[lP][lAM][lTk] = 1; effs[lP][lAM][lPs] = 1; effs[lP][lAM][lSh] = 1;
      effs[lP][lFW][lSt] = 0; effs[lP][lFW][lTk] = 1; effs[lP][lFW][lPs] = 1; effs[lP][lFW][lSh] = 1;
      /////////////// Tactica E //////////////
      effs[lE][lGK][lSt] = 1; effs[lE][lGK][lTk] = 0; effs[lE][lGK][lPs] = 0; effs[lE][lGK][lSh] = 0;
      effs[lE][lDF][lSt] = 0; effs[lE][lDF][lTk] = 1; effs[lE][lDF][lPs] = 1; effs[lE][lDF][lSh] = 1;
      effs[lE][lDM][lSt] = 0; effs[lE][lDM][lTk] = 1; effs[lE][lDM][lPs] = 1; effs[lE][lDM][lSh] = 1;
      effs[lE][lMF][lSt] = 0; effs[lE][lMF][lTk] = 1; effs[lE][lMF][lPs] = 1; effs[lE][lMF][lSh] = 1;
      effs[lE][lAM][lSt] = 0; effs[lE][lAM][lTk] = 1; effs[lE][lAM][lPs] = 1; effs[lE][lAM][lSh] = 1;
      effs[lE][lFW][lSt] = 0; effs[lE][lFW][lTk] = 1; effs[lE][lFW][lPs] = 1; effs[lE][lFW][lSh] = 1;      
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
  }
}

void tactic::dump()
{
  cout << "-------------------------------------" << endl;
  cout << "bool isFilled " << isFilled << endl;
  cout << "Simu::Ltactic tac " << tac << endl;
  cout << "-------------------------------------" << endl;  
}
