# include <freertos/FreeRTOS.h>
# include <freertos/task.h>

/**
 Adaptado de Shawn Hymel, Dec. 3, 2020 - Serial e Led com mutex e variável local
 * License: 0BSD
*/

// Pins
static const int led_pin_1 = 2;

// toggleLED: piscar led com rate_1
void toggleLED(void *parameter) {
  // Copy the parameter into a local variable
  int num = *(int *)parameter; //typecast

  Serial.print("Recebi: ");
  Serial.println(num);

  while(1) {
    digitalWrite(led_pin_1, HIGH);
    vTaskDelay(num / portTICK_PERIOD_MS);
    digitalWrite(led_pin_1, LOW);
    vTaskDelay(num / portTICK_PERIOD_MS);
  }
}

void setup() {

  // Configura pin
  pinMode(led_pin_1, OUTPUT);

  long int delay_arg;

  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS); //aguarda 1s para a serial iniciar antes de escrever algo
  
  Serial.println("FreeRTOS Mutex    ");
  Serial.println("Entre com numero de milissegundos para o Led piscar");
  
  // aguarda algo da serial
  while (Serial.available() <= 0);

  // Read integer value
  delay_arg = Serial.parseInt();
  Serial.print("Sending: ");
  Serial.println(delay_arg);

  // Tarefas em loop infinito
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED,  // função a ser chamada
              "ToggleLED",   // nome da task
              1024,         // tamanho em bytes
              (void *)&delay_arg,         // parâmetros a passar a função
              1,            // Prioridade (0 a configMAX_PRIORITIES - 1)
              NULL,         // handle da task
              1);     // núcleo: 0 (pro) 1 (app)

  Serial.println("Done!");
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS); // coloca task em blocked
  // apenas para permitir tarefas de menor prioridade serem executadas algum momento
}