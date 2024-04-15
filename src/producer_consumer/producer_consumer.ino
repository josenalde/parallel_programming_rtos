#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

xQueueHandle commQueue;
static void Producer(void *pvParameters);
static void Consumer(void *pvParameters);
 
#define QUEUE_ITEM_SIZE 1
#define QUEUE_LENGTH    10

//TaskHandle_t producerHandle = NULL; // um handle da task pode ser usado depois dentro de um vTaskDelete(producerHandle) por exemplo...
 
void setup() {
  Serial.begin(115200);
  
  commQueue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE); //1 byte por vez
  
  xTaskCreatePinnedToCore(Producer,               /* Função que executa a tarefa */
              "Producer",             /* String que define o nome da tarefa */
              1024,                      /* tamanho da pilha da tarefa (bytes) */
              NULL,                     /* parametros para a tarefa */
              tskIDLE_PRIORITY + 1,     /* prioridade da tarefa (1) */ 
              NULL,
              1);                   /* handle da tarefa, por exemplo producerHandle */
  
  xTaskCreatePinnedToCore(Consumer,               /* Função que executa a tarefa */
              "Consumer", /* String que define o nome da tarefa */
              1024,                      /* tamanho da pilha da tarefa (bytes) */
              NULL,                     /* parametros para a tarefa */
              tskIDLE_PRIORITY + 1,     /* prioridade da tarefa */
              NULL,
              0);                   /* handle da tarefa */
}


static void Producer(void *pvParameters) {
    unsigned char i=0; //1 byte (0 a 255)
    while(1) {
        i++;
        xQueueSend(commQueue, &i, pdMS_TO_TICKS(0));
        Serial.print("Enviei no tick: ");
        Serial.print(xTaskGetTickCount());
        Serial.print(" o valor: ");
        Serial.println(i);
        //Serial.println(uxQueueSpacesAvailable(commQueue));
        vTaskDelay(pdMS_TO_TICKS(20)); //post no fim da fila a cada 20ms
    }
}


static void Consumer(void *pvParameters) {
  unsigned char c;
  while(1) {
    //Serial.print("Disponivel ao receber: ");
    //Serial.println(uxQueueSpacesAvailable(commQueue));
    xQueueReceive(commQueue, &c, pdMS_TO_TICKS(20));
    Serial.print("Recebi no tick: ");
    Serial.print(xTaskGetTickCount());
    Serial.print(" o valor: ");
    Serial.println(c);
    
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void loop() {
    // nada
}
