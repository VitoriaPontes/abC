#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "lista.h"
#include "hardware/adc.h"
#include "include/ssd1306.h"
#include "hardware/i2c.h"
#define BUTTON1_PIN 5 // botão 1
#define BUTTON2_PIN 6 // botão 2
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;


// função para exibir o texto no display
void display_text_on_ssd1306(uint8_t *ssd, const char *line1, const char *line2, int buffer_length, struct render_area *frame_area) {
    ssd1306_clear(ssd); // Limpa o display antes de desenhar
    ssd1306_draw_string(ssd, 5, 0, line1);
    ssd1306_draw_string(ssd, 5, 8, line2);
    render_on_display(ssd, frame_area);
}

// embaralha as sílabas da palavra
void embaralhar(Palavra secreta){
    int guardar;
    int dado;
    for(int i = 0; i < secreta.tam; i++){
        dado = rand()%(secreta.tam);
        guardar = secreta.posicoes[dado];
        secreta.posicoes[dado] = secreta.posicoes[i];
        secreta.posicoes[i] = guardar;
    }
}

/*void exibir(Palavra secreta){
    //calculate_render_area_buffer_length(&frame_area);
    for (int i = 0; i < secreta.tam; i++) {
                if (i == opcao) {
                    printf("->%s<-  ", secreta.soletrado[secreta.posicoes[i] - 1]);
                }
                else {
                    printf("%s  ", secreta.soletrado[secreta.posicoes[i] - 1]);
                }
            }
            mudanca = 0;
}*/

// função principal da partida
void jogar(Palavra secreta, struct rendes_area *frame_area, int pontos){

    ssd1306_init();
    //definindo variáveis
    int opcao = 0;
    int guardar;
    int troca = -1;
    int cont;
    
    //embaralhando as sílabas
    embaralhar(secreta);
    
    int mudanca = 0;
    int jogar = 1;
    //loop da partida
    while (1){
        system("clear");
        //imprime o menu do jogo
    if (mudanca == 1) {
        //char text[100]; // Ajuste o tamanho do array conforme necessário
        //text[0] = '\0'; // Inicializa a string vazia
        /*for (int i = 0; i < secreta.tam; i++) {
            char buffer[20];
            if (i == opcao) {
                snprintf(buffer, sizeof(buffer), "->%s<-  ", secreta.soletrado[secreta.posicoes[i] - 1]);
            } else {
                snprintf(buffer, sizeof(buffer), "%s  ", secreta.soletrado[secreta.posicoes[i] - 1]);
            }
            strncat(text, buffer, sizeof(text) - strlen(text) - 1);
        }*/
       char *text1 = "";
        for(int i = 0; i < secreta.tam; i++){
            if (i == opcao){
                text1 = strcat(text1, "->");
                text1 = strcat(text1, secreta.soletrado[secreta.posicoes[i] - 1]);
                text1 = strcat(text1, "<-  ");
            }
            else{
                text1 = strcat(text1, secreta.soletrado[secreta.posicoes[i] - 1]);
                text1 = strcat(text1, "  ");
            }
        }
        char text2[20];
        snprintf(text2, sizeof(text2), "Pontos: %d", pontos);
        display_text_on_ssd1306(ssd, text1, text2, ssd1306_buffer_length, &frame_area);
        mudanca = 0;
    }
        
        //movimentação da seta de escolha
        adc_select_input(1);
        uint adc_x = adc_read(); // leitura do valor do analógico
        
        if (adc_x < 200 && opcao > 0){
            opcao--;
            mudanca = 1;
        }

        else if (adc_x > 3500 && opcao < secreta.tam - 1){
            opcao++;
            mudanca = 1;
        }

        else{
            if ((gpio_get(BUTTON1_PIN) == 0 || gpio_get(BUTTON2_PIN) == 0) && troca == -1){
                troca = opcao;
                mudanca = 1;
            }
            else if ((gpio_get(BUTTON1_PIN) == 0 || gpio_get(BUTTON2_PIN) == 0) && troca != -1){
                guardar = secreta.posicoes[opcao];
                secreta.posicoes[opcao] = secreta.posicoes[troca];
                secreta.posicoes[troca] = guardar;
                troca = -1;
                mudanca = 1;
            }
            while (gpio_get(BUTTON1_PIN) == 0 || gpio_get(BUTTON2_PIN) == 0){
                sleep_ms(20);
            }
        }

        sleep_ms(200);
        
        //checando se chegou no resultado correto
        cont = 0;
        for (int j = 0; j < secreta.tam; j++){
            if (secreta.posicoes[j] == (j+1)){
                cont++;
            }
        }
        
        if (cont == secreta.tam){
            printf("Parabéns! A palavra é %s. Você ganhou um ponto.\n\n", secreta.nome);
            sleep_ms(5000);
            break;
        }
    }
    return;
        
}

void desembaraca(Lista *nomes, struct render_area *frame_area, int pontos){
    // preparação para o início do jogo
    // escolhe aleatoriamente qual palavra será selecionada
    srand(time(NULL));
    int num = rand()%nomes->qtd;
    Palavra secreta;
    acessarValor(nomes, num, &secreta); //secreta será a palavra escolhida
    //printf("%s", secreta.nome); //teste
    
    // iniciando o jogo
    jogar(secreta, &frame_area, pontos);
    return;
}

// main
int main() {
    // inicialização do ADC
    stdio_init_all();
    adc_init();

    adc_gpio_init(26);
    adc_gpio_init(27);

    gpio_init(BUTTON1_PIN);
    gpio_set_dir(BUTTON1_PIN, GPIO_IN);
    gpio_pull_up(BUTTON1_PIN);

    gpio_init(BUTTON2_PIN);
    gpio_set_dir(BUTTON2_PIN, GPIO_IN);
    gpio_pull_up(BUTTON2_PIN);

    // inicializando o i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // inicializando o oled
    

    // configuração do display
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 1,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    //display_text_on_ssd1306(ssd, "oi", "oieee", ssd1306_buffer_length, &frame_area);


    //criação da lista
    Lista *nomes;
    nomes = criar(nomes);
    printf("Lista criada\n");
    //criação do elemento abacate
    char *temp1[] = {"a", "ba", "ca", "te"};
    int pos1[] = {1, 2, 3, 4};
    Palavra abacate = criarPalavra("abacate", temp1, pos1, 4);
    
    //criação do elemento banana
    char *temp2[] = {"te", "le", "fo", "ne", "ma"};
    int pos2[] = {1, 2, 3};
    Palavra telefonema = criarPalavra("telefonema", temp2, pos2, 5);
    
    //criação do elemento cachorro
    char *temp3[] = {"bi", "bli", "o", "te", "ca"};
    int pos3[] = {1, 2, 3};
    Palavra biblioteca = criarPalavra("biblioteca", temp3, pos3, 5);
    
    //criação do elemento paralelepipedo
    char *temp4[] = {"sen", "si", "bi", "li", "da", "de"};
    int pos4[] = {1, 2, 3, 4, 5, 6, 7};
    Palavra sensibilidade = criarPalavra("sensibilidade", temp4, pos4, 6);
    
    //criação da palavra matematica
    char *temp5[] = {"mo", "nu", "men", "to"};
    int pos5[] = {1, 2, 3, 4, 5};
    Palavra monumento = criarPalavra("monumento", temp5, pos5, 4);
    
    //inserção dos elementos na lista
    inserirInicio(nomes, monumento);
    inserirInicio(nomes, sensibilidade);
    inserirInicio(nomes, biblioteca);
    inserirInicio(nomes, telefonema);
    inserirInicio(nomes, abacate);
    
    //exibindo a lista
    //exibir(nomes); //teste

    int pontos = 0;

    //loop do jogo
    while(1){
        printf("Pontuação: %d\n", pontos);
        desembaraca(nomes, &frame_area, pontos); // como no momento só existe esse jogo, ele vai rodar infinitamente
        pontos++;
    }
}
