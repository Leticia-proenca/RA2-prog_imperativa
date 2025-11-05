#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

// da o nome da  ategoria com base no enum
const char* obterNomeCategoria(CategoriaAlimento cat) {
    const char* nomes[] = {
        "Cereais e derivados",
        "Verduras, hortaliças e derivados",
        "Frutas e derivados",
        "Gorduras e óleos",
        "Pescados e frutos do mar",
        "Carnes e derivados",
        "Leite e derivados",
        "Bebidas (alcoólicas e não alcoólicas)",
        "Ovos e derivados",
        "Produtos açucarados",
        "Miscelâneas",
        "Outros alimentos industrializados",
        "Alimentos preparados",
        "Leguminosas e derivados",
        "Nozes e sementes"
    };
    return nomes[cat];
}

// cria novo nó de categoria
NoCategoria* criarNoCategoria(CategoriaAlimento cat, const char* nome) {
    NoCategoria* novo = (NoCategoria*)malloc(sizeof(NoCategoria));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memória para categoria\n");
        exit(1);
    }
    
    novo->categoria = cat;
    strncpy(novo->nome, nome, 99);
    novo->nome[99] = '\0';
    novo->listaAlimentos = NULL;
    novo->proximo = NULL;
    novo->arvoreEnergia = NULL;
    novo->arvoreProteina = NULL;
    
    return novo;
}

// insere nova categoria na lista, confere e coloca em ordem alfabetica
void inserirCategoriaOrdenada(NoCategoria** lista, NoCategoria* novo) {
    NoCategoria* atual = *lista;
    NoCategoria* anterior = NULL;
    
    // procura posição correta na ordem alfabética
    while (atual != NULL && strcmp(novo->nome, atual->nome) > 0) {
        anterior = atual;
        atual = atual->proximo;
    }
    
    // coloca no inicio
    if (anterior == NULL) {
        novo->proximo = *lista;
        *lista = novo;
    } else {
        // coloca no meio ou no final
        novo->proximo = atual;
        anterior->proximo = novo;
    }
}

// busca categoria especifica na lista
NoCategoria* buscarCategoria(NoCategoria* lista, CategoriaAlimento cat) {
    NoCategoria* atual = lista;
    
    while (atual != NULL) {
        if (atual->categoria == cat) {
            return atual;
        }
        atual = atual->proximo;
    }
    
    return NULL;
}

// remove categoria da lista
void removerCategoria(NoCategoria** lista, CategoriaAlimento cat) {
    NoCategoria* atual = *lista;
    NoCategoria* anterior = NULL;
    
    while (atual != NULL) {
        if (atual->categoria == cat) {
            if (anterior == NULL) {
                *lista = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            
            liberarListaAlimentos(atual->listaAlimentos);
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
}

// libera a lista inteira
void liberarListaCategorias(NoCategoria* lista) {
    NoCategoria* atual = lista;
    NoCategoria* temp;
    
    while (atual != NULL) {
        temp = atual;
        atual = atual->proximo;
        liberarListaAlimentos(temp->listaAlimentos);
        free(temp);
    }
}

// cria novo nó de alimento
NoAlimento* criarNoAlimento(Alimento* alimento) {
    NoAlimento* novo = (NoAlimento*)malloc(sizeof(NoAlimento));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memória para alimento\n");
        exit(1);
    }
    
    novo->alimento = alimento;
    novo->proximo = NULL;
    
    return novo;
}

// insere alimentos na lista - na ordem alfabética
void inserirAlimentoOrdenado(NoAlimento** lista, NoAlimento* novo) {
    NoAlimento* atual = *lista;
    NoAlimento* anterior = NULL;
    
    // encontra a posição certa da ordem alfabetica para inserir
    while (atual != NULL && strcmp(novo->alimento->descricao, atual->alimento->descricao) > 0) {
        anterior = atual;
        atual = atual->proximo;
    }
    
    // insere no inicio
    if (anterior == NULL) {
        novo->proximo = *lista;
        *lista = novo;
    } else {
        // insere no meio ou no fim
        novo->proximo = atual;
        anterior->proximo = novo;
    }
}

// remove alimento da lista
void removerAlimento(NoAlimento** lista, int numeroAlimento) {
    NoAlimento* atual = *lista;
    NoAlimento* anterior = NULL;
    
    while (atual != NULL) {
        if (atual->alimento->numero == numeroAlimento) {
            if (anterior == NULL) {
                *lista = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            
            free(atual->alimento);
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
}

// libera a lista inteira de alimentos
void liberarListaAlimentos(NoAlimento* lista) {
    NoAlimento* atual = lista;
    NoAlimento* temp;
    
    while (atual != NULL) {
        temp = atual;
        atual = atual->proximo;
        free(temp->alimento);
        free(temp);
    }
}