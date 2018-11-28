## Estructura de las carpetas:

**build**: Contiene librerías linkadas (.o generalmente), y todo lo que se produzca de la compilación. No debe subirse al repositorio ya que son archivos generados automáticamente

**doc**: Log de cambios y posibles cosas que queramos explicar.

**src**: Codigo fuente de implementación. (.cpp)

**include**: Header files. (.h)

**config**: Archivos que configuren el comportamiento del programa, y sean tuneables.

**test**: Carpeta para testear.


## Archivos especiales:

**.gitignore**: Qué no debe incluirse en git (cosas que se generen automáticamente sobre todo).

**CMakeLists.txt**: Instrucciones para que cmake (Linux) genere el makefile para compilar, luego sólo es necesario hacer "make" :)

**release.notes**: Notas con cambios importantes que vayamos haciendo y tal.


## Importante:
Intentar trabajar en branches y sólo pushear a master cuando las cosas funcionen.

## Cómo compilar en

**LINUX**: Es necesario tener instalado cmake, make y un compilador de c++:
* make: Desde la terminal:
```
sudo apt-get install make
```
* cmake: Desde la terminal:
```
sudo apt-get install cmake
```
* compilador: Desde la terminal:
```
sudo apt-get install g++
```
Abrir la terminal en la carpeta build y ejecutar:
```
cmake .. && make
```

**Windows**: Sólo he conseguido compilar usando [Visual Studio Code](https://code.visualstudio.com/?wt.mc_id=DX_841432), teniendo instalados [CMake](https://cmake.org/download/) y [Visual Studio Community 2017](https://visualstudio.microsoft.com/downloads/) como compilador de C++