//Implementacion de las funciones

#include "textmisc.h"
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

string GetInjuryType()
{
	//Obtener valor aleatorio entre 0 y 9
	int index = (rand() % 10);
	string types[10] = 
	{
		"diarrea extrema :truño:",
		"bolas azules",
		"una pierna amputada",
		"un flechazo en la rodilla",
		"un codo rasguñado",
		"una fumada del 15 :weed:",
		"cáncer testicular",
		"cirugía plástica",
		"un leve resfriado",
		"un tentáculo roto"
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
			f.ignore(1, ' ');
			getline(f,value);
			return value;
		}
		basura = "";
		f >> basura;
	}while (basura!="");
	f.close();
}
