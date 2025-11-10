#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "linked_list.h"

CategoriaAlimento stringParaCategoria(const char* str);
// le o csv
void lerCSV(const char* nomeArquivo, Alimento** alimentos, int* numAlimentos);
// escreve o bin com base no csv
void escreverBinario(const char* nomeArquivo, Alimento* alimentos, int numAlimentos);
// le o bin
void lerBinario(const char* nomeArquivo, Alimento** alimentos, int* numAlimentos);

#endif