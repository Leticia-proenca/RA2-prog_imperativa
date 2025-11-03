#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

ListaCategorias* criarListaCategorias() {
    ListaCategorias *lista = (ListaCategorias*)malloc(sizeof(ListaCategorias));
    if (lista != NULL) {
        lista->inicio = NULL;
        lista->tamanho = 0;
    }
    return lista;
}

ListaAlimentos* criarListaAlimentos() {
    ListaAlimentos *lista = (ListaAlimentos*)malloc(sizeof(ListaAlimentos));
    if (lista != NULL) {
        lista->inicio = NULL;
        lista->tamanho = 0;
    }
    return lista;
}

// Insere alimento em ordem alfabética na lista de alimentos
void inserirAlimentoOrdenado(ListaAlimentos *listaAlim, Alimento alimento) {
    NodeAlimento *novoNode = (NodeAlimento*)malloc(sizeof(NodeAlimento));
    if (novoNode == NULL) {
        printf("Erro ao alocar memória para novo alimento!\n");
        return;
    }
    
    novoNode->dados = alimento;
    novoNode->proximo = NULL;
    
    // Lista vazia ou novo alimento vem antes do primeiro
    if (listaAlim->inicio == NULL || 
        strcmp(alimento.nome, listaAlim->inicio->dados.nome) < 0) {
        novoNode->proximo = listaAlim->inicio;
        listaAlim->inicio = novoNode;
    } else {
        // Procura posição correta (ordem alfabética)
        NodeAlimento *atual = listaAlim->inicio;
        while (atual->proximo != NULL && 
            strcmp(alimento.nome, atual->proximo->dados.nome) > 0) {
            atual = atual->proximo;
        }
        novoNode->proximo = atual->proximo;
        atual->proximo = novoNode;
    }
    
    listaAlim->tamanho++;
}

// Insere categoria em ordem alfabética na lista de categorias
void inserirCategoriaOrdenada(ListaCategorias *listaCat, NodeCategoria *novaCategoria) {
    // Lista vazia ou nova categoria vem antes da primeira
    if (listaCat->inicio == NULL || 
        strcmp(novaCategoria->nomeCategoria, listaCat->inicio->nomeCategoria) < 0) {
        novaCategoria->proximo = listaCat->inicio;
        listaCat->inicio = novaCategoria;
    } else {
        // Procura posição correta (ordem alfabética)
        NodeCategoria *atual = listaCat->inicio;
        while (atual->proximo != NULL && 
            strcmp(novaCategoria->nomeCategoria, atual->proximo->nomeCategoria) > 0) {
            atual = atual->proximo;
        }
        novaCategoria->proximo = atual->proximo;
        atual->proximo = novaCategoria;
    }
    
    listaCat->tamanho++;
}

// Busca categoria existente ou cria nova (em ordem alfabética)
NodeCategoria* buscarOuCriarCategoria(ListaCategorias *listaCat, const char *nomeCategoria) {
    // Busca categoria existente
    NodeCategoria *atual = listaCat->inicio;
    while (atual != NULL) {
        if (strcmp(atual->nomeCategoria, nomeCategoria) == 0) {
            return atual; // Categoria encontrada
        }
        atual = atual->proximo;
    }
    
    // Categoria não existe, cria nova
    NodeCategoria *novaCategoria = (NodeCategoria*)malloc(sizeof(NodeCategoria));
    if (novaCategoria == NULL) {
        printf("Erro ao alocar memória para categoria!\n");
        return NULL;
    }
    
    strcpy(novaCategoria->nomeCategoria, nomeCategoria);
    novaCategoria->alimentos = criarListaAlimentos();
    novaCategoria->proximo = NULL;
    
    // Insere em ordem alfabética
    inserirCategoriaOrdenada(listaCat, novaCategoria);
    
    return novaCategoria;
}

// Adiciona alimento na categoria correspondente
void adicionarAlimento(ListaCategorias *listaCat, Alimento alimento) {
    NodeCategoria *categoria = buscarOuCriarCategoria(listaCat, alimento.categoria);
    if (categoria != NULL) {
        inserirAlimentoOrdenado(categoria->alimentos, alimento);
    }
}

void exibirCategorias(ListaCategorias *listaCat) {
    if (listaCat->inicio == NULL) {
        printf("Nenhuma categoria cadastrada!\n");
        return;
    }
    
    printf(" * ALIMENTOS POR CATEGORIA * \n");
    
    NodeCategoria *catAtual = listaCat->inicio;
    int numCategoria = 1;
    
    while (catAtual != NULL) {
        printf("CATEGORIA %d: %s\n", numCategoria, catAtual->nomeCategoria);
        
        NodeAlimento *alimAtual = catAtual->alimentos->inicio;
        int numAlimento = 1;
        
        if (alimAtual == NULL) {
            printf("Essa categoria não possui alimentos\n");
        }
        
        while (alimAtual != NULL) {
            printf("  %d. %s\n", numAlimento, alimAtual->dados.nome);
            printf("     ID: %d\n", alimAtual->dados.id);
            printf("     Calorias: %.2f kcal\n", alimAtual->dados.calorias);
            printf("     Proteínas: %.2f g\n", alimAtual->dados.proteinas);
            printf("     Carboidratos: %.2f g\n", alimAtual->dados.carboidratos);
            printf("     Gorduras: %.2f g\n", alimAtual->dados.gorduras);
            printf("\n");
            
            alimAtual = alimAtual->proximo;
            numAlimento++;
        }
        
        catAtual = catAtual->proximo;
        numCategoria++;
    }
    
    printf("Total de categorias: %d\n", listaCat->tamanho);
}

void liberarCategorias(ListaCategorias *listaCat) {
    NodeCategoria *catAtual = listaCat->inicio;
    
    while (catAtual != NULL) {
        NodeCategoria *proxCat = catAtual->proximo;
        
        // Libera todos os alimentos da categoria
        NodeAlimento *alimAtual = catAtual->alimentos->inicio;
        while (alimAtual != NULL) {
            NodeAlimento *proxAlim = alimAtual->proximo;
            free(alimAtual);
            alimAtual = proxAlim;
        }
        
        free(catAtual->alimentos);
        free(catAtual);
        catAtual = proxCat;
    }
    
    free(listaCat);
    printf("Memória liberada com sucesso!\n");
}

ListaCategorias* carregarDoBinario() {
    FILE *arquivoBIN = fopen("dados.bin", "rb");
    
    if (arquivoBIN == NULL) {
        printf("Erro ao abrir o arquivo binário para leitura!\n");
        return NULL;
    }
    
    ListaCategorias *listaCat = criarListaCategorias();
    if (listaCat == NULL) {
        fclose(arquivoBIN);
        return NULL;
    }
    
    Alimento alimento;
    int totalAlimentos = 0;
    
    printf("\nCarregando dados do arquivo binário...\n");
    
    while (fread(&alimento, sizeof(Alimento), 1, arquivoBIN) == 1) {
        adicionarAlimento(listaCat, alimento);
        totalAlimentos++;
    }
    
    fclose(arquivoBIN);
    printf("Dados carregados com sucesso!\n");
    
    return listaCat;
}