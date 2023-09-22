# include <freertos/FreeRTOS.h>
# include <freertos/task.h>

/**
 Adaptado de Shawn Hymel, Dec. 3, 2020 - Piscar LED com intervalos diferentes em tarefas diferentes
 * License: 0BSD
*/

// LED invervalos de piscada
static const int rate_1 = 500;  // ms
static const int rate_2 = 300;  // ms

// Pins
static const int led_pin = 2;

// t1: piscar led com rate_1
void toggleLED_1(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
  }
}

// t2: piscar led com rate_2
void toggleLED_2(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
  }
}

void setup() {

  // Configura pin
  pinMode(led_pin, OUTPUT);

  // Tarefas em loop infinito
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED_1,  // função a ser chamada
              "Toggle 1",   // nome da task
              1024,         // tamanho em bytes
              NULL,         // parâmetros a passar a função
              0,            // Prioridade (0 a configMAX_PRIORITIES - 1)
              NULL,         // handle da task
              1);     // núcleo: 0 (pro) 1 (app)

  // Tarefas em loop infinito
  xTaskCreatePinnedToCore(  
              toggleLED_2,  
              "Toggle 2",   
              1024,         
              NULL,         
              1,            
              NULL,         
              1);     

  // If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in
  // main after setting up your tasks.
}

void loop() {
  // Diferente da abordagem super loop, com RTOS loop() não precisa fazer nada. Mas pode-se colocar algum vtaskdelay para que tarefas de menor prioridade acessem
  // setup() e loop() rodam suas tarefas com prioridade 1 no núcleo 1 no esp32
}