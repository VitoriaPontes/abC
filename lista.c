#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

//Funções relacionadas ao TAD necessário para o melhor funcionamento das listas, podendo assumir tamanho diversos.
//Mas para exemplo do funcionamento do jogo mostrarei com apenas 5 palavras.

Lista *criar() {
    Lista *novo;
    novo = (Lista*)malloc(sizeof(Lista));
    if (novo == NULL) {
        return NULL;
    }
    novo->inicio = NULL;
    novo->qtd = 0;
    return novo;
}

int inserirInicio (Lista *lista, Palavra nova) {
    if (lista == NULL) {
        return 0;
    }

    Elemento *novo;
    novo = (Elemento*)malloc(sizeof(Elemento));
    if (novo == NULL) return 0;
    novo->info = nova;
    novo->prox = lista->inicio;
    lista->inicio = novo;
    lista->qtd++;
    return 1;
}

int acessarValor(Lista *lista, int valor, Palavra *x) {
    if (vazia(lista)) return 0;
    Elemento *aux;
    aux = lista->inicio;
    int cont = 0;
    if (valor > lista->qtd) return 0;
    while (aux != NULL && cont < valor) {
        aux = aux->prox;
        cont++;
    }
    if (aux == NULL) return 0;
    *x = aux->info;
    return 1;
}

void destruir (Lista *lista) {
    if (lista != NULL) {
        Elemento *aux;
        while (lista->inicio != NULL) {
            aux = lista->inicio;
            lista->inicio = (lista->inicio)->prox;
            free(aux);
        }
        free(lista);
    }
}

int vazia (Lista *lista) {
    if (lista == NULL || lista->inicio == NULL)
        return 1;
    else
        return 0;
}

int exibir(Lista *lista) {
    if (vazia(lista)) return 0;
    Elemento *aux;
    aux = lista->inicio;
    while (aux != NULL) {
        printf("%s ", aux->info.nome);
        aux = aux->prox;
    }
    printf("\n");
    return 1;
}

Palavra criarPalavra(char *nome, char *soletrado[], int tam){
    struct palavra nova;
    nova.nome = nome;
    nova.soletrado = soletrado;
    nova.tam = tam;
    return nova;
}

