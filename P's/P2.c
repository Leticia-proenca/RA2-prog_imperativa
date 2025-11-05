#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ler_arquivo.h"
#include "lista.h"
#include "arvore_binaria.h"

/* Variável global para controlar se houve modificação */
static int dadosModificados = 0;

/* Função para construir as estruturas de dados a partir do array de alimentos */
NoCategoria* construirEstruturas(Alimento* alimentos, int numAlimentos) {
    NoCategoria* listaCategorias = NULL;
    int i;
    
    /* Para cada alimento, insere na estrutura apropriada */
    for (i = 0; i < numAlimentos; i++) {
        CategoriaAlimento cat = alimentos[i].categoria;
        
        /* Busca ou cria a categoria */
        NoCategoria* noCategoria = buscarCategoria(listaCategorias, cat);
        if (noCategoria == NULL) {
            const char* nomeCategoria = obterNomeCategoria(cat);
            noCategoria = criarNoCategoria(cat, nomeCategoria);
            inserirCategoriaOrdenada(&listaCategorias, noCategoria);
        }
        
        /* Cria cópia do alimento e insere na lista */
        Alimento* copiaAlimento = (Alimento*)malloc(sizeof(Alimento));
        if (copiaAlimento == NULL) {
            fprintf(stderr, "Erro ao alocar memória para alimento\n");
            exit(1);
        }
        *copiaAlimento = alimentos[i];
        
        NoAlimento* noAlimento = criarNoAlimento(copiaAlimento);
        inserirAlimentoOrdenado(&noCategoria->listaAlimentos, noAlimento);
    }
    
    /* Constrói as árvores para cada categoria */
    NoCategoria* atual = listaCategorias;
    while (atual != NULL) {
        atual->arvoreEnergia = construirArvoreEnergia(atual->listaAlimentos);
        atual->arvoreProteina = construirArvoreProteina(atual->listaAlimentos);
        atual = atual->proximo;
    }
    
    return listaCategorias;
}

/* Lista todas as categorias */
void listarCategorias(NoCategoria* listaCategorias) {
    NoCategoria* atual = listaCategorias;
    int contador = 1;
    
    printf("\n=== CATEGORIAS DE ALIMENTOS ===\n");
    while (atual != NULL) {
        printf("%d. %s\n", contador, atual->nome);
        contador++;
        atual = atual->proximo;
    }
    printf("\n");
}

/* Lista todos os alimentos de uma categoria */
void listarAlimentosCategoria(NoCategoria* categoria) {
    NoAlimento* atual = categoria->listaAlimentos;
    
    printf("\n=== ALIMENTOS DA CATEGORIA: %s ===\n", categoria->nome);
    while (atual != NULL) {
        printf("%d - %s (Energia: %.2f kcal, Proteína: %.2f g)\n",
            atual->alimento->numero,
            atual->alimento->descricao,
            atual->alimento->energia,
            atual->alimento->proteina);
        atual = atual->proximo;
    }
    printf("\n");
}

/* Lista alimentos por energia (ordem decrescente) */
void listarPorEnergia(NoCategoria* categoria) {
    printf("\n=== ALIMENTOS POR ENERGIA (DECRESCENTE): %s ===\n", categoria->nome);
    percorrerDecrescente((NoArvore*)categoria->arvoreEnergia);
    printf("\n");
}

/* Lista alimentos por proteína (ordem decrescente) */
void listarPorProteina(NoCategoria* categoria) {
    printf("\n=== ALIMENTOS POR PROTEÍNA (DECRESCENTE): %s ===\n", categoria->nome);
    percorrerDecrescente((NoArvore*)categoria->arvoreProteina);
    printf("\n");
}

/* Lista alimentos com energia no intervalo */
void listarEnergiaIntervalo(NoCategoria* categoria, float min, float max) {
    printf("\n=== ALIMENTOS COM ENERGIA ENTRE %.2f e %.2f kcal: %s ===\n", 
        min, max, categoria->nome);
    percorrerIntervalo((NoArvore*)categoria->arvoreEnergia, min, max);
    printf("\n");
}

/* Lista alimentos com proteína no intervalo */
void listarProteinaIntervalo(NoCategoria* categoria, float min, float max) {
    printf("\n=== ALIMENTOS COM PROTEÍNA ENTRE %.2f e %.2f g: %s ===\n", 
        min, max, categoria->nome);
    percorrerIntervalo((NoArvore*)categoria->arvoreProteina, min, max);
    printf("\n");
}

/* Escolhe uma categoria */
NoCategoria* escolherCategoria(NoCategoria* listaCategorias) {
    int opcao;
    NoCategoria* atual = listaCategorias;
    int contador = 1;
    
    listarCategorias(listaCategorias);
    printf("Escolha o número da categoria: ");
    if (scanf("%d", &opcao) != 1) {
        while (getchar() != '\n') {} /* Limpa buffer */
        return NULL;
    }
    while (getchar() != '\n') {} /* Limpa buffer */
    
    /* Encontra a categoria selecionada */
    while (atual != NULL && contador < opcao) {
        contador++;
        atual = atual->proximo;
    }
    
    return atual;
}

/* Remove uma categoria */
void removerCategoriaMenu(NoCategoria** listaCategorias) {
    NoCategoria* categoria = escolherCategoria(*listaCategorias);
    
    if (categoria == NULL) {
        printf("Categoria inválida!\n");
        return;
    }
    
    CategoriaAlimento cat = categoria->categoria;
    char nome[100];
    strncpy(nome, categoria->nome, 99);
    nome[99] = '\0';
    
    /* Libera as árvores antes de remover */
    liberarArvore((NoArvore*)categoria->arvoreEnergia);
    liberarArvore((NoArvore*)categoria->arvoreProteina);
    categoria->arvoreEnergia = NULL;
    categoria->arvoreProteina = NULL;
    
    removerCategoria(listaCategorias, cat);
    dadosModificados = 1;
    
    printf("Categoria '%s' removida com sucesso!\n", nome);
}

/* Remove um alimento */
void removerAlimentoMenu(NoCategoria* listaCategorias) {
    NoCategoria* categoria = escolherCategoria(listaCategorias);
    int numeroAlimento;
    
    if (categoria == NULL) {
        printf("Categoria inválida!\n");
        return;
    }
    
    listarAlimentosCategoria(categoria);
    printf("Digite o número do alimento a remover: ");
    if (scanf("%d", &numeroAlimento) != 1) {
        while (getchar() != '\n') {} /* Limpa buffer */
        printf("Número inválido!\n");
        return;
    }
    while (getchar() != '\n') {} /* Limpa buffer */
    
    removerAlimento(&categoria->listaAlimentos, numeroAlimento);
    
    /* Reconstrói as árvores */
    liberarArvore((NoArvore*)categoria->arvoreEnergia);
    liberarArvore((NoArvore*)categoria->arvoreProteina);
    categoria->arvoreEnergia = construirArvoreEnergia(categoria->listaAlimentos);
    categoria->arvoreProteina = construirArvoreProteina(categoria->listaAlimentos);
    
    dadosModificados = 1;
    printf("Alimento removido com sucesso!\n");
}

/* Salva dados atualizados em arquivo binário */
void salvarDadosAtualizados(NoCategoria* listaCategorias) {
    int numAlimentos = 0;
    NoCategoria* catAtual;
    NoAlimento* alimAtual;
    Alimento* arrayAlimentos;
    int indice;
    
    /* Conta total de alimentos */
    catAtual = listaCategorias;
    while (catAtual != NULL) {
        alimAtual = catAtual->listaAlimentos;
        while (alimAtual != NULL) {
            numAlimentos++;
            alimAtual = alimAtual->proximo;
        }
        catAtual = catAtual->proximo;
    }
    
    /* Aloca array */
    arrayAlimentos = (Alimento*)malloc(numAlimentos * sizeof(Alimento));
    if (arrayAlimentos == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(1);
    }
    
    /* Copia dados para o array */
    indice = 0;
    catAtual = listaCategorias;
    while (catAtual != NULL) {
        alimAtual = catAtual->listaAlimentos;
        while (alimAtual != NULL) {
            arrayAlimentos[indice] = *(alimAtual->alimento);
            indice++;
            alimAtual = alimAtual->proximo;
        }
        catAtual = catAtual->proximo;
    }
    
    /* Salva em arquivo */
    escreverBinario("dados.bin", arrayAlimentos, numAlimentos);
    free(arrayAlimentos);
}

/* Menu principal */
void menuPrincipal(NoCategoria* listaCategorias) {
    int opcao = 0;
    NoCategoria* categoria;
    float min, max;
    
    while (opcao != 9) {
        printf("\n======================================\n");
        printf("       SISTEMA DE ALIMENTOS\n");
        printf("======================================\n");
        printf("1. Listar todas as categorias\n");
        printf("2. Listar alimentos de uma categoria\n");
        printf("3. Listar alimentos por energia (decrescente)\n");
        printf("4. Listar alimentos por proteína (decrescente)\n");
        printf("5. Listar alimentos por intervalo de energia\n");
        printf("6. Listar alimentos por intervalo de proteína\n");
        printf("7. Remover uma categoria\n");
        printf("8. Remover um alimento\n");
        printf("9. Sair\n");
        printf("======================================\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n') {} /* Limpa buffer */
            printf("Opção inválida!\n");
            continue;
        }
        while (getchar() != '\n') {} /* Limpa buffer */
        
        if (opcao == 1) {
            listarCategorias(listaCategorias);
        } else if (opcao == 2) {
            categoria = escolherCategoria(listaCategorias);
            if (categoria != NULL) {
                listarAlimentosCategoria(categoria);
            } else {
                printf("Categoria inválida!\n");
            }
        } else if (opcao == 3) {
            categoria = escolherCategoria(listaCategorias);
            if (categoria != NULL) {
                listarPorEnergia(categoria);
            } else {
                printf("Categoria inválida!\n");
            }
        } else if (opcao == 4) {
            categoria = escolherCategoria(listaCategorias);
            if (categoria != NULL) {
                listarPorProteina(categoria);
            } else {
                printf("Categoria inválida!\n");
            }
        } else if (opcao == 5) {
            categoria = escolherCategoria(listaCategorias);
            if (categoria != NULL) {
                printf("Digite o valor mínimo de energia: ");
                if (scanf("%f", &min) != 1) {
                    while (getchar() != '\n') {} /* Limpa buffer */
                    printf("Valor inválido!\n");
                    continue;
                }
                printf("Digite o valor máximo de energia: ");
                if (scanf("%f", &max) != 1) {
                    while (getchar() != '\n') {} /* Limpa buffer */
                    printf("Valor inválido!\n");
                    continue;
                }
                while (getchar() != '\n') {} /* Limpa buffer */
                listarEnergiaIntervalo(categoria, min, max);
            } else {
                printf("Categoria inválida!\n");
            }
        } else if (opcao == 6) {
            categoria = escolherCategoria(listaCategorias);
            if (categoria != NULL) {
                printf("Digite o valor mínimo de proteína: ");
                if (scanf("%f", &min) != 1) {
                    while (getchar() != '\n') {} /* Limpa buffer */
                    printf("Valor inválido!\n");
                    continue;
                }
                printf("Digite o valor máximo de proteína: ");
                if (scanf("%f", &max) != 1) {
                    while (getchar() != '\n') {} /* Limpa buffer */
                    printf("Valor inválido!\n");
                    continue;
                }
                while (getchar() != '\n') {} /* Limpa buffer */
                listarProteinaIntervalo(categoria, min, max);
            } else {
                printf("Categoria inválida!\n");
            }
        } else if (opcao == 7) {
            removerCategoriaMenu(&listaCategorias);
        } else if (opcao == 8) {
            removerAlimentoMenu(listaCategorias);
        } else if (opcao == 9) {
            printf("Encerrando programa...\n");
        } else {
            printf("Opção inválida!\n");
        }
    }
    
    /* Salva dados se houve modificação */
    if (dadosModificados == 1) {
        printf("\nSalvando alterações em dados.bin...\n");
        salvarDadosAtualizados(listaCategorias);
        printf("Dados salvos com sucesso!\n");
    }
}

/*
 * Programa P2: Sistema de gerenciamento de alimentos
 */
int main(void) {
    Alimento* alimentos = NULL;
    NoCategoria* listaCategorias = NULL;
    int numAlimentos;
    
    printf("=== Programa P2: Sistema de Alimentos ===\n\n");
    
    /* Lê dados do arquivo binário */
    printf("Carregando dados de dados.bin...\n");
    numAlimentos = lerBinario("dados.bin", &alimentos);
    printf("Total de alimentos carregados: %d\n\n", numAlimentos);
    
    /* Constrói as estruturas de dados */
    printf("Construindo estruturas de dados...\n");
    listaCategorias = construirEstruturas(alimentos, numAlimentos);
    printf("Estruturas construídas com sucesso!\n");
    
    /* Libera o array original */
    free(alimentos);
    
    /* Inicia menu principal */
    menuPrincipal(listaCategorias);
    
    /* Libera as árvores de cada categoria */
    NoCategoria* catAtual = listaCategorias;
    while (catAtual != NULL) {
        liberarArvore((NoArvore*)catAtual->arvoreEnergia);
        liberarArvore((NoArvore*)catAtual->arvoreProteina);
        catAtual = catAtual->proximo;
    }
    
    /* Libera toda a estrutura */
    liberarListaCategorias(listaCategorias);
    
    printf("\nPrograma encerrado.\n");
    
    return 0;
}