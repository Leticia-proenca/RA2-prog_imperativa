#include <stdio.h>
#include <stdlib.h>
#include "arvore_binaria.h"

// cria nó 
NoArvore* criarNoArvore(float chave, NoAlimento* alimento) {
    NoArvore* novo = (NoArvore*)malloc(sizeof(NoArvore));
    if (novo == NULL) {
        fprintf(stderr, "Não possui memória o suficiente para o nó.\n");
        exit(1);
    }
    
    novo->chave = chave;
    novo->alimento = alimento;
    novo->esquerda = NULL;
    novo->direita = NULL;
    
    return novo;
}

// inserir nó na árvore
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

// ver a árvore em ordem decrescente
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

// ver a arvore no intervalo do min e max fornecido pelo user
void percorrerIntervalo(NoArvore* raiz, float min, float max) {
    if (raiz == NULL) {
        return;
    }
    
    // se estiver na esquerda
    if (raiz->chave > min) {
        percorrerIntervalo(raiz->esquerda, min, max);
    }
    
    // verifica se esta no intervalo fornecido
    if (raiz->chave >= min && raiz->chave <= max) {
        printf("%d - %s: %.2f\n", 
            raiz->alimento->alimento->numero,
            raiz->alimento->alimento->descricao,
            raiz->chave);
    }
    
    // se esta na direita
    if (raiz->chave < max) {
        percorrerIntervalo(raiz->direita, min, max);
    }
}

// libera árvore inteira
void liberarArvore(NoArvore* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

// árvore com os valores de energia
NoArvore* construirArvoreEnergia(NoAlimento* listaAlimentos) {
    NoArvore* raiz = NULL;
    NoAlimento* atual = listaAlimentos;
    
    while (atual != NULL) {
        raiz = inserirNaArvore(raiz, atual->alimento->energia, atual);
        atual = atual->proximo;
    }
    
    return raiz;
}

// árvore com os valores da proteína
NoArvore* construirArvoreProteina(NoAlimento* listaAlimentos) {
    NoArvore* raiz = NULL;
    NoAlimento* atual = listaAlimentos;
    
    while (atual != NULL) {
        raiz = inserirNaArvore(raiz, atual->alimento->proteina, atual);
        atual = atual->proximo;
    }
    
    return raiz;
}