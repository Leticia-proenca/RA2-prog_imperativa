#include <stdio.h>
#include <stdlib.h>
#include "ler_arquivo.h"

/*
 * Programa P1: Lê dados de alimentos de um arquivo CSV e salva em formato binário
 */
int main(void) {
    Alimento* alimentos = NULL;
    int numAlimentos;
    
    printf("=== Programa P1: Conversão CSV para Binário ===\n\n");
    
    /* Lê dados do arquivo CSV */
    printf("Lendo arquivo alimentos.csv...\n");
    numAlimentos = lerCSV("alimentos.csv", &alimentos);
    printf("Total de alimentos lidos: %d\n\n", numAlimentos);
    
    /* Escreve dados em arquivo binário */
    printf("Salvando dados em dados.bin...\n");
    escreverBinario("dados.bin", alimentos, numAlimentos);
    
    /* Libera memória alocada */
    free(alimentos);
    
    printf("\nProcesso concluído com sucesso!\n");
    
    return 0;
}