#include "equipo.h"
#include "jugador.h"
#include "textmisc.h"
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

//Funciones de la clase equipo
equipo::equipo(string abrev)
{
	Njugadores = 0;
	abreviatura = abrev;
	nombre = GetLeagueDatString(abreviatura);
}
void equipo::Clear(){Njugadores = 0;}
void equipo::Load()
{
	string filename = abreviatura + ".txt";
	string basura;
	ifstream fichero;
	
	fichero.open(filename.c_str());
	if(!fichero)
	{
		return;
	}
    //Bloque de cálculo (lectura de fichero)
    getline(fichero,basura);
    getline(fichero,basura); //Las dos primeras lineas son encabezados
    do //Compruebo que aun no he llegado al final, y luego paso datos al vector
    {
        basura = "";
        fichero >> basura;
        if (basura != "")
        {
            jug[Njugadores].Name = basura;
            fichero >> jug[Njugadores].Age;
            fichero >> jug[Njugadores].Nat;
            fichero >> jug[Njugadores].St;
            fichero >> jug[Njugadores].Tk;
            fichero >> jug[Njugadores].Ps;
            fichero >> jug[Njugadores].Sh;
            fichero >> jug[Njugadores].Ag;
            fichero >> jug[Njugadores].KAb;
            fichero >> jug[Njugadores].TAb;
            fichero >> jug[Njugadores].PAb;
            fichero >> jug[Njugadores].SAb;
            fichero >> jug[Njugadores].Gam;
            fichero >> jug[Njugadores].Trd;
            fichero >> jug[Njugadores].Min;
            fichero >> jug[Njugadores].Mom;
            fichero >> jug[Njugadores].Sav;
            fichero >> jug[Njugadores].Con;
            fichero >> jug[Njugadores].Ktk;
            fichero >> jug[Njugadores].Kps;
            fichero >> jug[Njugadores].Sht;
            fichero >> jug[Njugadores].Gls;
            fichero >> jug[Njugadores].Ass;
            fichero >> jug[Njugadores].DP;
            fichero >> jug[Njugadores].Inj;
            fichero >> jug[Njugadores].Sus;
            fichero >> jug[Njugadores].Fit;
            jug[Njugadores].MinHoy = 0;
            jug[Njugadores].hasPain = false;
            jug[Njugadores].Cond = jug[Njugadores].Fit*100/max(1,jug[Njugadores].Trd);
            Njugadores++;
        }
    }while (basura != "");
    fichero.close();
}
void equipo::Save()
{
    //Inicialización de variables
    ofstream fichero;
    string filename = abreviatura + ".txt";
    int i;
    //Apertura del fichero
    fichero.open (filename.c_str());
    
    //Encabezados
    fichero << "Name         Age Nat St Tk Ps Sh Ag KAb TAb PAb SAb Gam Fit  Min Mom Sav Con Ktk Kps Sht Gls Ass  DP Inj Sus Sta" << endl;
    fichero << "----------------------------------------------------------------------------------------------------------------" << endl;
    //Volcado de la información en el fichero
    for (i=0;i<Njugadores;i++)
    {
        fichero << left << setfill(' ') << setw(13) << jug[i].Name;
        fichero << right << setfill(' ') << setw(3) << jug[i].Age;
        fichero << setfill(' ') << setw(4) << jug[i].Nat;
        fichero << setfill(' ') << setw(3) << jug[i].St;
        fichero << setfill(' ') << setw(3) << jug[i].Tk;
        fichero << setfill(' ') << setw(3) << jug[i].Ps;
        fichero << setfill(' ') << setw(3) << jug[i].Sh;
        fichero << setfill(' ') << setw(3) << jug[i].Ag;
        fichero << setfill(' ') << setw(4) << jug[i].KAb;
        fichero << setfill(' ') << setw(4) << jug[i].TAb;
        fichero << setfill(' ') << setw(4) << jug[i].PAb;
        fichero << setfill(' ') << setw(4) << jug[i].SAb;
        fichero << setfill(' ') << setw(4) << jug[i].Gam;
        fichero << setfill(' ') << setw(4) << jug[i].Trd;
        fichero << setfill(' ') << setw(5) << jug[i].Min;
        fichero << setfill(' ') << setw(4) << jug[i].Mom;
        fichero << setfill(' ') << setw(4) << jug[i].Sav;
        fichero << setfill(' ') << setw(4) << jug[i].Con;
        fichero << setfill(' ') << setw(4) << jug[i].Ktk;
        fichero << setfill(' ') << setw(4) << jug[i].Kps;
        fichero << setfill(' ') << setw(4) << jug[i].Sht;
        fichero << setfill(' ') << setw(4) << jug[i].Gls;
        fichero << setfill(' ') << setw(4) << jug[i].Ass;
        fichero << setfill(' ') << setw(4) << jug[i].DP;
        fichero << setfill(' ') << setw(4) << jug[i].Inj;
        fichero << setfill(' ') << setw(4) << jug[i].Sus;
        fichero << setfill(' ') << setw(4) << jug[i].Fit << endl;
    }
    
    //Cierre de fichero
    fichero.close ();
}

int equipo::Search(string nombre)
{
	for(int i=0;i<Njugadores;i++)
	{
		if(nombre == jug[i].Name)
		{
			return i;
		}
	}
}

void equipo::ReduceInj()
{
	for(int i=0;i<Njugadores;i++)
	{
		jug[i].ReduceInj(abreviatura);
	}
}
void equipo::ReduceSus()
{
	for(int i=0;i<Njugadores;i++)
	{
		jug[i].ReduceSus(abreviatura);
	}
}
void equipo::AddTrd(int trdbonus)
{
	for(int i=0;i<Njugadores;i++)
	{
		jug[i].AddTrd(trdbonus);
	}
}
void equipo::SetCond()
{
	for(int i=0;i<Njugadores;i++)
	{
		jug[i].SetCond();
	}
}
void equipo::SetTrd()
{
	for(int i=0;i<Njugadores;i++)
	{
		jug[i].SetTrd();
	}
}
void equipo::ComputeFit()
{
	for(int i=0;i<Njugadores;i++)
	{
		jug[i].ComputeFit();
	}
}
string equipo::VetoedGK()
{
	int posbest=0;
	int pos2best=1;
	for(int i=0;i<Njugadores;i++)
	{
		if(jug[i].isGK())
		{
			if(jug[i].St > jug[posbest].St || (jug[i].St == jug[posbest].St && jug[i].Min > jug[posbest].Min))
			{
				pos2best = posbest;
				posbest = i;
			}
			else if((jug[i].St > jug[pos2best].St || (jug[i].St == jug[pos2best].St && jug[i].Min > jug[pos2best].Min)) && i!=posbest)
			{
				pos2best = i;
			}
		}
	}
	//Tengo el portero suplente, hay que mirar que esté disponible
	if(jug[pos2best].Sus>0 || jug[pos2best].Inj>0)
	{
		return "";
	}
	else
	{
		return jug[posbest].Name;
	}
}
