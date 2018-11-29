#ifndef arraytools_h
#define arraytools_h

#include <string>
using namespace std;

//Funcion para comprobar si en un array hay alguna cosa repetida
template <class T>
bool TieneRepetidos(T* array, int size)
{
  for(int i=0;i<size-1;i++)
    {
      for(int j=i+1;j<size;j++)
	{
	  if(array[i]==array[j])
	    {
	      return true;
	    }
	}
    }
  return false;
}

template <class T>
T SumArray(T* array, int size)
{
	T sum = 0;
	for(int i=0;i<size;i++)
	{
		sum += array[i];
	}
	return sum;
}


#endif
