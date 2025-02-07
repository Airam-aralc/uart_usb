#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/i2c.h"
//#include "hardware/clocks.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
//#include "uart_usb.pio.h" 

#define led_pin_g 11  // Led verde do RGB
#define led_pin_b 12  // Led azul do RGB
#define botao_A 5     // Botão A da BitDogLab
#define botao_B 6     // Botão B da BitDogLab
#define WS2812_PIN 7  // Matriz de led
#define NUM_PIXELS 25
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

void exibir_numero_matriz(int numero);

bool numeros[10][NUM_PIXELS] = 
{   //NÚMERO 0
    { 0, 1, 1, 1, 0,  
      0, 1, 0, 1, 0,  
      0, 1, 0, 1, 0,  
      0, 1, 0, 1, 0,  
      0, 1, 1, 1, 0 },  
    //NÚMERO 1
    { 0, 1, 0, 0, 0,  
      0, 0, 0, 1, 0,  
      0, 1, 0, 0, 0,  
      0, 0, 0, 1, 0,  
      0, 1, 0, 0, 0 },
    //NÚMERO 2
    { 0, 1, 1, 1, 0,  
      0, 1, 0, 0, 0,  
      0, 1, 1, 1, 0,  
      0, 0, 0, 1, 0,  
      0, 1, 1, 1, 0 }, 
    //NÚMERO 3
    { 0, 1, 1, 1, 0,  
      0, 0, 0, 1, 0,  
      0, 1, 1, 1, 0,  
      0, 0, 0, 1, 0,  
      0, 1, 1, 1, 0 },
    //NÚMERO 4
    { 0, 1, 0, 0, 0,  
      0, 0, 0, 1, 0,  
      0, 1, 1, 1, 0,  
      0, 1, 0, 1, 0,  
      0, 1, 0, 1, 0 }, 
    //NÚMERO 5
    { 0, 1, 1, 1, 0,  
      0, 0, 0, 1, 0,  
      0, 1, 1, 1, 0,  
      0, 1, 0, 0, 0,  
      0, 1, 1, 1, 0 },
    //NÚMERO 6
    { 0, 1, 1, 1, 0,  
      0, 1, 0, 1, 0,  
      0, 1, 1, 1, 0,  
      0, 1, 0, 0, 0,  
      0, 1, 1, 1, 0 },
    //NÚMERO 7
    { 0, 1, 0, 0, 0,  
      0, 0, 0, 1, 0,
      0, 1, 0, 0, 0,  
      0, 0, 0, 1, 0,  
      0, 1, 1, 1, 0 }, 
    //NÚMERO 8
    { 0, 1, 1, 1, 0,
      0, 1, 0, 1, 0,
      0, 1, 1, 1, 0,  
      0, 1, 0, 1, 0,  
      0, 1, 1, 1, 0 }, 
    //NÚMERO 9
    { 0, 1, 1, 1, 0,  
      0, 0, 0, 1, 0,  
      0, 1, 1, 1, 0,  
      0, 1, 0, 1, 0,  
      0, 1, 1, 1, 0 }
};

static volatile uint32_t last_time = 0;  // Para debouncing, armazena o tempo do último evento
static void gpio_irq_handler(uint gpio, uint32_t events); //Prototipação da função de interrupção

ssd1306_t ssd; // variável global do display

int main()
{
    stdio_init_all(); // Inicializa comunicação USB CDC para monitor serial

    // Configura os pinos dos LEDs como saída
    gpio_init(led_pin_g);
    gpio_set_dir(led_pin_g, GPIO_OUT);
    gpio_put(led_pin_g, 0); // Inicialmente desligado
    gpio_init(led_pin_b);
    gpio_set_dir(led_pin_b, GPIO_OUT);
    gpio_put(led_pin_b, 0); // Inicialmente desligado


    gpio_init(botao_A);             //inicializa o botão A
    gpio_set_dir(botao_A, GPIO_IN); //configura como entrada
    gpio_pull_up(botao_A);          //pull up ativado
    gpio_init(botao_B);             //inicializa o botão B
    gpio_set_dir(botao_B, GPIO_IN);
    gpio_pull_up(botao_B);

    // Configuração do I2C
    i2c_init(I2C_PORT, 400 * 1000); // Inicializa o a 400Khz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA); // Habilita o Pull-up para SDA
    gpio_pull_up(I2C_SCL); // Habilita o Pull-up para SCL

    //Configura as interrupções para detectar quando os botões forem pressionados
    gpio_set_irq_enabled_with_callback(botao_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(botao_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    bool cor = true;

    printf("RP2040 inicializado. Envie alguma letra de a-z ou algum numero de 0-9.\n");

    while (true)
    {

        if (stdio_usb_connected()) {
            char c;
             if (scanf("%c", &c) == 1){
           // if (scanf("%c", &c) == 1 && c >= '0' && c <= '9'){
               // int numero = c - '0'; // Converter de char para int

                printf("Recebido: '%c'\n", c);
                ssd1306_fill(&ssd, false);
                char texto[2] = {c, '\0'};

                ssd1306_draw_string(&ssd, texto, 40, 25);
                ssd1306_send_data(&ssd);

                // Exibe na matriz de LEDs
                // exibir_numero_matriz(numero);
            }
        }


/*        if (stdio_usb_connected())
        { // Certifica-se de que o USB está conectado
            char c;
            if (scanf("%c", &c) == 1)
            { // Lê caractere da entrada padrão
                printf("Recebido: '%c'\n", c);

                switch (c)
                {
                case 'r': // testa com r e R
                    ssd1306_fill(&ssd, false); //Limpa o display
                    ssd1306_draw_string(&ssd, "R", 25, 25);
                    ssd1306_send_data(&ssd); // Atualiza o display
                    break;
                default:
                    printf("Comando inválido: '%c'\n", c);
                }
            }
        }*/

        sleep_ms(40);
    }

    return 0;
}


//INTERRUPÇÃO
void gpio_irq_handler(uint gpio, uint32_t events) 
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_time > 200000) // 200 ms de debouncing
    { 
        last_time = current_time;
        
        ssd1306_fill(&ssd, false); // Limpa o display

        if (gpio == botao_A) { // Muda o estado do led verde, com uma mensagem no display e no Serial Monitor
           gpio_put(led_pin_g, !gpio_get(led_pin_g));
           
           if(!gpio_get(led_pin_g)){
                printf("LED verde foi desligado!\n");
                ssd1306_draw_string(&ssd, "LED VERDE OFF", 20, 25);
           } else{
                printf("LED verde foi ligado!\n");
                ssd1306_draw_string(&ssd, "LED VERDE ON", 20, 25);
           }
        } 
        else if (gpio == botao_B) { // Muda o estado do led verde, com uma mensagem no display e no Serial Monitor
            gpio_put(led_pin_b, !gpio_get(led_pin_b));
            
            if(!gpio_get(led_pin_b)){
                printf("LED azul foi desligado!\n");
                ssd1306_draw_string(&ssd, "LED AZUL OFF", 20, 25);
           } else{
                printf("LED azul foi ligado!\n");
                ssd1306_draw_string(&ssd, "LED AZUL ON", 20, 25);
           }
        }

        ssd1306_send_data(&ssd); // Atualiza o display
    }
}

/*
//função para a matriz de leds
void exibir_numero_matriz(int numero) 
    {
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (numeros[numero][i]) {
            ws2812_put_pixel(0x00FF00); // Verde para representar o número
        } else {
            ws2812_put_pixel(0x000000); // Preto para apagar o LED
        }
       }
    }
    */