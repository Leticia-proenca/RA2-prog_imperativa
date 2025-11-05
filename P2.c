#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ler_arquivo.h"
#include "linked_list.h"
#include "arvore_binaria.h"

// variavel para o controle de modificações
static int dadosModificados = 0;

// constroi as estruturas de dados a partir do array de alimentos 
NoCategoria* construirEstruturas(Alimento* alimentos, int numAlimentos) {
    NoCategoria* listaCategorias = NULL;
    int i;
    
    // para cada alimento, insere na estrutura certa
    for (i = 0; i < numAlimentos; i++) {
        CategoriaAlimento cat = alimentos[i].categoria;
        
        // busca ou cria a categoria
        NoCategoria* noCategoria = buscarCategoria(listaCategorias, cat);
        if (noCategoria == NULL) {
            const char* nomeCategoria = obterNomeCategoria(cat);
            noCategoria = criarNoCategoria(cat, nomeCategoria);
            inserirCategoriaOrdenada(&listaCategorias, noCategoria);
        }
        
        // faz copia do alimento e coloca no array
        Alimento* copiaAlimento = (Alimento*)malloc(sizeof(Alimento));
        if (copiaAlimento == NULL) {
            fprintf(stderr, "Erro ao alocar memória para alimento\n");
            exit(1);
        }
        *copiaAlimento = alimentos[i];
        
        NoAlimento* noAlimento = criarNoAlimento(copiaAlimento);
        inserirAlimentoOrdenado(&noCategoria->listaAlimentos, noAlimento);
    }
    
    // controi arvore para cada categoria
    NoCategoria* atual = listaCategorias;
    while (atual != NULL) {
        atual->arvoreEnergia = construirArvoreEnergia(atual->listaAlimentos);
        atual->arvoreProteina = construirArvoreProteina(atual->listaAlimentos);
        atual = atual->proximo;
    }
    
    return listaCategorias;
}

// lista todas as categorias
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

// lista todos os limenots de uma categoria
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

// lista em ordem decrescente de energia os alimentos
void listarPorEnergia(NoCategoria* categoria) {
    printf("\n=== ALIMENTOS POR ENERGIA (DECRESCENTE): %s ===\n", categoria->nome);
    percorrerDecrescente((NoArvore*)categoria->arvoreEnergia);
    printf("\n");
}

// lista em ordem decrescente de proteina os alimentos
void listarPorProteina(NoCategoria* categoria) {
    printf("\n=== ALIMENTOS POR PROTEÍNA (DECRESCENTE): %s ===\n", categoria->nome);
    percorrerDecrescente((NoArvore*)categoria->arvoreProteina);
    printf("\n");
}

// lista os alimentos com valores de energia dentro de um determinado intervalo
void listarEnergiaIntervalo(NoCategoria* categoria, float min, float max) {
    printf("\n=== ALIMENTOS COM ENERGIA ENTRE %.2f e %.2f kcal: %s ===\n", 
        min, max, categoria->nome);
    percorrerIntervalo((NoArvore*)categoria->arvoreEnergia, min, max);
    printf("\n");
}

// lista os laimenots com valores de porteina dentro de um determinado intervalo
void listarProteinaIntervalo(NoCategoria* categoria, float min, float max) {
    printf("\n=== ALIMENTOS COM PROTEÍNA ENTRE %.2f e %.2f g: %s ===\n", 
        min, max, categoria->nome);
    percorrerIntervalo((NoArvore*)categoria->arvoreProteina, min, max);
    printf("\n");
}

// escolhe uma categoria
NoCategoria* escolherCategoria(NoCategoria* listaCategorias) {
    int opcao;
    NoCategoria* atual = listaCategorias;
    int contador = 1;
    
    listarCategorias(listaCategorias);
    printf("Escolha o número da categoria: ");
    if (scanf("%d", &opcao) != 1) {
        while (getchar() != '\n') {} // Limpa buffer 
        return NULL;
    }
    while (getchar() != '\n') {} // Limpa buffer 
    
    // encontra a categoria escolhida
    while (atual != NULL && contador < opcao) {
        contador++;
        atual = atual->proximo;
    }
    
    return atual;
}

// remove uma categoria
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
    
    // libera as arvores antes de deletar
    liberarArvore((NoArvore*)categoria->arvoreEnergia);
    liberarArvore((NoArvore*)categoria->arvoreProteina);
    categoria->arvoreEnergia = NULL;
    categoria->arvoreProteina = NULL;
    
    removerCategoria(listaCategorias, cat);
    dadosModificados = 1;
    
    printf("Categoria '%s' removida com sucesso!\n", nome);
}

// remove alimento
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
        while (getchar() != '\n') {} // Limpa buffer 
        printf("Número inválido!\n");
        return;
    }
    while (getchar() != '\n') {} // Limpa buffer 
    
    removerAlimento(&categoria->listaAlimentos, numeroAlimento);
    
    // refaz as arvores - atualiza
    liberarArvore((NoArvore*)categoria->arvoreEnergia);
    liberarArvore((NoArvore*)categoria->arvoreProteina);
    categoria->arvoreEnergia = construirArvoreEnergia(categoria->listaAlimentos);
    categoria->arvoreProteina = construirArvoreProteina(categoria->listaAlimentos);
    
    dadosModificados = 1;
    printf("Alimento removido com sucesso!\n");
}

// salva os novos dados no bin
void salvarDadosAtualizados(NoCategoria* listaCategorias) {
    int numAlimentos = 0;
    NoCategoria* catAtual;
    NoAlimento* alimAtual;
    Alimento* arrayAlimentos;
    int indice;
    
    // conta total de alimentos
    catAtual = listaCategorias;
    while (catAtual != NULL) {
        alimAtual = catAtual->listaAlimentos;
        while (alimAtual != NULL) {
            numAlimentos++;
            alimAtual = alimAtual->proximo;
        }
        catAtual = catAtual->proximo;
    }
    
    // aloca array
    arrayAlimentos = (Alimento*)malloc(numAlimentos * sizeof(Alimento));
    if (arrayAlimentos == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(1);
    }
    
    // copia os dados e coloca no array
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
    
    // salva em um arquivo
    escreverBinario("dados.bin", arrayAlimentos, numAlimentos);
    free(arrayAlimentos);
}

// menu principal
void menuPrincipal(NoCategoria* listaCategorias) {
    int opcao = 0;
    NoCategoria* categoria;
    float min, max;
    
    while (opcao != 9) {
        printf("  **** SISTEMA DE ALIMENTOS ****\n\n");
        printf("1. Listar todas as categorias\n");
        printf("2. Listar todos os alimentos de uma categoria\n");
        printf("3. Listar alimentos por energia (decrescente)\n");
        printf("4. Listar alimentos por proteína (decrescente)\n");
        printf("5. Listar alimentos por energia que estão dentro de um determinado intervalo\n");
        printf("6. Listar alimentos por proteína que estão dentro de um determinado intervalo\n");
        printf("7. Remover uma categoria\n");
        printf("8. Remover um alimento\n");
        printf("9. Sair\n\n");

        printf("Escolha uma opção: ");
        

        // verifica se o que o user digitou é int
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n') {} // Limpa buffer 
            printf("Opção inválida!\n");
            continue;
        }
        while (getchar() != '\n') {} // Limpa buffer 

        // funcionalidades do menu
        switch (opcao) {
            case 1:
                listarCategorias(listaCategorias);
                break;

            case 2:
                categoria = escolherCategoria(listaCategorias);
                if (categoria != NULL) {
                    listarAlimentosCategoria(categoria);
                } else {
                    printf("Categoria inválida!\n");
                }
                break;

            case 3:
                categoria = escolherCategoria(listaCategorias);
                if (categoria != NULL) {
                    listarPorEnergia(categoria);
                } else {
                    printf("Categoria inválida!\n");
                }
                break;

            case 4:
                categoria = escolherCategoria(listaCategorias);
                if (categoria != NULL) {
                    listarPorProteina(categoria);
                } else {
                    printf("Categoria inválida!\n");
                }
                break;

            case 5:
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
                break;

            case 6:
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
                break;

            case 7:
                removerCategoriaMenu(&listaCategorias);
                break;

            case 8:
                removerAlimentoMenu(listaCategorias);
                break;

            case 9:
                printf("Encerrando programa...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }
    }
    
    // se tiver modificação, salva elas
    if (dadosModificados == 1) {
        printf("\nSalvando alterações em dados.bin...\n");
        salvarDadosAtualizados(listaCategorias);
        printf("Dados salvos com sucesso!\n");
    }
}

/* Programa P2: Sistema de gerenciamento de alimentos */

int main(void) {
    Alimento* alimentos = NULL;
    NoCategoria* listaCategorias = NULL;
    int numAlimentos;
    
    printf("=== Programa P2: Sistema de Alimentos ===\n\n");
    
    // le o bin
    printf("Carregando dados de dados.bin...\n");
    lerBinario("dados.bin", &alimentos, &numAlimentos);
    printf("Total de alimentos carregados: %d\n\n", numAlimentos);
    
    /* Constrói as estruturas de dados */
    printf("Construindo estruturas de dados...\n");
    listaCategorias = construirEstruturas(alimentos, numAlimentos);
    printf("Estruturas construídas com sucesso!\n");
    
    // libera array original
    free(alimentos);
    
    // inicia o menu
    menuPrincipal(listaCategorias);
    
    // libera as arvores de categoria
    NoCategoria* catAtual = listaCategorias;
    while (catAtual != NULL) {
        liberarArvore((NoArvore*)catAtual->arvoreEnergia);
        liberarArvore((NoArvore*)catAtual->arvoreProteina);
        catAtual = catAtual->proximo;
    }
    
    // libera toda a estrutura
    liberarListaCategorias(listaCategorias);
    
    printf("\nPrograma encerrado.\n");
    
    return 0;
}