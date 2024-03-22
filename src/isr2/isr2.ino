# include <freertos/FreeRTOS.h>
# include <freertos/task.h>
# include <freertos/semphr.h>

SemaphoreHandle_t SMF;

void IRAM_ATTR isr() { // não pode ser mutex dentro de isr
    xSemaphoreGiveFromISR(SMF, NULL); //Libera o semaforo (sem_wait semaphore.h) colocando em estado 1
}

void t1(void* z) {
    while (1) {
         //Tenta obter o semaforo durante 200ms (Timeout). Caso o semaforo nao fique
        //disponivel em 200ms, retornara FALSE
        if (xSemaphoreTake(SMF, pdMS_TO_TICKS(200)) == true) { //sem_post
            //Se obteu o semaforo entre os 200ms de espera, fara o toggle do pino 23
            //Para liberação imediata colocar pdMS_TO_TICKS(0)
            for (int i = 0; i < 3; i++) {
                digitalWrite(23, HIGH);
                vTaskDelay(pdMS_TO_TICKS(200));
                digitalWrite(23, LOW);
                vTaskDelay(pdMS_TO_TICKS(200));
            }
            Serial.print("Estou no trecho após liberação do semáforo pela isr com SMF= ");
            /* Se é de contagem retorna o valor corrente. Se é binário retorna 1 se disponível e 0 em caso contrário (bloqueado) */
            Serial.println(uxSemaphoreGetCount(SMF));
            Serial.println("Pisquei o LED no pino 23 (red)");
            /* xSemaphoreGive(SMF) não é necessário para sincronização com ISR. Retorna ao início do laço e fica esperando
               o semáforo ser liberado novamente  /* We have finished our task.*/
        } else {
            for (int i = 0; i < 3; i++) { //no pino 22
                digitalWrite(22, HIGH);
                vTaskDelay(pdMS_TO_TICKS(200));
                digitalWrite(22, LOW);
                vTaskDelay(pdMS_TO_TICKS(200));
            }
            Serial.print("Pisquei o LED no pino 22 (green) com SMF = ");
            Serial.println(uxSemaphoreGetCount(SMF)); /* estado 0 */
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // suspensão 100ms, permitindo outra thread executar
   }
 }

void setup() {
    Serial.begin(115200);
    SMF = xSemaphoreCreateBinary();
    pinMode(23, OUTPUT);
    pinMode(22, OUTPUT);
    pinMode(26, INPUT);
    attachInterrupt(26, isr, RISING);
    xTaskCreatePinnedToCore(t1, "t1" , 4096 , NULL, 1 , NULL, 0); //Cria a tarefa que analisa o semaforo
    //handle da função, alias para debug, stack size em bytes, parametro void* para a task, prioridade (1-25), no FreeRTOS ESP32 quanto maior,
    // maior a prioridade, sexto parâmetro não usado (local para guardar ID da task), afinidade do núcleo (0 PRO_CPU, 1 APP_CPU, tskNO_AFFINITY)
    //xPortGetCoreID()
}

void loop() {
    
}

