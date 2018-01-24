//Implementacion de las funciones

#include "textmisc.h"
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

//Leer una variable definida en League.dat.
string GetLeagueDat(string variable)
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
