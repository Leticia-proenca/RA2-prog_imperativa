#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

/* Retorna o nome da categoria baseado no enum */
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

/* Cria um novo nó de categoria */
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

/* Insere uma categoria na lista mantendo ordem alfabética */
void inserirCategoriaOrdenada(NoCategoria** lista, NoCategoria* novo) {
    NoCategoria* atual = *lista;
    NoCategoria* anterior = NULL;
    
    /* Encontra a posição correta para inserção */
    while (atual != NULL && strcmp(novo->nome, atual->nome) > 0) {
        anterior = atual;
        atual = atual->proximo;
    }
    
    /* Insere no início */
    if (anterior == NULL) {
        novo->proximo = *lista;
        *lista = novo;
    } else {
        /* Insere no meio ou fim */
        novo->proximo = atual;
        anterior->proximo = novo;
    }
}

/* Busca uma categoria na lista */
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

/* Remove uma categoria da lista */
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

/* Libera toda a lista de categorias */
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

/* Cria um novo nó de alimento */
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

/* Insere um alimento na lista mantendo ordem alfabética */
void inserirAlimentoOrdenado(NoAlimento** lista, NoAlimento* novo) {
    NoAlimento* atual = *lista;
    NoAlimento* anterior = NULL;
    
    /* Encontra a posição correta para inserção */
    while (atual != NULL && strcmp(novo->alimento->descricao, atual->alimento->descricao) > 0) {
        anterior = atual;
        atual = atual->proximo;
    }
    
    /* Insere no início */
    if (anterior == NULL) {
        novo->proximo = *lista;
        *lista = novo;
    } else {
        /* Insere no meio ou fim */
        novo->proximo = atual;
        anterior->proximo = novo;
    }
}

/* Remove um alimento da lista */
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

/* Libera toda a lista de alimentos */
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