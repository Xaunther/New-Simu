//Implementacion de las funciones

#include "textmisc.h"
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
int GetLeagueDat(string variable)
{
  int value;
  string basura;
  ifstream f;
  f.open("League.dat");
  f >> basura;
  do
    {
      if(basura==variable)
	{
	  f >> basura; //Me como el igual
	  f >> value;
	  return value;
	}
      basura = "";
      f >> basura;
    }while (basura!="");
  f.close();
}

string GetLeagueDatString(string variable)
{
  string value;
  string basura;
  ifstream f;
  f.open("League.dat");
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
      basura = "";
      f >> basura;
    }while (basura!="");
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
      return 0; 
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

string Substitute(string &s, string s_search, string s_sub)
{
  size_t pos = 0;
  do
  {
    pos = s.find(s_search);
    s.replace(pos, s_search.length(), s_sub);
    if(pos != string::npos)
    {
      pos += s_sub.length();
    }
  }while(s.find(s_search) != string::npos);

  return s;

}