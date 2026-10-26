CC = gcc
CFLAGS = -Wall -O2
SRC = EP.1.c EP.1.heap.c EP.1.lista.c EP.1.matriz.c

all: ep_lista ep_matriz

ep_lista: $(SRC)
	$(CC) $(CFLAGS) EP.1.c -o ep_lista

ep_matriz: $(SRC)
	$(CC) $(CFLAGS) -DMATRIZ EP.1.c -o ep_matriz

teste: all
	./ep_lista exemplos/entrada_valida.txt saida_lista.txt
	./ep_matriz exemplos/entrada_valida.txt saida_matriz.txt
	diff saida_lista.txt exemplos/saida_valida.txt && echo "lista: OK"
	diff saida_matriz.txt exemplos/saida_valida.txt && echo "matriz: OK"

clean:
	rm -f ep_lista ep_matriz saida_lista.txt saida_matriz.txt

.PHONY: all teste clean
