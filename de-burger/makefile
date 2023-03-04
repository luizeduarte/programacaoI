parametrosCompilacao=-Wall -lncurses 
nomePrograma=deburger

all: $(nomePrograma)

$(nomePrograma): main.o de-burguer.o
	gcc -o $(nomePrograma) main.o de-burguer.o $(parametrosCompilacao)

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

de-burguer.o: de-burguer.h de-burguer.c
	gcc -c de-burguer.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)
