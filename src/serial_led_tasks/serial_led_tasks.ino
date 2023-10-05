# include <freertos/FreeRTOS.h>
# include <freertos/task.h>
#include <stdlib.h>

/**
 Adaptado de Shawn Hymel, Dec. 3, 2020 - Serial e Led
 * License: 0BSD
*/

static const uint8_t buf_len = 20;

// LED invervalo de piscada inicial
static int rate_1 = 500;  // ms

// Pins
static const int led_pin_1 = 2;

// toggleLED: piscar led com rate_1
void toggleLED(void *parameter) {
  while(1) {
    digitalWrite(led_pin_1, HIGH);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    digitalWrite(led_pin_1, LOW);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
  }
}

// t2: ler numero da serial para mudar frequencia de piscada
void readSerial(void *parameters) {
  char c;
  char buf[buf_len];
  uint8_t idx = 0;

  // Clear whole buffer
  memset(buf, 0, buf_len);

  // Loop forever
  while (1) {
    // Read characters from serial // pode usar também Serial.parseInt e neste caso não precisa buffer
    if (Serial.available() > 0) {
      c = Serial.read();
      // Update delay variable and reset buffer if we get a newline character
      if (c == '\n') {
        rate_1 = atoi(buf);
        Serial.print("Alterei delay do LED para: ");
        Serial.println(rate_1);
        memset(buf, 0, buf_len);
        idx = 0;
      } else {
        // Only append if index is not over message limit
        if (idx < buf_len - 1) {
          buf[idx] = c;
          idx++;
        }
      }
    }
  }
}

void setup() {

  // Configura pin
  pinMode(led_pin_1, OUTPUT);

  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS); //aguarda 1s para a serial iniciar antes de escrever algo
  Serial.println("Multi-task LED Demo");
  Serial.println("Entre com numero de milissegundos para o Led piscar");
  
  // Tarefas em loop infinito
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED,  // função a ser chamada
              "ToggleLED",   // nome da task
              1024,         // tamanho em bytes
              NULL,         // parâmetros a passar a função
              1,            // Prioridade (0 a configMAX_PRIORITIES - 1)
              NULL,         // handle da task
              1);     // núcleo: 0 (pro) 1 (app)

  // Tarefas em loop infinito
  xTaskCreatePinnedToCore(  
              readSerial,  
              "readSerial",   
              1024,         
              NULL,         
              1,            
              NULL,         
              1);     

  // Apaga tarefas "setup e loop"
  vTaskDelete(NULL); // garante que loop não rodará
}

void loop() {
  // Diferente da abordagem super loop, com RTOS loop() não precisa fazer nada. Mas pode-se colocar algum vtaskdelay para que tarefas de menor prioridade acessem
  // setup() e loop() rodam suas tarefas com prioridade 1 no núcleo 1 no esp32
}