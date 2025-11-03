#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "P1.h"
#include "alimentos.h"

#define MAX_NOME 100
#define MAX_LINHA 512

// estrutura padrão para o aramazenamenot das informações od alimento
typedef struct {
    int id;
    char nome[MAX_NOME];
    float calorias;
    float proteinas;
    float carboidratos;
    float gorduras;
} Alimento;

// Função para remover quebras de linha
void removerQuebraLinha(char *str) {
    str[strcspn(str, "\n")] = '\0';
    str[strcspn(str, "\r")] = '\0';
}

int abrir_arquivo() {
    FILE *arquivoCSV; //arquivo que armazena os alimentos em csv
    FILE *arquivoBIN; // arquivo binário
    char linha[MAX_LINHA];
    Alimento alimento;
    int contador = 0;
    
    // Abre o arquivo CSV para leitura
    arquivoCSV = fopen("alimentos.csv", "r");

    //verificação se o arquivo CSV possui algum informação
    if (arquivoCSV == NULL) {
        printf("Erro ao abrir o arquivo CSV!\n");
        printf("Certifique-se de que o arquivo 'alimentos.csv' existe.\n");
        return 1;
    }
    
    // Abre o arquivo binário para escrita
    arquivoBIN = fopen("dados.bin", "wb");
    if (arquivoBIN == NULL) {
        printf("Erro ao criar o arquivo binário!\n");
        fclose(arquivoCSV);
        return 1;
    }
    
    printf("Lendo arquivo CSV e convertendo para binário...\n\n"); // ponto de verificação
    
    // Lê cada linha do CSV
    while (fgets(linha, sizeof(linha), arquivoCSV) != NULL) {
        removerQuebraLinha(linha);
        
        // Parse da linha CSV (formato: id,nome,calorias,proteinas,carboidratos,gorduras)
        char *token;
        int campo = 0;
        char linhaCopia[MAX_LINHA];
        strcpy(linhaCopia, linha);
        
        token = strtok(linhaCopia, ","); //remove as virgulas do csv
        while (token != NULL && campo < 6) {
            switch(campo) {
                case 0: // ID
                    alimento.id = atoi(token);
                    break;
                case 1: // Nome
                    strncpy(alimento.nome, token, MAX_NOME - 1);
                    alimento.nome[MAX_NOME - 1] = '\0';
                    break;
                case 2: // Calorias
                    alimento.calorias = atof(token);
                    break;
                case 3: // Proteínas
                    alimento.proteinas = atof(token);
                    break;
                case 4: // Carboidratos
                    alimento.carboidratos = atof(token);
                    break;
                case 5: // Gorduras
                    alimento.gorduras = atof(token);
                    break;
            }
            campo++;
            token = strtok(NULL, ",");
        }
        
        // Escreve o registro no arquivo binário
        fwrite(&alimento, sizeof(Alimento), 1, arquivoBIN);
        contador++;
        
    }
    
    // Fecha os arquivos
    fclose(arquivoCSV);
    fclose(arquivoBIN);

    printf("Arquivos fechados");
    
    return 0;
}