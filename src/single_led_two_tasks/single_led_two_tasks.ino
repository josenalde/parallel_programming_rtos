#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h" //apenas para ESP-IDF
#include "esp_err.h" //apenas para ESP-IDF

/**
 Adaptado de Shawn Hymel, Dec. 3, 2020 - Piscar LED com intervalos diferentes em tarefas diferentes
 * License: 0BSD
*/

// LED invervalos de piscada
static const int rate_1 = 500;  // ms
static const int rate_2 = 300;  // ms
static TaskHandle_t tLed1 = NULL; //handle para task
static TaskHandle_t tLed2 = NULL; //handle para task
// Pins
static const int led_pin = 26;
#define RUNS 5
volatile int i = 0;

//#define TASK_1_STACK_SIZE       (configMINIMAL_STACK_SIZE * 4)
//#define TASK_1_PRIORITY         (tskIDLE_PRIORITY  + 1)

// t1: piscar led com rate_1
void toggleLED_1(void *parameter) {
  Serial.print("Primeira chamada de ");
  Serial.println(__func__);
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    Serial.print(pcTaskGetTaskName(NULL));
    Serial.print(" rodando no núcleo ");
    Serial.print(xPortGetCoreID());
    Serial.print(" com prioridade: ");
    Serial.println(uxTaskPriorityGet(NULL));
    //ESP_LOGW("main", "%s tick :  %d", __func__, xTaskGetTickCount()); //esp-idf
    i++;
    if (i == 2*RUNS) vTaskResume(tLed2);
  }
}

// t2: piscar led com rate_2
void toggleLED_2(void *parameter) {
  Serial.print("Primeira chamada de ");
  Serial.println(pcTaskGetTaskName(NULL));
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
    Serial.print(pcTaskGetTaskName(NULL));
    Serial.print(" rodando no núcleo ");
    Serial.print(xPortGetCoreID());
    Serial.print(" com prioridade: ");
    Serial.println(uxTaskPriorityGet(NULL));
    // Se quiser executar uma só vez ou a depender de uma condição/counter global etc.
    if (i == RUNS) vTaskSuspend(NULL);
    i++; //global
    //ESP_LOGI("main", "%s tick :  %d", __func__, xTaskGetTickCount()); //esp-idf

  }
}

void setup() {

  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(1000));
  // Configura pin
  pinMode(led_pin, OUTPUT);
  Serial.print(pcTaskGetTaskName(NULL)); //quem chamou foi o loop()
  Serial.print(" ... Estou no SETUP rodando no nucleo ");
  Serial.print(xPortGetCoreID());
  Serial.print(" com prioridade: ");
  Serial.println(uxTaskPriorityGet(NULL));
  /*
    TESTES
    a) núcleos diferentes
    b) mesmo núcleo com mesma prioridade

    
  */
  
  // Tarefas em loop infinito
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED_1,  // função a ser chamada
              "Toggle 1",   // nome da task
              1024,         // tamanho em bytes
              NULL,         // parâmetros a passar a função
              1,            // Prioridade (0 a configMAX_PRIORITIES - 1)
              &tLed1,         // handle da task
              0);     // núcleo: 0 (pro) 1 (app)

  // Tarefas em loop infinito
  xTaskCreatePinnedToCore(  
              toggleLED_2,  
              "Toggle 2",   
              1024,         
              NULL,         
              1,            
              &tLed2,         
              0);     

  vTaskDelete(NULL); // Deleta loop(), ou seja, quem chamou setup() (TESTE COMENTAR)
  // Adicionar um delay no setup() se task rodam no mesmo núcleo garante que as tarefas serão executadas
}

void loop() {
  // Diferente da abordagem super loop, com RTOS loop() não precisa fazer nada. Mas pode-se colocar algum vtaskdelay para que tarefas de menor prioridade acessem
  // setup() e loop() rodam suas tarefas com prioridade 1 no núcleo 1 no esp32
  Serial.print("Loop rodando com prioridade: ");
  Serial.print(uxTaskPriorityGet(NULL));
  Serial.print(" no núcleo : ");
  Serial.println(xPortGetCoreID());
  //Serial.println(pcTaskGetTaskName(NULL)); //return loopTask

}
