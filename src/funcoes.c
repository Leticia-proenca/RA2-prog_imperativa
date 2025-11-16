#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ler_arquivo.h"
#include "linked_list.h"
#include "arvore_binaria.h"

static int dadosModificados = 0;

// constroi as estruturas de dados a partir do array de alimentos, tranforma array de alimentos em estrutura de dados comlexa
NoCategoria* construirEstruturas(Alimento* alimentos, int numAlimentos) {// recebe array de alimentos e retona a linked list
    NoCategoria* listaCategorias = NULL;// head da linked list
    int i;// contador
    
    // para cada alimento, insere na estrutura certa
    for (i = 0; i < numAlimentos; i++) {
        CategoriaAlimento cat = alimentos[i].categoria;// extrai a actegoria de cada alimento (enum)
        
        NoCategoria* noCategoria = buscarCategoria(listaCategorias, cat);// busca categoria
        if (noCategoria == NULL) {// se a categoria não existe ela cria
            const char* nomeCategoria = obterNomeCategoria(cat);// pega o nome da categoria
            noCategoria = criarNoCategoria(cat, nomeCategoria);// cria um novo nó
            inserirCategoriaOrdenada(&listaCategorias, noCategoria);// insere na lista
        }
        
        // faz copia do alimento e coloca no array, copia todos os campos do alimento
        Alimento* copiaAlimento = (Alimento*)malloc(sizeof(Alimento));
        if (copiaAlimento == NULL) {
            fprintf(stderr, "Erro ao alocar memória para alimento\n");// verifica erro de alocação de memória
            exit(1);
        }
        *copiaAlimento = alimentos[i];
        
        NoAlimento* noAlimento = criarNoAlimento(copiaAlimento);// cria nó para o alimento
        inserirAlimentoOrdenado(&noCategoria->listaAlimentos, noAlimento);// insere na lista de alimentos em ordem alfabética
    }
    
    // depois de processar todos os alimentos, percorre cada categoria 
    // controi arvore para cada categoria
    // pra cada categoria cria 2 arvores: energia e proteina
    NoCategoria* atual = listaCategorias;
    while (atual != NULL) {
        atual->arvoreEnergia = construirArvoreEnergia(atual->listaAlimentos);
        atual->arvoreProteina = construirArvoreProteina(atual->listaAlimentos);
        atual = atual->proximo;
    }
    
    return listaCategorias;// retorna estrutura completa montada
}

// lista todas as categorias
void listarCategorias(NoCategoria* listaCategorias) {
    NoCategoria* atual = listaCategorias;
    int contador = 1;// numera as categorias para o menu
    
    printf("\n=== CATEGORIAS DE ALIMENTOS ===\n");
    while (atual != NULL) {//percorre a lista printando o nome e o númmero
        printf("%d. %s\n", contador, atual->nome);
        contador++;
        atual = atual->proximo;
    }
    printf("\n");
}

// lista todos os limentos de uma categoria
void listarAlimentosCategoria(NoCategoria* categoria) {// começa do primeiro alimento da lista
    NoAlimento* atual = categoria->listaAlimentos;
    
    printf("\n=== ALIMENTOS DA CATEGORIA: %s ===\n", categoria->nome);
    while (atual != NULL) {// percorre a lista e printa as info
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
    percorrerDecrescente((NoArvore*)categoria->arvoreEnergia);// puxa da arvore binaria: direita->raiz->esquerda
    printf("\n");
}

// lista em ordem decrescente de proteina os alimentos, mesma coisa q listaPorEnergia
void listarPorProteina(NoCategoria* categoria) {
    printf("\n=== ALIMENTOS POR PROTEÍNA (DECRESCENTE): %s ===\n", categoria->nome);
    percorrerDecrescente((NoArvore*)categoria->arvoreProteina);
    printf("\n");
}

// lista os alimentos com valores de energia dentro de um determinado intervalo
void listarEnergiaIntervalo(NoCategoria* categoria, float min, float max) {
    printf("\n=== ALIMENTOS COM ENERGIA ENTRE %.2f e %.2f kcal: %s ===\n", 
        min, max, categoria->nome);
    percorrerIntervalo((NoArvore*)categoria->arvoreEnergia, min, max);// puxa da arvore binaria
    printf("\n");
}

// lista os laimenots com valores de porteina dentro de um determinado intervalo
void listarProteinaIntervalo(NoCategoria* categoria, float min, float max) {
    printf("\n=== ALIMENTOS COM PROTEÍNA ENTRE %.2f e %.2f g: %s ===\n", 
        min, max, categoria->nome);
    percorrerIntervalo((NoArvore*)categoria->arvoreProteina, min, max);
    printf("\n");
}

// função de limpar buffer do teclado
void limparBuffer(){
    while (getchar() != '\n') {}
}

// função para limpar o termal
void limparTerminal(){
    system("clear");
}

// escolhe uma categoria
NoCategoria* escolherCategoria(NoCategoria* listaCategorias) {// retorna o ponteiro da categoria ou null se não encontrado
    int opcao;
    NoCategoria* atual = listaCategorias;
    int contador = 1;
    
    listarCategorias(listaCategorias);// lista as categorias
    printf("Escolha o número da categoria: ");// input do user
    if (scanf("%d", &opcao) != 1) {// verifica o input
        limparBuffer(); // Limpa buffer 
        return NULL;
    }
    limparBuffer(); // Limpa buffer 
    
    // encontra a categoria escolhida
    while (atual != NULL && contador < opcao) {// vai navegando até achar a categoria na posição escolhida, só avança até o que foi escolhido
        contador++;
        atual = atual->proximo;
    }
    
    return atual;
}

// remove uma categoria e seu alimentos
void removerCategoriaMenu(NoCategoria** listaCategorias) {// ponteiro duplo pq modifica o head da linked list
    NoCategoria* categoria = escolherCategoria(*listaCategorias);
    
    if (categoria == NULL) {// valida escolha do user
        printf("Categoria inválida!\n");
        return;
    }
    
    // guarda as info antes de deletar
    CategoriaAlimento cat = categoria->categoria;
    char nome[100];
    strncpy(nome, categoria->nome, 99);
    nome[99] = '\0';
    
    // libera as arvores antes de deletar
    liberarArvore((NoArvore*)categoria->arvoreEnergia);
    liberarArvore((NoArvore*)categoria->arvoreProteina);
    categoria->arvoreEnergia = NULL;// null para evitar uso após a liberação, previne memory leak
    categoria->arvoreProteina = NULL;
    
    removerCategoria(listaCategorias, cat);// remove a categoria
    dadosModificados = 1;// mostra modificação
    
    printf("Categoria '%s' removida com sucesso!\n", nome);// confirma
}

// remove alimento 
void removerAlimentoMenu(NoCategoria* listaCategorias) {
    NoCategoria* categoria = escolherCategoria(listaCategorias);// primeiro o user escolhe a categoria do alimento
    int numeroAlimento;
    
    if (categoria == NULL) {// validação da esolha de categoria
        printf("Categoria inválida!\n");
        return;
    }
    
    listarAlimentosCategoria(categoria);// lista todos os alimentos da categoria
    printf("Digite o número do alimento a remover: ");
    if (scanf("%d", &numeroAlimento) != 1) {// digita o númeor do alimento
        limparBuffer(); // Limpa buffer 
        printf("Número inválido!\n");// valida entrada do numero do alimento
        return;
    }
    limparBuffer(); // Limpa buffer 
    
    removerAlimento(&categoria->listaAlimentos, numeroAlimento);// remove da linnked list
    
    // refaz as arvores - atualiza
    // primeiro retira o alimento da arvore binaria
    liberarArvore((NoArvore*)categoria->arvoreEnergia);
    liberarArvore((NoArvore*)categoria->arvoreProteina);
    // reconstroi a arvore sem o alimento
    categoria->arvoreEnergia = construirArvoreEnergia(categoria->listaAlimentos);
    categoria->arvoreProteina = construirArvoreProteina(categoria->listaAlimentos);
    
    dadosModificados = 1;// marca a modificação
    printf("Alimento removido com sucesso!\n");
}

// salva os novos dados no bin
void salvarDadosAtualizados(NoCategoria* listaCategorias) {
    //salva estruturas de volta para o bin
    // converte linked list -> array -> bin
    int numAlimentos = 0;
    NoCategoria* catAtual;
    NoAlimento* alimAtual;
    Alimento* arrayAlimentos;
    int indice;
    
    // conta total de alimentos
    catAtual = listaCategorias;
    while (catAtual != NULL) {// percorre todas as categorias
        alimAtual = catAtual->listaAlimentos;
        while (alimAtual != NULL) {// percorre todos os alimentos
            numAlimentos++;
            alimAtual = alimAtual->proximo;
        }
        catAtual = catAtual->proximo;// conta total de alimentos
    }
    
    // aloca array temprario para salvar
    arrayAlimentos = (Alimento*)malloc(numAlimentos * sizeof(Alimento));
    if (arrayAlimentos == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(1);
    }
    
    // copia os dados e coloca no array, pega array pq a função escreverBinario puxa um array
    indice = 0;
    catAtual = listaCategorias;
    while (catAtual != NULL) {// percorre todas as categorias
        alimAtual = catAtual->listaAlimentos;
        while (alimAtual != NULL) {// percorre todos os alimentos
            arrayAlimentos[indice] = *(alimAtual->alimento);// copia a estrutura inteira do alimento
            indice++;
            alimAtual = alimAtual->proximo;
        }
        catAtual = catAtual->proximo;
    }
    
    // salva em um arquivo
    escreverBinario("dados.bin", arrayAlimentos, numAlimentos);
    free(arrayAlimentos);// libera o array temporario
}

// menu principal
void menuPrincipal(NoCategoria* listaCategorias) {
    // variavei para armazenar as esolhas do user
    int opcao = 0;
    NoCategoria* categoria;
    float min, max;
    
    while (opcao != 9) {
        //print do menu
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

        printf("Escolha uma opção: ");//input do user
        

        // verifica se o que o user digitou é int
        if (scanf("%d", &opcao) == 1) {
            limparBuffer(); // Limpa buffer 
        } else {
            limparBuffer(); // Limpa buffer 
            printf("Opção inválida!\n");
        }

        // funcionalidades do menu
        if (opcao == 1) {
                limparTerminal();  
                listarCategorias(listaCategorias);
            } else if (opcao == 2){
                limparTerminal();  
                categoria = escolherCategoria(listaCategorias);// pede pro usuario escolher a categoria
                if (categoria != NULL) {
                    listarAlimentosCategoria(categoria);// lsta os alimentos
                } else {
                    printf("Categoria inválida!\n");
                }
            } else if (opcao == 3) {
                limparTerminal();  
                categoria = escolherCategoria(listaCategorias); 
                if (categoria != NULL) {
                    listarPorEnergia(categoria);// puxa a arvore binaria da categoria escolhida pelo user
                } else {
                    printf("Categoria inválida!\n");
                }
            } else if (opcao == 4){
                limparTerminal();  
                categoria = escolherCategoria(listaCategorias);
                if (categoria != NULL) {
                    listarPorProteina(categoria);
                } else {
                    printf("Categoria inválida!\n");
                }
            } else if (opcao == 5){
                limparTerminal();  
                categoria = escolherCategoria(listaCategorias);
                if (categoria != NULL) {
                    printf("Digite o valor mínimo de energia: ");// pede o minimo do intervalo

                    if (scanf("%f", &min) == 1) {
                        printf("Digite o valor máximo de energia: ");

                        if (scanf("%f", &max) == 1){
                            limparBuffer(); // limpa buffer
                            listarEnergiaIntervalo(categoria, min, max);

                        } else{
                            limparBuffer(); //limpa buffer
                            printf("Valor invalido!\n");
                        }

                    } else {
                        limparBuffer(); //limpa buffer
                        printf("Valor inválido!\n");
                    }

                } else {
                    printf("Categoria inválida!");
                }
            } else if (opcao == 6){
                limparTerminal();  
                categoria = escolherCategoria(listaCategorias);

                if (categoria != NULL) {
                    printf("Digite o valor mínimo de proteína: ");

                    if (scanf("%f", &min) == 1) {
                        printf("Digite o valor máximo de proteína: ");

                        if (scanf("%f", &max) == 1){
                            limparBuffer(); // limpa buffer
                            listarProteinaIntervalo(categoria, min, max);
                        } else {
                            limparBuffer(); //limpa buffer
                            printf("Valor invalido!\n");
                        }
                    } else {
                        limparBuffer(); //limpa buffer
                        printf("Valor inválido!\n");
                    }
                } else {
                    printf("Categoria inválida!");
                }
            } else if (opcao == 7){
                limparTerminal();  
                removerCategoriaMenu(&listaCategorias);
            } else if (opcao == 8){
                limparTerminal();
                removerAlimentoMenu(listaCategorias);
            }
        }
    
    // se tiver modificação, salva elas
    if (dadosModificados == 1) {
        printf("\nSalvando alterações em dados.bin...\n");
        salvarDadosAtualizados(listaCategorias);
        printf("Dados salvos com sucesso!\n");
    }
}