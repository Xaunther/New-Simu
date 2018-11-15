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
    int asistencias;
    int chuts;
    int pases;
    int tackles;
    int paradas;
    bool rojas;
    bool amarillas;
    int minutos;
    int faltas;
    bool hajugado;
    //Funciones
    jug_stats();
};

#endif