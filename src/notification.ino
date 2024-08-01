/*
 By Josenalde Oliveira - task notification basics
*/

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

TaskHandle_t task1hdl = NULL;
TaskHandle_t task2hdl = NULL;

void task1(void *p) {
  while(true) {
    xTaskNotifyGive(task2hdl); //default notification value is 0 and Give adds 1, so it is 1
    // equivalent to a binary semaphore
    /* xTaskNotifyGive(task2hdl);
    xTaskNotifyGive(task2hdl);
    xTaskNotifyGive(task2hdl);
    */
    vTaskDelay(1000);
  }
}

void task2(void *p) {
  int notificationValue;
  while(true) {
    notificationValue = ulTaskNotifyTake(pdTRUE, (TickType_t) portMAX_DELAY); //1st: pdTrue (1) reset notification value received to 0
    if (notificationValue > 0) {
      Serial.print("Received ");
      Serial.println(notificationValue);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  xTaskCreatePinnedToCore(task1,
                          "task1",
                          1024,
                          NULL,
                          1,
                          NULL,
                          0);

  xTaskCreatePinnedToCore(task2,
                          "task2",
                          1024,
                          NULL,
                          1,
                          &task2hdl,
                          0);

  vTaskDelete(NULL);
}

void loop() {
  Serial.println("Never reached...");
  // this is a RTOS code, not a super loop()/baremetal
}

