#ifndef ARVORE_H
#define ARVORE_H

#include "linked_list.h"

// define estrutura do nó da árvore
typedef struct NoArvore {
    float chave;
    NoAlimento* alimento;
    struct NoArvore* esquerda;
    struct NoArvore* direita;
} NoArvore;

// funções de manipulação da árvore
NoArvore* criarNoArvore(float chave, NoAlimento* alimento);
NoArvore* inserirNaArvore(NoArvore* raiz, float chave, NoAlimento* alimento);
void percorrerDecrescente(NoArvore* raiz);
void percorrerIntervalo(NoArvore* raiz, float min, float max);
void liberarArvore(NoArvore* raiz);

//funções para construir arvore
NoArvore* construirArvoreEnergia(NoAlimento* listaAlimentos);
NoArvore* construirArvoreProteina(NoAlimento* listaAlimentos);

#endif