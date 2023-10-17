# include <freertos/FreeRTOS.h>
# include <freertos/task.h>

// LED invervalos de piscada
static const int rate_1 = 800;  // ms
static const int rate_2 = 800;  // ms

// Pins
static const int led_pin_1 = 2;
static const int led_pin_2 = 22;

static SemaphoreHandle_t mutex; //semáforo

// t1: piscar led com rate_1
void toggleLED_1(void *parameter) {
  while(1) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    digitalWrite(led_pin_1, HIGH);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    digitalWrite(led_pin_1, LOW);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    xSemaphoreGive(mutex);
  }
  
}

// t2: piscar led com rate_2
void toggleLED_2(void *parameter) {
  while(1) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    digitalWrite(led_pin_2, HIGH);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
    digitalWrite(led_pin_2, LOW);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
    xSemaphoreGive(mutex);
  }
}

void setup() {

  // Configura pin
  pinMode(led_pin_1, OUTPUT);
  pinMode(led_pin_2, OUTPUT);

  mutex = xSemaphoreCreateMutex();
  
  // Tarefas em loop infinito
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED_1,  // função a ser chamada
              "Toggle 1",   // nome da task
              1024,         // tamanho em bytes
              NULL,         // parâmetros a passar a função
              1,            // Prioridade (0 a configMAX_PRIORITIES - 1)
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
  vTaskDelay(1000 / portTICK_PERIOD_MS); // coloca task em blocked
}