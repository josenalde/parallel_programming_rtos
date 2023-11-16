#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Perform an action every 10 ticks.
 void taskA( void * pvParameters ) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 500; //1 tick

     // Initialise the xLastWakeTime variable with the current time.
     xLastWakeTime = xTaskGetTickCount();

     while(1) {
         // Wait for the next cycle.
        Serial.print("Antes de ligar...");
        Serial.println(xLastWakeTime);
        vTaskDelayUntil( &xLastWakeTime, xFrequency);
         digitalWrite(22, HIGH);
         vTaskDelayUntil( &xLastWakeTime, xFrequency);
         digitalWrite(22, LOW);
         Serial.print("Ao desligar...");
         Serial.println(xLastWakeTime);
         Serial.println("----------------------");
     }
 }

 // Perform an action every 10 ticks.
 /*void taskB( void * pvParameters ) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 0; //1 tick

     // Initialise the xLastWakeTime variable with the current time.
     xLastWakeTime = xTaskGetTickCount();

     while(1) {
         // Wait for the next cycle.
         vTaskDelayUntil( &xLastWakeTime, xFrequency);
         digitalWrite(22, LOW);
     }
 }*/

void setup() {

  // Configure Serial
  Serial.begin(115200);

  pinMode(22, OUTPUT);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(pdMS_TO_TICKS(1000));
  
  // Start CLI task
  xTaskCreatePinnedToCore(taskA,
                          "taskA",
                          1024,
                          NULL,
                          1,
                          NULL,
                          1);

  // Start blink task
  /*xTaskCreatePinnedToCore(taskB,
                          "taskB",
                          1024,
                          NULL,
                          1,
                          NULL,
                          1);*/

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}