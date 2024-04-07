#include <LiquidCrystal_I2C.h>
#include <DHTesp.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <math.h>

SemaphoreHandle_t sem_button_P; // pagination button
SemaphoreHandle_t sem_button_C; // cancel button

//declare tasks
TaskHandle_t taskHDisplayHome = NULL;
TaskHandle_t taskHReadTemp = NULL;
TaskHandle_t taskHButtonP = NULL;
TaskHandle_t taskHButtonC = NULL;
TaskHandle_t taskHDisplayTemp = NULL;

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);
DHTesp sensor;

//globals
volatile float temp_c, aux = 0.0;
volatile float temp_f;
volatile boolean tempHasChanged = false;
volatile boolean flagButton = true;
volatile boolean firstTime;

char *msg_init = "TADSautomation";

#define SENSOR_PIN 15 //sda

// isrs for button P and button C
void IRAM_ATTR isr_button_P() { //26
    xSemaphoreGiveFromISR(sem_button_P, NULL); 
}

void IRAM_ATTR isr_button_C() { //25
    xSemaphoreGiveFromISR(sem_button_C, NULL); 
}

// tasks

void taskDisplayHome(void* arg) {
    LCD.clear();
    LCD.setCursor(1, 0); //column, line
    LCD.print((char *) arg);
    vTaskSuspend(taskHDisplayTemp);
    vTaskSuspend(NULL);
}

void taskReadTemp(void* arg) {
  TempAndHumidity data;
  while (1) {
    data = sensor.getTempAndHumidity();
    temp_c = data.temperature; //data.humidity
    temp_f = (temp_c * 1.8) + 32;
    if (temp_c != aux) tempHasChanged = true;
    aux = temp_c;
    //Serial.println(tempHasChanged);
    vTaskDelay(pdMS_TO_TICKS(2000)); //Wait for a new reading from the sensor (DHT22 has ~0.5Hz sample rate)
  }
}

void taskButtonP(void* arg) {
  while (1) {
    if (xSemaphoreTake(sem_button_P, pdMS_TO_TICKS(100)) == true) { //sem_post
      flagButton = !flagButton;
      firstTime = true;
      LCD.clear();
      LCD.setCursor(1, 0); //column, line
      vTaskResume(taskHDisplayTemp);  
    }
    vTaskDelay(pdMS_TO_TICKS(50));
 }
}

void taskDisplayTemp(void *arg) {
  while (1) {
    if (tempHasChanged) {
      LCD.clear();
      LCD.setCursor(1, 0); //column, line
      if (flagButton && !isnan(temp_c)) {
          LCD.print(temp_c);
          LCD.print(" ");
          LCD.print("\xdf");
          LCD.print("C");
          firstTime = false;
      } else if (!flagButton && !isnan(temp_f)) {
          LCD.print(temp_f);
          LCD.print(" ");
          LCD.print("\xdf");
          LCD.print("F");
          firstTime = false;
      }
      tempHasChanged = false;
    } else {
      //Serial.println(flagButton);
      if (flagButton && !isnan(temp_c)  && firstTime) {
          LCD.print(temp_c);
          LCD.print(" ");
          LCD.print("\xdf");
          LCD.print("C");
          firstTime = false;
      } else if (!flagButton && !isnan(temp_f) && firstTime) {
          LCD.print(temp_f);
          LCD.print(" ");
          LCD.print("\xdf");
          LCD.print("F");
          firstTime = false;
      }
      
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

 void taskButtonC(void* arg) {
  while (1) {
    if (xSemaphoreTake(sem_button_C, pdMS_TO_TICKS(10)) == true) { //sem_post
      //vTaskSuspendAll(); //xTaskResumeAll()
      //vTaskResume(taskHDisplayHome);
      LCD.clear();
      LCD.setCursor(1, 0); //column, line
      LCD.print((char *)arg);
      //LCD.print((char *) arg);
      vTaskSuspend(taskHDisplayTemp);
    }
    vTaskDelay(pdMS_TO_TICKS(100)); // suspensão 100ms, permitindo outra thread executar
  }
 }

 void setup() {
  Serial.begin(115200);
  sensor.setup(SENSOR_PIN, DHTesp::DHT22);

  pinMode(25, INPUT); //cancel (return to greeting page)
  pinMode(26, INPUT); //toggle temperature F/C
  attachInterrupt(26, isr_button_P, RISING);
  attachInterrupt(25, isr_button_C, RISING);

  LCD.init();
  LCD.backlight();

  sem_button_P = xSemaphoreCreateBinary();
  sem_button_C = xSemaphoreCreateBinary();

  xTaskCreatePinnedToCore(taskReadTemp, "taskReadTemp" , 4096 , NULL, 1 , &taskHReadTemp, 1);
  xTaskCreatePinnedToCore(taskDisplayHome, "taskDisplayHome" , 4096 , (void *)(msg_init), 2 , &taskHDisplayHome, 1);
  xTaskCreatePinnedToCore(taskButtonP, "taskButtonP" , 4096 , NULL, 2 , &taskHButtonP, 0);
  xTaskCreatePinnedToCore(taskButtonC, "taskButtonC" , 4096 , (void *)(msg_init), 2 , &taskHButtonC, 0);
  xTaskCreatePinnedToCore(taskDisplayTemp, "taskDisplayTemp" , 4096 , NULL, 1 , &taskHDisplayTemp, 1);

  vTaskDelete(NULL);
}

void loop() {
  //vTaskDelay(pdMS_TO_TICKS(100));
}


