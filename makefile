# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Pasta com os arquivos fonte
SRC_DIR = src

# Arquivos fonte compartilhados
COMMON_SRCS = $(SRC_DIR)/ler_arquivo.c $(SRC_DIR)/arvore_binaria.c $(SRC_DIR)/linked_list.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

# Alvos principais
all: P1 P2

# Programa 1
P1: $(SRC_DIR)/P1.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o P1 $(SRC_DIR)/P1.o $(COMMON_OBJS)

# Programa 2
P2: $(SRC_DIR)/P2.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o P2 $(SRC_DIR)/P2.o $(COMMON_OBJS)

# Regra genérica de compilação
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(SRC_DIR)/*.o P1 P2

.PHONY: all clean
