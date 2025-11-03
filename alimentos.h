#ifndef ALIMENTO_H
#define ALIMENTO_H

#define MAX_NOME 100
#define MAX_CATEGORIA 50

typedef struct {
    int id;
    char nome[MAX_NOME];
    char categoria[MAX_CATEGORIA];
    float calorias;
    float proteinas;
    float carboidratos;
    float gorduras;
} Alimento;

#endif