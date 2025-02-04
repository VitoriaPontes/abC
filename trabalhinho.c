#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "lista.h"
//#include "pico/stdlib.h"

// funções para o funcionamento do jogo Desembaraçar
void desembaraca(Lista *nomes){
    // escolhe aleatoriamente qual palavra será selecionada
    srand(time(NULL));
    int num = rand()%nomes->qtd;
    struct palavra secreta;
    acessarValor(nomes, num, &secreta); //secreta será a palavra escolhida
    //printf("%s", secreta.nome); //teste
    aleatorizar(secreta.soletrado, secreta.tam);
    return;
}

//aleatoriza o jogo
void aleatorizar(char **aleatorio, int tam){
    //printf("%s%s%s\n", aleatorio[0], aleatorio[1], aleatorio[2]); //teste
    char *guardar = "";
    int dado;
    for(int i = 0; i < tam; i++){
        dado = rand()%tam;
        guardar = aleatorio[dado];
        aleatorio[dado] = aleatorio[i];
        aleatorio[i] = guardar;
    }
    //printf("%s%s%s", aleatorio[0], aleatorio[1], aleatorio[2]); //teste
}

void exibirMenu(char **aleatorio, int tam){
    for (int i = 0; i < tam; i++){
        printf("%s", aleatorio[i]);
    }
    
    printf("\n\n");
    
    
}

// main
int main() {
    //criação da lista
    Lista *nomes;
    nomes = criar(nomes);
    
    //criação do elemento abacate
    char *temp1[] = {"a", "ba", "ca", "te"};
    Palavra abacate = criarPalavra("abacate", temp1, 4);
    
    //criação do elemento banana
    char *temp2[] = {"ba", "na", "na"};
    Palavra banana = criarPalavra("banana", temp2, 3);
    
    //criação do elemento cachorro
    char *temp3[] = {"ca", "chor", "ro"};
    Palavra cachorro = criarPalavra("cachorro", temp3, 3);
    
    //criação do elemento paralelepipedo
    char *temp4[] = {"pa", "ra", "le", "le", "pi", "pe", "do"};
    Palavra paralelepipedo = criarPalavra("paralelepipedo", temp4, 7);
    
    //criação da palavra matematica
    char *temp5[] = {"ma", "te", "ma", "ti", "ca"};
    Palavra matematica = criarPalavra("matematica", temp5, 5);
    
    //inserção dos elementos na lista
    inserirInicio(nomes, paralelepipedo);
    inserirInicio(nomes, matematica);
    inserirInicio(nomes, cachorro);
    inserirInicio(nomes, banana);
    inserirInicio(nomes, abacate);
    
    //exibindo a lista
    //exibir(nomes); //teste

    int pontos = 0;
    desembaraca(nomes);
    return 0;
}