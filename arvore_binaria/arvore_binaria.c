#include <stdio.h>
#include <stdlib.h>
#include "arvore_binaria.h"

/* Cria um novo nó da árvore */
NoArvore* criarNoArvore(float chave, NoAlimento* alimento) {
    NoArvore* novo = (NoArvore*)malloc(sizeof(NoArvore));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memória para nó da árvore\n");
        exit(1);
    }
    
    novo->chave = chave;
    novo->alimento = alimento;
    novo->esquerda = NULL;
    novo->direita = NULL;
    
    return novo;
}

/* Insere um nó na árvore binária */
NoArvore* inserirNaArvore(NoArvore* raiz, float chave, NoAlimento* alimento) {
    if (raiz == NULL) {
        return criarNoArvore(chave, alimento);
    }
    
    if (chave < raiz->chave) {
        raiz->esquerda = inserirNaArvore(raiz->esquerda, chave, alimento);
    } else {
        raiz->direita = inserirNaArvore(raiz->direita, chave, alimento);
    }
    
    return raiz;
}

/* Percorre a árvore em ordem decrescente (direita-raiz-esquerda) */
void percorrerDecrescente(NoArvore* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    percorrerDecrescente(raiz->direita);
    
    printf("%d - %s: %.2f\n", 
        raiz->alimento->alimento->numero,
        raiz->alimento->alimento->descricao,
        raiz->chave);
    
    percorrerDecrescente(raiz->esquerda);
}

/* Percorre a árvore mostrando apenas valores no intervalo [min, max] */
void percorrerIntervalo(NoArvore* raiz, float min, float max) {
    if (raiz == NULL) {
        return;
    }
    
    /* Se o valor pode estar à esquerda */
    if (raiz->chave > min) {
        percorrerIntervalo(raiz->esquerda, min, max);
    }
    
    /* Se o valor está no intervalo */
    if (raiz->chave >= min && raiz->chave <= max) {
        printf("%d - %s: %.2f\n", 
            raiz->alimento->alimento->numero,
            raiz->alimento->alimento->descricao,
            raiz->chave);
    }
    
    /* Se o valor pode estar à direita */
    if (raiz->chave < max) {
        percorrerIntervalo(raiz->direita, min, max);
    }
}

/* Libera toda a árvore */
void liberarArvore(NoArvore* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

/* Constrói árvore de indexação por energia */
NoArvore* construirArvoreEnergia(NoAlimento* listaAlimentos) {
    NoArvore* raiz = NULL;
    NoAlimento* atual = listaAlimentos;
    
    while (atual != NULL) {
        raiz = inserirNaArvore(raiz, atual->alimento->energia, atual);
        atual = atual->proximo;
    }
    
    return raiz;
}

/* Constrói árvore de indexação por proteína */
NoArvore* construirArvoreProteina(NoAlimento* listaAlimentos) {
    NoArvore* raiz = NULL;
    NoAlimento* atual = listaAlimentos;
    
    while (atual != NULL) {
        raiz = inserirNaArvore(raiz, atual->alimento->proteina, atual);
        atual = atual->proximo;
    }
    
    return raiz;
}