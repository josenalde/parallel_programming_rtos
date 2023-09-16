# include <freertos/FreeRTOS.h>
# include <freertos/task.h>
# include <freertos/semphr.h>
# include <esp_system.h>

SemaphoreHandle_t SMF;

void IRAM_ATTR isr() {
    xSemaphoreGiveFromISR(SMF, NULL); //Libera o semaforo (sem_wait semaphore.h)
}

void t1(void* z) {
    while (1) {
         //Tenta obter o semaforo durante 200ms (Timeout). Caso o semaforo nao fique
        //disponivel em 200ms, retornara FALSE
        if (xSemaphoreTake(SMF, pdMS_TO_TICKS(200)) == true) { //sem_post
            //Se obteu o semaforo entre os 200ms de espera, fara o toggle do pino 23
            for (int i = 0; i < 4; i++) {
                digitalWrite(23, HIGH);
                vTaskDelay(pdMS_TO_TICKS(400));
                digitalWrite(23, LOW);
                vTaskDelay(pdMS_TO_TICKS(400));
            }
        } else {
            for (int i = 0; i < 4; i++) { //no pino 22
                digitalWrite(22, HIGH);
                vTaskDelay(pdMS_TO_TICKS(400));
                digitalWrite(22, LOW);
                vTaskDelay(pdMS_TO_TICKS(400));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
   }
 }

void setup() {
    Serial.begin(115200);
    SMF = xSemaphoreCreateBinary();
    pinMode(23, OUTPUT);
    pinMode(22, OUTPUT);
    pinMode(5, INPUT);
    attachInterrupt(5, isr, RISING);
    xTaskCreatePinnedToCore(t1, "t1" , 4096 , NULL, 1 , NULL, 0); //Cria a tarefa que analisa o semaforo
    //handle da função, alias para debug, stack size em bytes, parametro void* para a task, prioridade (1-25), no FreeRTOS ESP32 quanto maior,
    // maior a prioridade, sexto parâmetro não usado (local para guardar ID da task), afinidade do núcleo (0 PRO_CPU, 1 APP_CPU, tskNO_AFFINITY)
    //xPortGetCoreID()
}

void loop() {
    //Serial.print("pino 23: ");
    //Serial.println(digitalRead(23));
    Serial.print("pino 22: ");
    Serial.println(digitalRead(22));
}
