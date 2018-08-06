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
