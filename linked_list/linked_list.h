#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "alimentos.h"

// Nó da lista de alimentos
typedef struct NodeAlimento {
    Alimento dados;
    struct NodeAlimento *proximo;
} NodeAlimento;

// Lista de alimentos de uma categoria
typedef struct {
    NodeAlimento *inicio;
    int tamanho;
} ListaAlimentos;

// Nó da lista de categorias
typedef struct NodeCategoria {
    char nomeCategoria[MAX_CATEGORIA];
    ListaAlimentos *alimentos;
    struct NodeCategoria *proximo;
} NodeCategoria;

// Lista de categorias (lista principal)
typedef struct {
    NodeCategoria *inicio;
    int tamanho;
} ListaCategorias;

// Funções da lista de categorias
ListaCategorias* criarListaCategorias();
NodeCategoria* buscarOuCriarCategoria(ListaCategorias *listaCat, const char *nomeCategoria);
void inserirAlimentoOrdenado(ListaAlimentos *listaAlim, Alimento alimento);
void inserirCategoriaOrdenada(ListaCategorias *listaCat, NodeCategoria *novaCategoria);
void adicionarAlimento(ListaCategorias *listaCat, Alimento alimento);
void exibirCategorias(ListaCategorias *listaCat);
void liberarCategorias(ListaCategorias *listaCat);
ListaCategorias* carregarDoBinario();

#endif