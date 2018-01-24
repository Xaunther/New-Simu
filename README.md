## Estructura de las carpetas:

**bin**: Contiene archivos binarios (.exe por ejemplo), resultado de compilar un programa. (No deberían añadirse a git).
**build**: Contiene librerías linkadas (.o generalmente). Para cada archivo .cpp es necesario hacer una (salvo el main).
**doc**: Log de cambios y posibles cosas que queramos explicar.
**src**: Codigo.
**test**: Carpeta para testear.


## Archivos especiales:

**.gitignore**: Qué no debe incluirse en git (cosas que se generen automáticamente sobre todo).
**makefile**: Instrucciones para compilar en Linux. Dev-c++ lo genera automáticamente a la hora de compilar, pero lo llama distinto.
**release.notes**: Notas con cambios importantes que vayamos haciendo y tal.


**Importante**: Intentar trabajar en branches y sólo pushear a master cuando las cosas funcionen.