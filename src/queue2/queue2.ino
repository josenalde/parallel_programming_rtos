#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

QueueHandle_t queue;

void setup() {
    Serial.begin(115200);
    //delay(1000);
    //if (queue != NULL) xQueueReset(queue);
    queue = xQueueCreate(10, sizeof(int));
    if(queue == NULL){
        Serial.println("Error creating the queue");
    }
}

void loop() {
    if(queue == NULL) return;
    for(int i = 0; i<10; i++){
        xQueueSend(queue, &i, portMAX_DELAY);
    }
    Serial.print("Available spaces after sending...");
    Serial.println(uxQueueSpacesAvailable(queue));
    int element;
    for(int i = 0; i < 10; i++) {
        xQueueReceive(queue, &element, portMAX_DELAY);
        Serial.print(element);
        Serial.print("|");
    }
    Serial.println();
    Serial.print("Available spaces after receiving...");
    Serial.println(uxQueueSpacesAvailable(queue));
    delay(1000);
}
