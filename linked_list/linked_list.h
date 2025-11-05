#ifndef LISTA_H
#define LISTA_H

#include "tipos.h"

// estrutura do nó da lista dos alimentos
typedef struct NoAlimento {
    Alimento* alimento;
    struct NoAlimento* proximo;
} NoAlimento;

// estrutura dos nós da lista de categorias
typedef struct NoCategoria {
    CategoriaAlimento categoria;
    char nome[100];
    NoAlimento* listaAlimentos;
    struct NoCategoria* proximo;
    void* arvoreEnergia;
    void* arvoreProteina;
} NoCategoria;

// manipulação da lisat de categorias
NoCategoria* criarNoCategoria(CategoriaAlimento cat, const char* nome);
void inserirCategoriaOrdenada(NoCategoria** lista, NoCategoria* novo);
NoCategoria* buscarCategoria(NoCategoria* lista, CategoriaAlimento cat);
void removerCategoria(NoCategoria** lista, CategoriaAlimento cat);
void liberarListaCategorias(NoCategoria* lista);

// manipulação para a lista de alimentos
NoAlimento* criarNoAlimento(Alimento* alimento);
void inserirAlimentoOrdenado(NoAlimento** lista, NoAlimento* novo);
void removerAlimento(NoAlimento** lista, int numeroAlimento);
void liberarListaAlimentos(NoAlimento* lista);

// auxilio para decobrir o nome da categoria
const char* obterNomeCategoria(CategoriaAlimento cat);

#endif