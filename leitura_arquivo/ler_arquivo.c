#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ler_arquivo.h"

// le o csv
void lerCSV(const char* nomeArquivo, Alimento** alimentos, int* numAlimentos) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo %s\n", nomeArquivo);
        exit(1);
    }
    
    int capacidade = 100;
    int contador = 0;
    *alimentos = (Alimento*)malloc(capacidade * sizeof(Alimento));
    if (*alimentos == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        fclose(arquivo);
        exit(1);
    }
    
    char linha[500];
    int categoria;
    
    // pula cabeçalho
    if (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // verifica se o cabeçalho tem letra
        if (linha[0] < '0' || linha[0] > '9') {
            // se tinha cabeçalho então não faz nada
        } else {
            // se não tinha cabeçalho então volta - faz rewind
            rewind(arquivo);
        }
    }
    
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // remove quebra linha
        linha[strcspn(linha, "\n")] = '\0';
        linha[strcspn(linha, "\r")] = '\0';
        
        // realoca se necessario
        if (contador >= capacidade) {
            capacidade *= 2;
            *alimentos = (Alimento*)realloc(*alimentos, capacidade * sizeof(Alimento));
            if (*alimentos == NULL) {
                fprintf(stderr, "Erro ao realocar memória\n");
                fclose(arquivo);
                exit(1);
            }
        }
        
        // pega as info linha por linha 
        if (sscanf(linha, "%d,%[^,],%f,%f,%d",
                &(*alimentos)[contador].numero,
                (*alimentos)[contador].descricao,
                &(*alimentos)[contador].energia,
                &(*alimentos)[contador].proteina,
                &categoria) == 5) {
            
            (*alimentos)[contador].categoria = (CategoriaAlimento)categoria;
            contador++;
        }
    }
    
    fclose(arquivo);
    *numAlimentos = contador;
}

// transcreve os dados para bin
void escreverBinario(const char* nomeArquivo, Alimento* alimentos, int numAlimentos) {
    FILE* arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao criar arquivo %s\n", nomeArquivo);
        exit(1);
    }
    
    // escreve o numero de alimentos
    fwrite(&numAlimentos, sizeof(int), 1, arquivo);
    
    // escreve todos os alimentos
    fwrite(alimentos, sizeof(Alimento), numAlimentos, arquivo);
    
    fclose(arquivo);
    printf("Arquivo binário criado com sucesso: %d alimentos salvos.\n", numAlimentos);
}

// le o bin
void lerBinario(const char* nomeArquivo, Alimento** alimentos, int* numAlimentos) {
    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo %s\n", nomeArquivo);
        exit(1);
    }
    
    // le numero de alimentos, importante para o funcionamento do arquivo P1
    if (fread(numAlimentos, sizeof(int), 1, arquivo) != 1) {
        fprintf(stderr, "Erro ao ler número de alimentos\n");
        fclose(arquivo);
        exit(1);
    }
    
    // aloca memória para os alimenotos
    *alimentos = (Alimento*)malloc(*numAlimentos * sizeof(Alimento));
    if (*alimentos == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        fclose(arquivo);
        exit(1);
    }
    
    // le os alimentos
    if (fread(*alimentos, sizeof(Alimento), *numAlimentos, arquivo) != (size_t)(*numAlimentos)) {
        fprintf(stderr, "Erro ao ler dados dos alimentos\n");
        free(*alimentos);
        fclose(arquivo);
        exit(1);
    }
    
    fclose(arquivo);
}