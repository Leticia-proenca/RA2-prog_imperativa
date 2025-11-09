#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

// da o nome da  categoria com base no enum, converte o enum para uma string, recebe enum, retona string constante
const char* obterNomeCategoria(CategoriaAlimento cat) {
    const char* nomes[] = {// array de strings constantes, por ter const não pode ser alterado
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
    return nomes[cat];// usa numero do enum como a posição do array
}

// cria novo nó de categoria
NoCategoria* criarNoCategoria(CategoriaAlimento cat, const char* nome) {// recebe o enum e o nome da categoria
    NoCategoria* novo = (NoCategoria*)malloc(sizeof(NoCategoria));// aloca memória dinâmica para o novo nó

    if (novo == NULL) { // verifica alocação
        fprintf(stderr, "Erro ao alocar memória para categoria\n");
        exit(1);
    }
    
    novo->categoria = cat;// armazena valor do enum no nó
    strncpy(novo->nome, nome, 99);// copia até 99 caracteres do nome para o nó, evita overflow no buffer
    novo->nome[99] = '\0';
    //inicializa todos os valores como null
    novo->listaAlimentos = NULL;
    novo->proximo = NULL;
    novo->arvoreEnergia = NULL;
    novo->arvoreProteina = NULL;
    
    return novo;// retorna o ponteiro para o novo nó
}

// insere nova categoria na lista, confere e coloca em ordem alfabetica
void inserirCategoriaOrdenada(NoCategoria** lista, NoCategoria* novo) {// NoCategoria **: ponteiro duplo para poder mudar a head da lista
    NoCategoria* atual = *lista;// ponteiro qu epercorre a lista
    NoCategoria* anterior = NULL;// nó anterior, para inserção
    
    // procura posição correta na ordem alfabética
    while (atual != NULL && strcmp(novo->nome, atual->nome) > 0) {// compara as string alfabeticamente
        // retorna > 0: novo -> nome vem depois de atual -> nome
        // retorna <0: novo -> nome vem antes
        // retorna 0 quanod forem iguais 
        anterior = atual;// avança para o próximo nó
        atual = atual->proximo;// guarda o nó que acabou de ver
    }
    
    // coloca no inicio
    if (anterior == NULL) {
        novo->proximo = *lista; // novo aponta para a antiga head
        *lista = novo; // se torna o novo head
    } else {
        // coloca no meio ou no final
        novo->proximo = atual;// aponta para o próximo nó
        anterior->proximo = novo;// nó anterior aponta para o novo
    }
}

// busca categoria especifica na lista
NoCategoria* buscarCategoria(NoCategoria* lista, CategoriaAlimento cat) {// retorna o primeiro  ou null se não encontrar
    NoCategoria* atual = lista;// começa no head
    
    while (atual != NULL) {// percorre a lista vendo todos os nós
        if (atual->categoria == cat) {// se encontrar retorna automaticamente
            return atual;
        }
        atual = atual->proximo;
    }
    
    return NULL;
}

// remove categoria da lista, liberando a memória
// remove todos os alimentos junto
void removerCategoria(NoCategoria** lista, CategoriaAlimento cat) {
    //ponteiros de referencia
    NoCategoria* atual = *lista;
    NoCategoria* anterior = NULL;
    
    while (atual != NULL) {
        if (atual->categoria == cat) {// procura a categoria a ser removida
            if (anterior == NULL) {// remove o primeiro nó
                *lista = atual->proximo;// passa o head para o próximo nó
            } else {// remover no meio/fim
                anterior->proximo = atual->proximo;// conecta o nó anterior ao próximo
            }
            
            liberarListaAlimentos(atual->listaAlimentos);// libera primeiro todos os alimentos, depois o nó da categoria 
            free(atual);
            return;
        }
        anterior = atual;// se não é esse vai pro próximo nó
        atual = atual->proximo;
    }
}

// libera da memória a lista inteira
void liberarListaCategorias(NoCategoria* lista) {
    //ponteiros de referencia
    NoCategoria* atual = lista; // percorre a lista
    NoCategoria* temp; //guarda o nó temporariamente antes de liberar
    
    while (atual != NULL) {
        temp = atual;// guarda o atual no temp
        atual = atual->proximo;// avança o atual para o próximo antes de liberar
        liberarListaAlimentos(temp->listaAlimentos);// libera todos os alimentos e dps o nó
        free(temp);
    }
}

// cria novo nó de alimento
NoAlimento* criarNoAlimento(Alimento* alimento) {// pega a estrutura de Alimento
    NoAlimento* novo = (NoAlimento*)malloc(sizeof(NoAlimento));// aloca memória

    if (novo == NULL) {// verfifica alocação
        fprintf(stderr, "Erro ao alocar memória para alimento\n");
        exit(1);
    }
    
    novo->alimento = alimento;// armazena ponteiro para alimento, não copia, só guarda o endereço na memória
    novo->proximo = NULL;// inicializa o próximo como null
    
    return novo;// retorna o novo nó
}

// insere alimentos na lista - na ordem alfabética por descrição
// mesma lógica do inserirCategoriaOrdenada
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
// mesma lógica do removerCategoria
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
// mesma lógoca do liberarListaCategorias
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