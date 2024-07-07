
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"


extern "C" void app_main();

// Definir un tag para los logs
static const char *TAG = "my_app";

// Función que imprime diferentes mensajes
void print_messages(void *pvParameter) {
    int counter = 0;
    while (1) {
        ESP_LOGI(TAG, "Mensaje Informativo #%d: Este es un mensaje informativo.", counter);
        ESP_LOGI(TAG, "Mensaje Informativo #%d: Solo para probar como funciona.", counter);
        ESP_LOGI("INFO", "Mensaje Informativo #%d: Este es un mensaje informativo.", counter);
        //------------- NO ME HAN MOSTRADO MENSAJE EN EL LOG -------------------------------------//
        // ESP_LOGW(TAG, "Mensaje de Advertencia #%d: Este es un mensaje de advertencia.", counter);
        // ESP_LOGE(TAG, "Mensaje de Error #%d: Este es un mensaje de error.", counter);

        counter++;
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Espera 2 segundos entre cada impresión
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Aplicación Iniciada");

    // Creamos un hilo FreeRTOS para imprimir los mensajes
    xTaskCreate(&print_messages, "print_messages_task", 2048, NULL, 5, NULL);
}