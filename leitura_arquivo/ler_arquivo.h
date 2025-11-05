#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "lista.h"

/* Funções para leitura e escrita de arquivos */
int lerCSV(const char* nomeArquivo, Alimento** alimentos);
void escreverBinario(const char* nomeArquivo, Alimento* alimentos, int numAlimentos);
int lerBinario(const char* nomeArquivo, Alimento** alimentos);

#endif