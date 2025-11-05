# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Arquivos objeto
OBJ_COMUM = lista.o arvore.o arquivo.o
OBJ_P1 = P1.o $(OBJ_COMUM)
OBJ_P2 = P2.o menu.o $(OBJ_COMUM)

# Executáveis
EXECUTAVEIS = P1 P2

# Regra padrão
all: $(EXECUTAVEIS)

# Compilação do P1
P1: $(OBJ_P1)
	$(CC) $(CFLAGS) -o P1 $(OBJ_P1)

# Compilação do P2
P2: $(OBJ_P2)
	$(CC) $(CFLAGS) -o P2 $(OBJ_P2)

# Regras para arquivos objeto
P1.o: P1.c arquivo.h tipos.h
	$(CC) $(CFLAGS) -c P1.c

P2.o: P2.c arquivo.h lista.h arvore.h tipos.h
	$(CC) $(CFLAGS) -c P2.c

lista.o: lista.c lista.h tipos.h
	$(CC) $(CFLAGS) -c lista.c

arvore.o: arvore.c arvore.h lista.h tipos.h
	$(CC) $(CFLAGS) -c arvore.c

arquivo.o: arquivo.c arquivo.h lista.h tipos.h
	$(CC) $(CFLAGS) -c arquivo.c

# Limpeza
clean:
	rm -f *.o $(EXECUTAVEIS) dados.bin

# Limpeza completa (incluindo arquivos de backup)
distclean: clean
	rm -f *~ *.bak

.PHONY: all clean distclean