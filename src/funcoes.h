#ifndef MENU_H
#define MENU_H

#include "linked_list.h"
#include "arvore_binaria.h"
#include "ler_arquivo.h"

NoCategoria* construirEstruturas(Alimento* alimentos, int numAlimentos);

void listarCategorias(NoCategoria* listaCategorias);
void listarAlimentosCategoria(NoCategoria* categoria);
void listarPorEnergia(NoCategoria* categoria);
void listarPorProteina(NoCategoria* categoria);
void listarEnergiaIntervalo(NoCategoria* categoria, float min, float max);
void listarProteinaIntervalo(NoCategoria* categoria, float min, float max);

NoCategoria* escolherCategoria(NoCategoria* listaCategorias);

void removerCategoriaMenu(NoCategoria** listaCategorias);
void removerAlimentoMenu(NoCategoria* listaCategorias);
void salvarDadosAtualizados(NoCategoria* listaCategorias);
void menuPrincipal(NoCategoria* listaCategorias);

#endif