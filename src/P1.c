#include <stdio.h>
#include <stdlib.h>
#include "ler_arquivo.h"

// P1: Lê dados de alimentos de um arquivo CSV e salva em formato binário 

int main(void) {// não recebe nada
    Alimento* alimentos = NULL;// array dinâmico, preenchido pelo lerCVS
    int numAlimentos;// quantidade de alimentos 
    
    printf("=== Programa P1: Conversão CSV para Binário ===\n\n");// informativo
    
    // le o csv
    printf("Lendo arquivo alimentos.csv...\n");
    lerCSV("alimentos.csv", &alimentos, &numAlimentos);//chama a função de ler o CSV e passa o endereço do ponteiro para que a função possa mudar ele 
    printf("Total de alimentos lidos: %d\n\n", numAlimentos);// confirmação
    
    // Escreve dados em arquivo binário 
    printf("Salvando dados em dados.bin...\n");
    escreverBinario("dados.bin", alimentos, numAlimentos);/// chama  função de converte para bin
    
    // libera memoria alocada
    free(alimentos);
    
    printf("\nProcesso concluído com sucesso!\n");
    
    return 0;
}