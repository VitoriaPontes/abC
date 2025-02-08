#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "lista.h"
#include "hardware/adc.h"
#include "include/ssd1306.h"
#include "hardware/i2c.h"
#include "pico/rand.h"    
#define BUTTON1_PIN 5 // botão 1
#define BUTTON2_PIN 6 // botão 2
#define LED_BLUE 12 // led azul
#define LED_GREEN 11 // led azul

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;


// função para exibir o texto no display
void display_text_on_ssd1306(uint8_t *ssd, const char *line1, const char *line2, int buffer_length, struct render_area *frame_area) {
    int x = 0;
    char str[2] = {0}; // Temporary string to hold a single character
    for (uint i = 0; i < strlen(line1); i++)
    {
        str[0] = line1[i];
        ssd1306_draw_string(ssd, x, 5, str);
        str[0] = line2[i];
        ssd1306_draw_string(ssd, x, 20, str);
        x += 8;
    }
    render_on_display(ssd, frame_area);
}

// embaralha as sílabas da palavra
void embaralhar(Palavra secreta){
    int guardar;
    uint8_t  dado;
    for(int i = 0; i < secreta.tam; i++){
        dado = (uint8_t)get_rand_32()%(secreta.tam);
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
void jogar(Palavra secreta, int pontos, uint8_t* ssd, struct render_area frame_area){
    //inicializando o display
    ssd1306_init();

    //definindo variáveis utilizadas
    int opcao = 0;
    int guardar;
    int troca = -1;
    int cont;
    int mudanca = 0;
    int jogar = 1;
    
    
    //embaralhando as sílabas
    embaralhar(secreta);
    

    //loop da partida
    while (1){

    //imprime o menu do jogo
    do {
      char text1[20] = "";
        for(int i = 0; i < secreta.tam; i++){
            if (i == opcao){
                strcat(text1, "y");
                strcat(text1, secreta.soletrado[secreta.posicoes[i] - 1]);
                strcat(text1, "y");
            }
            else{
                strcat(text1, secreta.soletrado[secreta.posicoes[i] - 1]);
                strcat(text1, " ");
            }
        }
        char text2[20];
        snprintf(text2, sizeof(text2), "Pontos %d", pontos);
        printf("%s\n", text1);
        printf("%s\n", text2);
        display_text_on_ssd1306(ssd, text1, text2, ssd1306_buffer_length, &frame_area);
        mudanca = 0;
    } while(mudanca == 1);
        
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
            if (troca != -1){
                gpio_put(LED_BLUE, true);
            } else
                gpio_put(LED_BLUE, false);
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

            break;
        }
    }
    return;
        
}

void desembaraca(Lista *nomes, int pontos, uint8_t* ssd, struct render_area frame_area){
    // preparação para o início do jogo
    // escolhe aleatoriamente qual palavra será selecionada
    uint8_t num = (uint8_t)get_rand_32()%nomes->qtd;
    //int num = (rand() % 10);
    
    Palavra secreta;
    acessarValor(nomes, num, &secreta); //secreta será a palavra escolhida
    //printf("%s", secreta.nome); //teste
    
    // iniciando o jogo
    jogar(secreta, pontos, ssd, frame_area);
    return;
}

// main
int main() {

    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

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
    int pos2[] = {1, 2, 3, 4, 5};
    Palavra telefonema = criarPalavra("telefonema", temp2, pos2, 5);
    
    //criação do elemento cachorro
    char *temp3[] = {"bi", "bli", "o", "te", "ca"};
    int pos3[] = {1, 2, 3, 4, 5};
    Palavra biblioteca = criarPalavra("biblioteca", temp3, pos3, 5);
    
    //criação do elemento paralelepipedo
    char *temp4[] = {"es", "pi", "na", "fre"};
    int pos4[] = {1, 2, 3, 4, 5, 6, 7};
    Palavra espinafre = criarPalavra("espinafre", temp4, pos4, 6);
    
    //criação da palavra matematica
    char *temp5[] = {"mo", "nu", "men", "to"};
    int pos5[] = {1, 2, 3, 4};
    Palavra monumento = criarPalavra("monumento", temp5, pos5, 4);
    
    //inserção dos elementos na lista
    inserirInicio(nomes, monumento);
    inserirInicio(nomes, espinafre);
    inserirInicio(nomes, biblioteca);
    inserirInicio(nomes, telefonema);
    inserirInicio(nomes, abacate);
    
    //exibindo a lista
    //exibir(nomes); //teste

    int pontos = 0;

    //loop do jogo
    while(1){
            uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

        printf("Pontuação: %d\n", pontos);
        desembaraca(nomes, pontos, ssd, frame_area); // como no momento só existe esse jogo, ele vai rodar infinitamente
        // Liga o LED
        gpio_put(LED_GREEN, true);
        sleep_ms(1500);  // Espera 250ms

        // Desliga o LED
        gpio_put(LED_GREEN, false);
        sleep_ms(150);  // Espera 250ms
    
    pontos++;
    }
}
