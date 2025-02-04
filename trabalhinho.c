#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "lista.h"
//#include "pico/stdlib.h"

// funções para o funcionamento do jogo Desembaraçar
void desembaraca(Lista *nomes){
    // preparação para o início do jogo
    // escolhe aleatoriamente qual palavra será selecionada
    srand(time(NULL));
    int num = rand()%nomes->qtd;
    Palavra secreta;
    acessarValor(nomes, num, &secreta); //secreta será a palavra escolhida
    //printf("%s", secreta.nome); //teste
    
    // iniciando o jogo
    jogar(secreta);
    return;
}

//aleatoriza o jogo
void aleatorizar(char **aleatorio, int tam){
    //printf("%s%s%s\n", aleatorio[0], aleatorio[1], aleatorio[2]); //teste
    char *guardar;
    int dado;
    for(int i = 0; i < tam; i++){
        dado = rand()%tam;
        guardar = aleatorio[dado];
        aleatorio[dado] = aleatorio[i];
        aleatorio[i] = guardar;
    }
    //printf("%s%s%s", aleatorio[0], aleatorio[1], aleatorio[2]); //teste
}

void jogar(Palavra secreta){

    //definindo variáveis
    
    int tam = secreta.tam;
    char **aleatorio = secreta.soletrado;
    int opcao = 0;
    char mover;
    char *guardar;
    int troca = -1;
    int cont;
    
    //embaralhando as sílabas
    aleatorizar(aleatorio, tam);
    
    int jogar = 1;
    //loop da partida
    while (1){
        system("clear");
        //imprime o menu do jogo
        for (int i = 0; i < tam; i++) {
            if (i == opcao) {
                printf("->%s<-  ", aleatorio[i]);
            }
            else {
                printf("%s  ", aleatorio[i]);
            }
        }
        
        //movimentação da seta de escolha
        fflush(stdin);
        mover = getchar();
        
        if (mover == 'a' && opcao > 0){
            opcao--;
        }
        else if (mover == 'd' && opcao < tam - 1){
            opcao++;
        }
        
        //desembaralhando de acordo com o jogador
        if (mover == 'k' && troca == -1){
            troca = opcao;
        }
        else if (mover == 'k' && troca != -1){
            guardar = aleatorio[opcao];
            aleatorio[opcao] = aleatorio[troca];
            aleatorio[troca] = guardar;
            troca = -1;
        }
        
        //checando se chegou no resultado correto
        cont = 0;
        for (int j = 0; j < tam; j++){
            if (strcmp(aleatorio[j], secreta.soletrado[j]) == 0){
                cont++;
            }
        }
        
        if (cont == tam){
            printf("Parabéns! Você ganhou um ponto.\n\n");
            break;
        }
    }
    return;
        
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
    
    //loop do jogo
    while(1){
        desembaraca(nomes); // como no momento só existe esse jogo, ele vai rodar infinitamente
        pontos++;
    }
    return 0;
}
