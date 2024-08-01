//https://wokwi.com/projects/404984634880005121
//https://www.youtube.com/watch?v=OZE7AgGWClc

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED 14
#define BT 12

TaskHandle_t xTaskTrataISRBTHandle;

void vTaskTrataISRBT(void *p);
void vInitHw(void);
void ISRcallback() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(xTaskTrataISRBTHandle, &xHigherPriorityTaskWoken);
}

void setup() {
  vInitHw();
  xTaskCreatePinnedToCore(vTaskTrataISRBT,
                          "task trata ISR BT",
                          configMINIMAL_STACK_SIZE+1024,
                          NULL,
                          1,
                          &xTaskTrataISRBTHandle,
                          APP_CPU_NUM); //1
  vTaskDelete(NULL);
}

void loop() {

}

void vInitHw(void) {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BT), ISRcallback, FALLING);
  Serial.println("Hardware....OK");
}

void vTaskTrataISRBT(void *p) {
  (void) p;
  while(true) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    digitalWrite(LED, !digitalRead(LED));
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
