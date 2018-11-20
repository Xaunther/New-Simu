//Implementacion de las funciones

#include "textmisc.h"
#include "Simu.h"
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <ctime>
using namespace std;

string GetInjuryType()
{
  //Obtener valor aleatorio entre 0 y 9
  int index = (rand() % 10);
  string types[10] = 
    {
      "diarrea extrema :truno:",
      "bolas azules",
      "una pierna amputada",
      "un flechazo en la rodilla",
      "un codo rasgunado",
      "una fumada del 15 :weed:",
      "cancer testicular",
      "cirugia plastica",
      "un leve resfriado",
      "un tentaculo roto"
    };
  return types[index];
}

void AddSuspendTxt(string newline)
{
  ifstream fin;
  ofstream fout;
  string content = "";
  string basura;
  //Open file and load it into a string
  fin.open("suspend.txt");
  if(fin)
    {
      while(!fin.eof())
	{
	  getline(fin, basura);
	  content+=basura + "\n";
	}
    }
  //add newline
  fin.close();
  content += newline;
  
  //Save it in file again
  fout.open("suspend.txt");
  fout << content;
  fout.close();
}
void AddInjuryTxt(string newline)
{
  ifstream fin;
  ofstream fout;
  string content = "";
  string basura;
  //Open file and load it into a string
  fin.open("injury.txt");
  if(fin)
    {
      while(!fin.eof())
	{
	  getline(fin, basura);
	  content+=basura;
	  if(!fin.eof())
	    {
	      content+="\n";
	    }
	}
    }
  fin.close();
  if(content=="")
    {
      content += newline;
    }
  else
    {
      content += "\n" + newline;
    }
  //Save it in file again
  fout.open("injury.txt");
  fout << content;
  fout.close();
}
void AddSkillschTxt(string newline)
{
  ifstream fin;
  ofstream fout;
  string content = "";
  string basura;
  //Open file and load it into a string
  fin.open("skillsch.txt");
  if(fin)
    {
      while(!fin.eof())
	{
	  getline(fin, basura);
	  content+=basura + "\n";
	}
    }
  //add newline
  fin.close();
  content += newline;
  
  //Save it in file again
  fout.open("skillsch.txt");
  fout << content;
  fout.close();
}

//Obtener una variable del League.dat
int GetLeagueDat(string variable) //Legacy
{
  return GetVarFrom(variable, Simu::League);
}
int GetVarFrom(string variable, string filename)
{
  int value;
  string basura;
  ifstream f;
  f.open(filename.c_str());
  if(!f)
  {
    cout << "No se ha encontrado el archivo " << filename << endl;
    exit(1);
  }
  f >> basura;
  do
    {
      if(basura==variable)
      {
        f >> basura; //Me como el igual
        f >> value;
        return value;
      }
      f >> basura;
    }while (!f.eof());
  f.close();
}

//Legacy for more general function
string GetLeagueDatString(string variable)
{
  return GetStringVarFrom(variable, Simu::League);
}
string GetStringVarFrom(string variable, string filename)
{
  string value;
  string basura;
  ifstream f;
  f.open(filename.c_str());
  if(!f)
  {
    cout << "No se ha encontrado el archivo " << filename << endl;
    exit(1);
  }
  f >> basura;
  do
    {
      if(basura==variable)
      {
        f >> basura; //Me como el igual
        f >> value;
        value = Substitute(value, "_", " ");
        return value;
      }
      f >> basura;
    }while (!f.eof());
  f.close();
}

//Contar lineas de un fichero que NO esten vacias
int GetUsedLines(string filename)
{
  int Nlines = 0;
  string basura;
  ifstream f;
  
  f.open(filename.c_str());
  if(!f)
    {
      cout << "No se ha encontrado el archivo " << filename << endl;
      exit(1); 
    }
  while(!f.eof())
    {
      getline(f, basura);
      if(basura!="")
	{
	  Nlines++;
	}
    }
  f.close();
  return Nlines;
}

char* GetTime()
{
  char* result;
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  result = asctime (timeinfo);
  result[24]='\0';
  return result;
}

//Sustituye todas las coincidencias de s_search en la string s por s_sub
string Substitute(string &s, string s_search, string s_sub)
{
  size_t pos = 0;
  do
  {
    pos = s.find(s_search);
    if(pos != string::npos)
    {
      s.replace(pos, s_search.length(), s_sub);
      pos += s_sub.length();
    }
  }while(s.find(s_search) != string::npos);

  return s;
}

//Numero de frases en un archivo (cuenta lineas salvo que empiecen  por # o estén vacías
int GetNTexts(string filename)
{
  //Abro fichero
  ifstream infile;
  infile.open(filename.c_str());
  int N = 0;
  string basura;
  //Comprobar que el fichero existe
  if(!infile)
  {
    cout << filename << " no encontrado" << endl;
    exit(1);
  }
  //Leer cuantas lineas hay que no esten vacias ni empiecen por #
  while(!infile.eof())
  {
    getline(infile, basura);
    if(basura.substr(0,1)!="#" && basura != "")
    {
      N++;
    }
  }
  return N;
}

//Devuelve string random de las que se encuentran en un archivo lang
string GetRandomText(string filename)
{
  int N = GetNTexts(filename);
  int k = rand() % N + 1;
  //Abro fichero
  ifstream infile;
  infile.open(filename.c_str());
  int i = 0;
  string basura;
  //Comprobar que el fichero existe
  if(!infile)
  {
    cout << filename << " no encontrado" << endl;
    exit(1);
  }
  //Leer cuantas lineas hay que no esten vacias ni empiecen por #
  while(i<k)
  {
    getline(infile, basura);
    if(basura.substr(0,1)!="#" && basura != "")
    {
      i++;
    }
  }
  return basura;
}