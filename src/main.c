#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define BOTAO_GPIO_NODE DT_NODELABEL(gpioc) 
#define BOTAO_PIN       13                 

// pinos dos leds
static const struct gpio_dt_spec segmentos[] = {
    {DEVICE_DT_GET(DT_NODELABEL(gpioa)), 0, GPIO_OUTPUT},  // PA0 A
    {DEVICE_DT_GET(DT_NODELABEL(gpioa)), 1, GPIO_OUTPUT},  // PA1 B
    {DEVICE_DT_GET(DT_NODELABEL(gpioa)), 4, GPIO_OUTPUT},  // PA4 C
    {DEVICE_DT_GET(DT_NODELABEL(gpiob)), 0, GPIO_OUTPUT},  // PB0 D
    {DEVICE_DT_GET(DT_NODELABEL(gpioc)), 1, GPIO_OUTPUT},  // PC1 E
    {DEVICE_DT_GET(DT_NODELABEL(gpioc)), 0, GPIO_OUTPUT},  // PC0 F
    {DEVICE_DT_GET(DT_NODELABEL(gpiob)), 8, GPIO_OUTPUT},  // PB8 G
};

// pino do botão
static const struct gpio_dt_spec botao = {
    DEVICE_DT_GET(BOTAO_GPIO_NODE), 
    BOTAO_PIN,                      
    GPIO_INPUT,                    
};

// matriz de binários de cada número hexadecimal
static const uint8_t valores_segmentos[16][7] = {
    {1, 1, 1, 1, 1, 1, 0},  // 0
    {0, 1, 1, 0, 0, 0, 0},  // 1
    {1, 1, 0, 1, 1, 0, 1},  // 2
    {1, 1, 1, 1, 0, 0, 1},  // 3
    {0, 1, 1, 0, 0, 1, 1},  // 4
    {1, 0, 1, 1, 0, 1, 1},  // 5
    {1, 0, 1, 1, 1, 1, 1},  // 6
    {1, 1, 1, 0, 0, 0, 0},  // 7
    {1, 1, 1, 1, 1, 1, 1},  // 8
    {1, 1, 1, 1, 0, 1, 1},  // 9
    {1, 1, 1, 0, 1, 1, 1},  // A
    {0, 0, 1, 1, 1, 1, 1},  // B
    {1, 0, 0, 1, 1, 1, 0},  // C
    {0, 1, 1, 1, 1, 0, 1},  // D
    {1, 0, 0, 1, 1, 1, 1},  // E
    {1, 0, 0, 0, 1, 1, 1}   // F
};

// exibir um número no led
void exibir_numero(int numero) {
    for (int i = 0; i < 7; i++) {
        gpio_pin_set(segmentos[i].port, segmentos[i].pin, valores_segmentos[numero][i]);
    }
}

int main(void) {
    int retorno, contador = 0, botao_estado_anterior = 1;

    for (int i = 0; i < 7; i++) {
        retorno = gpio_pin_configure(segmentos[i].port, segmentos[i].pin, GPIO_OUTPUT_ACTIVE);
    }

    retorno = gpio_pin_configure(botao.port, botao.pin, GPIO_INPUT);

    while (1) {
        int botao_estado_atual = gpio_pin_get(botao.port, botao.pin);

        // se o botão for pressionado e depois solto
        if (botao_estado_anterior == 0 && botao_estado_atual == 1) {
            contador = (contador + 1) % 16;
        }

        botao_estado_anterior = botao_estado_atual;

        exibir_numero(contador);  // Exibe o valor atual do contador
    }

    return 0;
}