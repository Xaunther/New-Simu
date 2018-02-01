#Makefile para Linux huehue

#Variables
GCC = g++ #Compilador
Script = bin/Simulador.out #Simulador compilado
libs = build/equipo.o build/jugador.o build/textmisc.o build/position.o build/tactic.o
main = src/Simulador.cpp

#Opciones custom de make
.PHONY: all clean

all: $(Script) #Compila todo

clean: #Borra compilaciones
	rm -f */*.o */*.exe */*.out

#Dependencias, de momento hay esto

#Compilación del programa
$(Script): $(libs) $(main)
	$(GCC) $(main) $(libs) -o $(Script)

#Linkear equipo
build/equipo.o: build/jugador.o build/textmisc.o src/equipo.cpp src/equipo.h
	$(GCC) -c src/equipo.cpp -o build/equipo.o

#Linkear jugador
build/jugador.o: build/textmisc.o src/jugador.cpp src/jugador.h
	$(GCC) -c src/jugador.cpp -o build/jugador.o

#Linkear textmisc
build/textmisc.o: src/textmisc.cpp src/textmisc.h
	$(GCC) -c src/textmisc.cpp -o build/textmisc.o

#Linkear position
build/position.o: src/tactic.o src/Simu.h src/position.cpp src/position.h
	$(GCC) -c src/position.cpp -o build/position.o

#Linkear tactic
build/tactic.o: src/Simu.h src/ability.o src/tactic.cpp src/tactic.h
	$(GCC) -c src/tactic.cpp -o build/tactic.o
