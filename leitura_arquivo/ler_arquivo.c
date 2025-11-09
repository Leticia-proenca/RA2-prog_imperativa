#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ler_arquivo.h"

// le o csv
void lerCSV(const char* nomeArquivo, Alimento** alimentos, int* numAlimentos) {// nomeArquivo é uma variavel de parametro
    // Alimento** é um ponteiro duplo que passa o endereço do ponteiro Alimentos 
    // numAlimentos é um ponteiro que retona o número de alimentos lidos
    FILE* arquivo = fopen(nomeArquivo, "r");// abre arquivo no read
    if (arquivo == NULL) {// se não conseguir abrir
        fprintf(stderr, "Erro ao abrir arquivo %s\n", nomeArquivo);
        exit(1);// encerra programa com error
    }
    
    int capacidade = 100;// memória inicial alocada
    int contador = 0;
    *alimentos = (Alimento*)malloc(capacidade * sizeof(Alimento));// aloca inicalmente memória para 100 alimentos

    if (*alimentos == NULL) {// verifica se a alocação falhou
        fprintf(stderr, "Erro ao alocar memória\n");
        fclose(arquivo);
        exit(1);
    }
    
    char linha[500];// buffer para armazenar linhas do csv
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
    
    // loop principal de leitura do arquivo
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {// le linha por linha, fgets retonra null quando termina o csv
        // remove quebra linha
        linha[strcspn(linha, "\r")] = '\0';
        
        // realoca se necessario, se ta cheio, dobra o tamanho
        if (contador >= capacidade) {
            capacidade *= 2;
            *alimentos = (Alimento*)realloc(*alimentos, capacidade * sizeof(Alimento));// redimensiona o bloco de memória existente
            // pode mudar o bloco de lugar mas mantem todos os dados

            if (*alimentos == NULL) { // verifica se a realocação falhou
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
                &categoria) == 5) { // o ==5 verifica se leu os 5 dados que precisava
            
            (*alimentos)[contador].categoria = (CategoriaAlimento)categoria;/// acessa a possição contador do array
            contador++;
        }
    }
    
    fclose(arquivo);
    *numAlimentos = contador;// atualiza ponteiro que ve a quantidade de alimentos lidos
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