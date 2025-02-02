#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_RED 2
#define LED_YELLOW 3
#define LED_GREEN 4
#define BUTTON 5

bool repeating_timer_callback(struct repeating_timer *t);
bool one_shot_callback(struct repeating_timer *t);
void setup();
bool debounce_button();

volatile bool allow_button_press = true;
volatile uint32_t last_button_press = 0;

// Callback do temporizador periódico (Semáforo)
bool repeating_timer_callback(struct repeating_timer *t) {
    static int state = 0;
    gpio_put(LED_RED, state == 0);
    gpio_put(LED_YELLOW, state == 1);
    gpio_put(LED_GREEN, state == 2);
    state = (state + 1) % 3;
    printf("Semáforo atualizado.\n");
    return true;
}

// Callback do temporizador de um disparo (One Shot)
bool one_shot_callback(struct repeating_timer *t) {
    static int state = 0;
    switch (state) {
        case 0:
            gpio_put(LED_RED, 1);
            gpio_put(LED_YELLOW, 1);
            gpio_put(LED_GREEN, 1);
            state++;
            break;
        case 1:
            gpio_put(LED_GREEN, 0);
            state++;
            break;
        case 2:
            gpio_put(LED_YELLOW, 0);
            state++;
            break;
        case 3:
            gpio_put(LED_RED, 0);
            allow_button_press = true;
            state = 0;
            return false;
    }
    add_alarm_in_ms(3000, (alarm_callback_t)one_shot_callback, NULL, true);
    return true;
}

// Função para debounce do botão
bool debounce_button() {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_button_press > 200) { // 200ms debounce
        last_button_press = current_time;
        return true;
    }
    return false;
}

// Callback do botão
void button_callback(uint gpio, uint32_t events) {
    if (allow_button_press && debounce_button()) {
        allow_button_press = false;
        add_alarm_in_ms(0, (alarm_callback_t)one_shot_callback, NULL, true);
    }
}

void setup() {
    stdio_init_all();
    gpio_init(LED_RED);
    gpio_init(LED_YELLOW);
    gpio_init(LED_GREEN);
    gpio_init(BUTTON);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);
    gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_callback);
}

int main() {
    setup();
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);
    while (1) {
        printf("Sistema rodando...\n");
        sleep_ms(1000);
    }
}