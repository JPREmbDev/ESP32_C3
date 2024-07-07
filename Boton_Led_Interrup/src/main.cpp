
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"


extern "C" void app_main();


// Definiciones para el LED y el Pulsador
#define LED_PIN GPIO_NUM_2
#define BUTTON_PIN GPIO_NUM_0

// Variables globales
static const char *TAG = "LED_BUTTON_APP";
volatile bool led_on = false;
volatile int button_press_count = 0;

// ISR del pulsador
static void IRAM_ATTR button_isr_handler(void* arg) {
    // Incrementar el contador de pulsaciones de manera explícita
    button_press_count = button_press_count + 1;;
    
    // Cambiar el estado del LED
    led_on = !led_on;
    gpio_set_level(LED_PIN, led_on);

    // Registro en el log
    if (led_on) {
        ESP_EARLY_LOGI(TAG, "LED Encendido (Pulsado #%d)", button_press_count);
    } else {
        ESP_EARLY_LOGI(TAG, "LED Apagado (Pulsado #%d)", button_press_count);
    }
}

// Función principal
void app_main(void) {
    // Configurar el LED
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;    // Sin interrupciones para el LED
    io_conf.mode = GPIO_MODE_OUTPUT;          // Modo de salida
    io_conf.pin_bit_mask = (1ULL << LED_PIN); // Pines seleccionados
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
    
    // Configurar el pulsador
    io_conf.intr_type = GPIO_INTR_NEGEDGE;    // Interrupción en el borde de bajada
    io_conf.mode = GPIO_MODE_INPUT;           // Modo de entrada
    io_conf.pin_bit_mask = (1ULL << BUTTON_PIN); // Pines seleccionados
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;  // Activar pull-up
    gpio_config(&io_conf);

    // Instalar el servicio de ISR
    gpio_install_isr_service(0);
    // Añadir ISR para el pulsador
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    ESP_LOGI(TAG, "Configuración completa. LED_PIN: %d, BUTTON_PIN: %d", LED_PIN, BUTTON_PIN);

    // Bucle principal vacío ya que todo se maneja por la ISR
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000)); // Evita que el CPU se quede ocioso
    }
}