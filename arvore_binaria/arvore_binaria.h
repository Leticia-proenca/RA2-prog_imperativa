#ifndef ARVORE_H
#define ARVORE_H

#include "lista.h"

/* Estrutura do nó da árvore binária de indexação */
typedef struct NoArvore {
    float chave;
    NoAlimento* alimento;
    struct NoArvore* esquerda;
    struct NoArvore* direita;
} NoArvore;

/* Funções para manipulação da árvore binária */
NoArvore* criarNoArvore(float chave, NoAlimento* alimento);
NoArvore* inserirNaArvore(NoArvore* raiz, float chave, NoAlimento* alimento);
void percorrerDecrescente(NoArvore* raiz);
void percorrerIntervalo(NoArvore* raiz, float min, float max);
void liberarArvore(NoArvore* raiz);

/* Funções para construir árvores a partir de uma lista de alimentos */
NoArvore* construirArvoreEnergia(NoAlimento* listaAlimentos);
NoArvore* construirArvoreProteina(NoAlimento* listaAlimentos);

#endif