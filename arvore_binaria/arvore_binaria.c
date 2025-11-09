#include <stdio.h>
#include <stdlib.h>
#include "arvore_binaria.h"

// cria nó 
NoArvore* criarNoArvore(float chave, NoAlimento* alimento) { // chave é usada para ordenar a arvore
    NoArvore* novo = (NoArvore*)malloc(sizeof(NoArvore)); //malloc retorna ponteiro void que é convertido para NoArvore*   /  sizeoff determina o tamanho da memória
    if (novo == NULL) { // verifica se tem memória para alocação
        fprintf(stderr, "Não possui memória o suficiente para o nó.\n");
        exit(1);// encerra o programa
    }
    
    novo->chave = chave; //inicializa o valor chave com o  valor recebido de energia ou proteina
    novo->alimento = alimento; // pega o alimento e associa ele ao no da arvore
    novo->esquerda = NULL;
    novo->direita = NULL;
    
    return novo; //retorna o ponteiro do nó criado
}

// inserir nó na árvore
// insere de uma maaneira que já fique organizada de menor para maior 
NoArvore* inserirNaArvore(NoArvore* raiz, float chave, NoAlimento* alimento) {// recebe a raiz atual, a chave e o alimento
    if (raiz == NULL) { // se não possui uma raiz para a arvore cria um nó
        return criarNoArvore(chave, alimento);// retorna o novo nó que será ligado
    }
    
    if (chave < raiz->chave) { // se a chave é menor que a raiz atual insere em um subarvore na esquerda
        raiz->esquerda = inserirNaArvore(raiz->esquerda, chave, alimento);// resultado é o novo filho esquerdo
    } else {
        raiz->direita = inserirNaArvore(raiz->direita, chave, alimento);// se a chave for igual ou maior que a raiz  coloca os valores na subarvore da direita
    }
    
    return raiz; // retorna a raiz atual
}

// ver a árvore em ordem decrescente
void percorrerDecrescente(NoArvore* raiz) {
    if (raiz == NULL) {// caso base
        return;
    }
    
    percorrerDecrescente(raiz->direita); // precessa subarvore da direita - valores maiores
    
    printf("%d - %s: %.2f\n", 
        raiz->alimento->alimento->numero, // numero do alimento
        raiz->alimento->alimento->descricao,// descricao do alimento
        raiz->chave);// valor da chave na arvore
    
    percorrerDecrescente(raiz->esquerda); // processa subarvore da esquerda - valores menores

    // ordem que precessa: direita, raiz, esquerda = decrescente
}

// ve a arvore no intervalo do min e max fornecido pelo user
void percorrerIntervalo(NoArvore* raiz, float min, float max) { // coloca os limites min e max e só percorre entre eles
    if (raiz == NULL) {
        return;
    }
    
    // se estiver na esquerda
    if (raiz->chave > min) { /// só vai para a esquerda se a raiz for maior que o min
        percorrerIntervalo(raiz->esquerda, min, max);
    }
    
    // verifica se esta no intervalo fornecido, se sim imprime as info do alimento
    if (raiz->chave >= min && raiz->chave <= max) {
        printf("%d - %s: %.2f\n", 
            raiz->alimento->alimento->numero,
            raiz->alimento->alimento->descricao,
            raiz->chave);
    }
    
    // se esta na direita, só vai se for menor que o max
    if (raiz->chave < max) {
        percorrerIntervalo(raiz->direita, min, max);
    }
}

// libera árvore inteira
// libera toda a memória alocada pela arvore
void liberarArvore(NoArvore* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvore(raiz->esquerda);// libera primeiro os filho e depois a raiz, se libera raiz antes perde acesso aos filhos
    liberarArvore(raiz->direita);
    free(raiz);
}

// árvore com os valores de energia
NoArvore* construirArvoreEnergia(NoAlimento* listaAlimentos) {// pega como base o campo energia para construir
    NoArvore* raiz = NULL;
    NoAlimento* atual = listaAlimentos;// ponteiro para a linked list
    
    while (atual != NULL) {// percorre toda a linked list de energia 
        raiz = inserirNaArvore(raiz, atual->alimento->energia, atual);// pra cada alimento pega a energia e insere na arvore 
        atual = atual->proximo;// vai por proximo nó
    }
    
    return raiz;// retorna raiz da arvore construida
}

// árvore com os valores da proteína
// mesmo lógica da de energia
NoArvore* construirArvoreProteina(NoAlimento* listaAlimentos) {
    NoArvore* raiz = NULL;
    NoAlimento* atual = listaAlimentos;
    
    while (atual != NULL) {
        raiz = inserirNaArvore(raiz, atual->alimento->proteina, atual);
        atual = atual->proximo;
    }
    
    return raiz;
}