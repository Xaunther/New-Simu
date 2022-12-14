//Clase que controla los live stats de los jugadores
#ifndef jug_stats_h
#define jug_stats_h

using namespace std;

//Clase stats
class jug_stats
{
    //Variables
public:
    int goles;
    int goles_propia;
    int encajados;
    int asistencias;
    int chuts;
    int chuts_puerta;
    int pases;
    int tackles;
    int paradas;
    bool rojas;
    bool amarillas;
    int minutos;
    int faltas;
    bool hajugado;
    bool lesionado;
    int trd_inicial;
    int St;
    int Tk;
    int Ps;
    int Sh;
    //Funciones
    jug_stats();
    bool Is_Playable();
    //Calculador de stats OwO
    void Update(); //Ojo que añade a lo que ya hay
};

#endif