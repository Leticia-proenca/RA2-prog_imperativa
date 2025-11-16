#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h" // para a parte do menuPrincipal
#include "ler_arquivo.h" // para a parte de ler binario
#include "linked_list.h" // para a parte de nó de categoria e liberarListaCategoria
#include "arvore_binaria.h" // para parte de no da arvore e liberarArvore 


/* Programa P2: Sistema de gerenciamento de alimentos */

int main(void) {
    // variaveis principais
    Alimento* alimentos = NULL;
    NoCategoria* listaCategorias = NULL;
    int numAlimentos;
    
    printf("=== Programa P2 ===\n\n");
    
    // le o bin
    printf("Carregando dados de dados.bin...\n");
    lerBinario("dados.bin", &alimentos, &numAlimentos);// carrega para um array simples
    printf("Total de alimentos carregados: %d\n\n", numAlimentos);
    
    // transfroma o array em uma estrutura complexa
    // cria a linked list e as arvores binarias  e organiza por categoria
    printf("Construindo estruturas de dados...\n");
    listaCategorias = construirEstruturas(alimentos, numAlimentos);
    printf("Estruturas construídas com sucesso!\n");
    
    // libera array original (dados já foram copiados para a estrutura)
    free(alimentos);
    
    // inicia o menu
    menuPrincipal(listaCategorias);
    
    // libera as arvores de categoria
    NoCategoria* catAtual = listaCategorias;
    while (catAtual != NULL) {
        liberarArvore((NoArvore*)catAtual->arvoreEnergia);
        liberarArvore((NoArvore*)catAtual->arvoreProteina);
        catAtual = catAtual->proximo;
    }
    
    // libera toda a estrutura
    liberarListaCategorias(listaCategorias);
    
    printf("\nPrograma encerrado.\n");
    
    return 0;
}
