# include <freertos/FreeRTOS.h>
# include <freertos/task.h>
#include <freertos/queue.h>

//adaptado de https://embarcados.com.br/rtos-queue-sincronizacao-e-comunicacao/
static QueueHandle_t buffer; //Objeto da fila

// t1: enviar valores para a fila
void t1(void *parameter) {
  // variavel local à task
  uint32_t itemS = 0; //uint32_t é um alias para unsigned int (typedef) - 32 bits = 4 bytes
  while(1) {
    if (itemS < 10) {
        xQueueSend(buffer, &itemS, pdMS_TO_TICKS(0));
        //Serial.println(itemS);
        itemS++;  
    } else {
        vTaskDelay(pdMS_TO_TICKS(5000)); //bloqueia t1 por 5s, troca contexto para t2 e neste momento t2 não recebe...
        itemS = 0; // fila cheia é então resetada
    }
    vTaskDelay(pdMS_TO_TICKS(500)); //bloqueia 0.5s entre chamadas
  }
}

// t2: receber (ler) valores da fila
void t2(void *parameter) {
  // variavel local à task
  uint32_t itemR = 0; //uint32_t é um alias para unsigned int (typedef) - 32 bits = 4 bytes
  while(1) {
    if (xQueueReceive(buffer, &itemR, pdMS_TO_TICKS(1000)) == true) {
        //Serial.printf("entrei no receive\n");
        //se recebeu dentro de 1s
        Serial.printf("Item recebido: %u", itemR);
    } else Serial.printf("Item nao recebido (timeout)\n");
  }
}

void setup() {

  Serial.begin(115200);
  buffer = xQueueCreate(10, sizeof(uint32_t)); //Cria a queue *buffer* com 10 slots de 4 Bytes

  // Tarefas em loop infinito
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              t1,  // função a ser chamada
              "T1",   // nome da task
              4096,         // tamanho em bytes
              NULL,         // parâmetros a passar a função
              1,            // Prioridade (0 a configMAX_PRIORITIES - 1)
              NULL,         // handle da task
              1);     // núcleo: 0 (pro) 1 (app)
  
  // Tarefas em loop infinito
  xTaskCreatePinnedToCore(  
              t2,  
              "T2",   
              4096,         
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
  //vTaskDelay(1000 / portTICK_PERIOD_MS); // coloca task em blocked
}
