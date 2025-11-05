#include <stdio.h>
#include <stdlib.h>
#include "ler_arquivo.h"

// P1: Lê dados de alimentos de um arquivo CSV e salva em formato binário 

int main(void) {
    Alimento* alimentos = NULL;
    int numAlimentos;
    
    printf("=== Programa P1: Conversão CSV para Binário ===\n\n");
    
    // le o csv
    printf("Lendo arquivo alimentos.csv...\n");
    lerCSV("alimentos.csv", &alimentos, &numAlimentos);
    printf("Total de alimentos lidos: %d\n\n", numAlimentos);
    
    /* Escreve dados em arquivo binário */
    printf("Salvando dados em dados.bin...\n");
    escreverBinario("dados.bin", alimentos, numAlimentos);
    
    // libera memoria alocada
    free(alimentos);
    
    printf("\nProcesso concluído com sucesso!\n");
    
    return 0;
}