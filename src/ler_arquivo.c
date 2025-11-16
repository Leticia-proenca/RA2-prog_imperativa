#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ler_arquivo.h"

// Função auxiliar para converter string em CategoriaAlimento
CategoriaAlimento stringParaCategoria(const char* str) {
    CategoriaAlimento categoria = CEREAIS_DERIVADOS;
    
    if (strcmp(str, "Cereais e derivados") == 0) {
        categoria = CEREAIS_DERIVADOS;
    } else if (strcmp(str, "Verduras, hortaliças e derivados") == 0) {
        categoria = VERDURAS_HORTALICAS_DERIVADOS;
    } else if (strcmp(str, "Frutas e derivados") == 0) {
        categoria = FRUTAS_DERIVADOS;
    } else if (strcmp(str, "Gorduras e óleos") == 0) {
        categoria = GORDURAS_OLEOS;
    } else if (strcmp(str, "Pescados e frutos do mar") == 0) {
        categoria = PESCADOS_FRUTOS_MAR;
    } else if (strcmp(str, "Carnes e derivados") == 0) {
        categoria = CARNES_DERIVADOS;
    } else if (strcmp(str, "Leite e derivados") == 0) {
        categoria = LEITE_DERIVADOS;
    } else if (strcmp(str, "Bebidas") == 0) {
        categoria = BEBIDAS;
    } else if (strcmp(str, "Ovos e derivados") == 0) {
        categoria = OVOS_DERIVADOS;
    } else if (strcmp(str, "Produtos açucarados") == 0) {
        categoria = PRODUTOS_ACUCARADOS;
    } else if (strcmp(str, "Miscelâneas") == 0) {
        categoria = MISCELANEAS;
    } else if (strcmp(str, "Outros industrializados") == 0) {
        categoria = OUTROS_INDUSTRIALIZADOS;
    } else if (strcmp(str, "Alimentos industrializados") == 0) {
        categoria = OUTROS_INDUSTRIALIZADOS;
    } else if (strcmp(str, "Alimentos preparados") == 0) {
        categoria = ALIMENTOS_PREPARADOS;
    } else if (strcmp(str, "Leguminosas e derivados") == 0) {
        categoria = LEGUMINOSAS_DERIVADOS;
    } else if (strcmp(str, "Nozes e sementes") == 0) {
        categoria = NOZES_SEMENTES;
    }
    
    return categoria;
}

// Função lerCSV 
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
    char categoriaStr[100];
    
    // pula cabeçalho
    if (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (linha[0] < '0' || linha[0] > '9') {
            // tinha cabeçalho, continua
        } else {
            // não tinha cabeçalho, volta
            rewind(arquivo);
        }
    }
    
    // loop principal de leitura do arquivo
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // remove quebra linha
        linha[strcspn(linha, "\r\n")] = '\0';
        
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
        
        // pega as info linha por linha (agora lendo categoria como string)
        if (sscanf(linha, "%d,%[^,],%f,%f,%[^\n]",
                &(*alimentos)[contador].numero,
                (*alimentos)[contador].descricao,
                &(*alimentos)[contador].energia,
                &(*alimentos)[contador].proteina,
                categoriaStr) == 5) {
            
            // converte string para enum
            (*alimentos)[contador].categoria = stringParaCategoria(categoriaStr);
            contador++;
        }
    }
    
    fclose(arquivo);
    *numAlimentos = contador;
}

// transcreve os dados para bin
void escreverBinario(const char* nomeArquivo, Alimento* alimentos, int numAlimentos) {
    FILE* arquivo = fopen(nomeArquivo, "wb");// wb = write binary

    if (arquivo == NULL) {// verifica se consegui criar um arquivo
        fprintf(stderr, "Erro ao criar arquivo %s\n", nomeArquivo);
        exit(1);
    }
    
    // escreve o numero de alimentos
    fwrite(&numAlimentos, sizeof(int), 1, arquivo);
    // numAlimentos = endereço da variavel | sizeof: tamanho do alimento | 1: quantidade a se escrever | arquivo: onde escrever
    
    // escreve todos os alimentos d euma vez
    fwrite(alimentos, sizeof(Alimento), numAlimentos, arquivo);
    // alimentos: ponteiro para array | sizeof: tamanho de cada estrutura | numAlimentos: quantidades de alimentos a se escrever
    
    fclose(arquivo);
    printf("Arquivo binário criado com sucesso: %d alimentos salvos.\n", numAlimentos);
}

// le o bin
void lerBinario(const char* nomeArquivo, Alimento** alimentos, int* numAlimentos) { // mesmo parametro do ler csv
    FILE* arquivo = fopen(nomeArquivo, "rb");// rb: read binary

    if (arquivo == NULL) {// verifica se conseguiu abrir o arquivo criado na função anterior
        fprintf(stderr, "Erro ao abrir arquivo %s\n", nomeArquivo);
        exit(1);
    }
    
    // le numero de alimentos, importante para o funcionamento do arquivo P1
    if (fread(numAlimentos, sizeof(int), 1, arquivo) != 1) { // !=1: verifica se falhou 
        // retorna número de alimentos lidos com sucesso
        fprintf(stderr, "Erro ao ler número de alimentos\n");
        fclose(arquivo);
        exit(1);
    }
    
    // aloca memória para os alimenotos
    *alimentos = (Alimento*)malloc(*numAlimentos * sizeof(Alimento));
    if (*alimentos == NULL) {// verifica se a alocação falhou
        fprintf(stderr, "Erro ao alocar memória\n");
        fclose(arquivo);
        exit(1);
    }
    
    // le todos os alimentos de uma vez
    if (fread(*alimentos, sizeof(Alimento), *numAlimentos, arquivo) != (size_t)(*numAlimentos)) {
        // !=(size_t)(*numAlimentos): converte para o tipo certo de comparação
        // verifica se leu quantidade esperada

        fprintf(stderr, "Erro ao ler dados dos alimentos\n");
        free(*alimentos);
        fclose(arquivo);
        exit(1);
    }// se falhou libera memória alocada
    
    fclose(arquivo);
}