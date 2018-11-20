#ifndef textmisc_h
#define textmisc_h

#include <string>
using namespace std;

//Add some basic functions
string GetInjuryType();
void AddSuspendTxt(string);
void AddInjuryTxt(string);
void AddSkillschTxt(string);
int GetLeagueDat(string);
string GetLeagueDatString(string);
int GetVarFrom(string, string);
string GetStringVarFrom(string, string);
int GetUsedLines(string);
char* GetTime();
string Substitute(string&, string, string);
//Funciones para texto random de archivo lang
int GetNTexts(string);
string GetRandomText(string);

#endif
