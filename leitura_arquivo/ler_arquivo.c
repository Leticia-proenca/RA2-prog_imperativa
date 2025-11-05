#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ler_arquivo.h"

/* Lê dados do arquivo CSV e retorna o número de alimentos lidos */
int lerCSV(const char* nomeArquivo, Alimento** alimentos) {
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
    
    /* Pula o cabeçalho se existir */
    if (fgets(linha, sizeof(linha), arquivo) != NULL) {
        /* Verifica se é cabeçalho (contém letras) */
        if (linha[0] < '0' || linha[0] > '9') {
            /* Era cabeçalho, não faz nada */
        } else {
            /* Não era cabeçalho, processa a linha */
            rewind(arquivo);
        }
    }
    
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        /* Remove quebra de linha */
        linha[strcspn(linha, "\n")] = '\0';
        linha[strcspn(linha, "\r")] = '\0';
        
        /* Realoca se necessário */
        if (contador >= capacidade) {
            capacidade *= 2;
            *alimentos = (Alimento*)realloc(*alimentos, capacidade * sizeof(Alimento));
            if (*alimentos == NULL) {
                fprintf(stderr, "Erro ao realocar memória\n");
                fclose(arquivo);
                exit(1);
            }
        }
        
        /* Parse da linha CSV */
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
    return contador;
}

/* Escreve dados em arquivo binário */
void escreverBinario(const char* nomeArquivo, Alimento* alimentos, int numAlimentos) {
    FILE* arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao criar arquivo %s\n", nomeArquivo);
        exit(1);
    }
    
    /* Escreve o número de alimentos */
    fwrite(&numAlimentos, sizeof(int), 1, arquivo);
    
    /* Escreve todos os alimentos */
    fwrite(alimentos, sizeof(Alimento), numAlimentos, arquivo);
    
    fclose(arquivo);
    printf("Arquivo binário criado com sucesso: %d alimentos salvos.\n", numAlimentos);
}

/* Lê dados do arquivo binário e retorna o número de alimentos lidos */
int lerBinario(const char* nomeArquivo, Alimento** alimentos) {
    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo %s\n", nomeArquivo);
        exit(1);
    }
    
    int numAlimentos;
    
    /* Lê o número de alimentos */
    if (fread(&numAlimentos, sizeof(int), 1, arquivo) != 1) {
        fprintf(stderr, "Erro ao ler número de alimentos\n");
        fclose(arquivo);
        exit(1);
    }
    
    /* Aloca memória para os alimentos */
    *alimentos = (Alimento*)malloc(numAlimentos * sizeof(Alimento));
    if (*alimentos == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        fclose(arquivo);
        exit(1);
    }
    
    /* Lê todos os alimentos */
    if (fread(*alimentos, sizeof(Alimento), numAlimentos, arquivo) != (size_t)numAlimentos) {
        fprintf(stderr, "Erro ao ler dados dos alimentos\n");
        free(*alimentos);
        fclose(arquivo);
        exit(1);
    }
    
    fclose(arquivo);
    return numAlimentos;
}