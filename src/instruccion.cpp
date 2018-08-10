//Implementacion de la clase instruccion
#include "instruccion.h"
#include "alineacion.h"
#include "Simu.h"
#include "textmisc.h"
#include <string>
#include <sstream>
#include <stdlib.h>
#include <iostream>
using namespace std;

//Default
instruccion::instruccion() : N_usedcond(0) {}

//Si dan una string como argumento, se supone que es la condicion leida de la ali
instruccion::instruccion(string condline) : N_usedcond(0)
{
  this->Fill(condline);
}
//A Rellenar a partir de string
void instruccion::Fill(string condline)
{
  stringstream ss;
  string basura;
  
  //Paso la linea a stringstream y voy leyendo palabra a palabra
  ss.str(condline);

  //Primero, tipo de instruccion
  ss >> basura;
  this->SetType(basura);  
  //Segundo, argumentos
  if(this->type==Simu::lSUB)
    {
      ss >> this->arg1;
      ss >> this->arg2;
      ss >> basura;
      this->SetPos(basura);
    }
  else if(this->type==Simu::lTACTIC)
    {
      ss >> basura;
      this->SetTactic(basura);
    }
  else if(this->type==Simu::lCHANGEPOS)
    {
      ss >> this->arg1;
      ss >> basura;
      this->SetPos(basura);
    }
  //Empezar con las condiciones, si hay
  ss >> basura;
  if(basura=="IF")
    {
      ss >> basura;
      int i=0;
      while(!ss.eof() && i < N_cond)
	{
	  this->SetCondition(i, basura);
	  //Algunas operaciones requieren simbolo, otras no
	  if((int)this->cond[i] < 3) //MIN, SCORE, SHOTS
	    {
	      ss >> basura;
	      this->SetSymbol(i, basura);
	    }
	  else //Si no requiere, por defecto se usa el igual
	    {
	      this->SetSymbol(i, "=");
	    }
	  //Finalmente, valorcillo con truquin
	  int valor_init = this->cond_value[i];
	  ss >> this->cond_value[i];
	  this->cond_value[i] += valor_init;
	  //SIGUIENTE
	  i++;
	}
      this->N_usedcond = i;
    }
  else if(basura=="")//No hay condiciones, el comando se aplica al inicio del partido (minuto 0)
    {
      this->SetCondition(0, "MIN");
      this->SetSymbol(0, "=");
      this->N_usedcond = 1;
    }
  else //Algo ha ido mal
    {
      this->Exit();
    }
  //Finalmente, se checkea que todo esta correcto y tiene sentido
  this->Check();
}

//Salir del programa
void instruccion::Exit()
{
  cout << "Instruccion invalida" << endl;
  exit(1);
}

//Tipo instruccion
void instruccion::SetType(string s_type)
{
  if(s_type=="PK")
    {
      this->type = Simu::lPK;
    }
  else if(s_type=="CPN")
    {
      this->type = Simu::lCPN;
    }
  else if(s_type=="AGG")
    {
      this->type = Simu::lAGG;
    }
  else if(s_type=="TACTIC")
    {
      this->type = Simu::lTACTIC;
    }
  else if(s_type=="SUB")
    {
      this->type = Simu::lSUB;
    }
  else if(s_type=="CHANGEPOS")
    {
      this->type = Simu::lCHANGEPOS;
    }
  else
    {
      this->Exit();
    }
}

//Tactica requerida
void instruccion::SetTactic(string s_tactic)
{
  if(s_tactic=="A")
    {
      this->tactic = Simu::lA;
    }
  else if(s_tactic=="D")
    {
      this->tactic = Simu::lD;
    }
  else if(s_tactic=="N")
    {
      this->tactic = Simu::lN;
    }
  else if(s_tactic=="L")
    {
      this->tactic = Simu::lL;
    }
  else if(s_tactic=="C")
    {
      this->tactic = Simu::lC;
    }
  else if(s_tactic=="P")
    {
      this->tactic = Simu::lP;
    }
  else if(s_tactic=="E")
    {
      this->tactic = Simu::lE;
    }
  else //Tactica no reconocida
    {
      this->Exit();
    }  
}

//Posicion requerida
void instruccion::SetPos(string s_pos)
{
  if(s_pos=="GK")
    {
      this->pos = Simu::lGK;
    }
  else if(s_pos=="DF")
    {
      this->pos = Simu::lDF;
    }
  else if(s_pos=="DM")
    {
      this->pos = Simu::lDM;
    }
  else if(s_pos=="MF")
    {
      this->pos = Simu::lMF;
    }
  else if(s_pos=="AM")
    {
      this->pos = Simu::lAM;
    }
  else if(s_pos=="FW")
    {
      this->pos = Simu::lFW;
    }
  else
    {
      this->Exit();
    }
}

//Tipo de condicion requerida para la numero x
void instruccion::SetCondition(int x, string s_cond)
{
  if(s_cond=="MIN")
    {
      this->cond[x] = Simu::lMIN;
    }
  else if(s_cond=="SCORE")
    {
      this->cond[x] = Simu::lSCORE;
    }
  else if(s_cond=="SHOTS")
    {
      this->cond[x] = Simu::lSHOTS;
    }
  else if(s_cond=="YELLOW")
    {
      this->cond[x] = Simu::lYELLOW;
    }
  else if(s_cond=="RED")
    {
      this->cond[x] = Simu::lRED;
    }
  else if(s_cond=="INJ")
    {
      this->cond[x] = Simu::lINJ;
    }
  else
    {
      this->Exit();
    }
}

//Simbolo usado
void instruccion::SetSymbol(int x, string s_symbol)
{
  //Se admiten todos los simbolos, no como en el viejo y ronoso simulador de barcos y putas
  //Pero con un truquin
  if(s_symbol=="<=")
    {
      this->cond_value[x] = 0;
      this->symbol[x] = Simu::lLT;
    }
  else if(s_symbol=="=")
    {
      this->cond_value[x] = 0;
      this->symbol[x] = Simu::lEQ;
    }
  else if(s_symbol==">=")
    {
      this->cond_value[x] = 0;
      this->symbol[x] = Simu::lGT;
    }
  else if(s_symbol=="<")
    {
      this->cond_value[x] = -1;
      this->symbol[x] = Simu::lLT;
    }
  else if(s_symbol==">")
    {
      this->cond_value[x] = 1;
      this->symbol[x] = Simu::lGT;
    }
  else
    {
      this->Exit();
    }
}

//Funcion para comprobar que las variables guardadas tienen sentido
void instruccion::Check()
{
  //Chequeo argumentos, esquema:
  /* arg1
    Si se usa PK, CPN, SUB, CHANGEPOS
    Debe ser un numero entre 1 y 11 (un jug. titular)
    Cualquier otra cosa, da igual porque no se usara.

    arg2
    Si se usa SUB
    Debe ser un numero entre 12 y 11+x, donde x es el numero de suplentes
    Cualquier otra cosa, inutil
   */
  if(this->type==Simu::lPK || this->type==Simu::lCPN || this->type==Simu::lSUB || this->type==Simu::lCHANGEPOS)
    {
      if(this->arg1<1 || this->arg1>11)
	{
	  this->Exit();
	}
    }
  if(this->type==Simu::lSUB)
    {
      int N_sup = GetLeagueDat("Suplentes");
      if(this->arg2<12 || this->arg2>11+N_sup)
	{
	  this->Exit();
	}
    }
}

void instruccion::dump()
{
  
}
