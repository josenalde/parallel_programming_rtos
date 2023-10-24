# include <freertos/FreeRTOS.h>
# include <freertos/task.h>
#include <freertos/queue.h>

/**
 * @file main.cpp
 * @author Evandro Teixeira
 * @brief 
 * @version 0.1
 * @date 14-01-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 adaptado por Prof. Josenalde Oliveira 2023 */

#define COLOR_BLACK         "\e[0;30m"
#define COLOR_RED           "\e[0;31m"
#define COLOR_GREEN         "\033[32m"
#define COLOR_YELLOW        "\e[0;33m"
#define COLOR_BLUE          "\e[0;34m"
#define COLOR_PURPLE        "\e[0;35m"
#define COLOR_CYAN          "\e[0;36m"
#define COLOR_WRITE         "\e[0;37m"
#define COLOR_RESET         "\033[0m"

#define MESSAGE_SIZE        32    // Tamanho da mensagem
#define NUMBER_OF_ELEMENTS  8     // Número de elementos na fila
#define BUTTON              5    // Pino do botão 
#define LED_BOARD           2     // Pino do LED
#define DEBOUNCE_BUTTON     1000000  // Tempo do debounce do botão (micros = 1s)

// Estrutura dados mensagem 
struct DataMsg_t {
  uint8_t state_led;
  char txt[MESSAGE_SIZE]; //31 caracteres imprimiveis
};

// Prototipo das tarefas 
void Tarefa_A(void *parameters);
void Tarefa_B(void *parameters);
void Tarefa_LED(void *parameters);

// Cria Handle da Mensagem de tarefa de A para tarefa B
QueueHandle_t Fila_Msg_A_para_B;

// Cria Handle da Mensagem da interrupção do botão para a tarefa LED
QueueHandle_t Fila_Button_para_LED;
/**
 * @brief Função da interrupção botão
 */
void IRAM_ATTR Button_ISR() {
  // tempo da ultima leitura do botão
  static uint32_t last_time = 0; 
  static DataMsg_t Msg = {.state_led = 0, .txt = {0}};

  // Algoritmo de debounce do botão
  if( (micros() - last_time) >= DEBOUNCE_BUTTON) {
    last_time = micros();
    if(Msg.state_led == 1) {
      // Prepara dado para ser publicado na fila
      Msg.state_led = 0;
      sprintf(Msg.txt,"LED BOARD: OFF"); //copia texto no membro txt da estrutura Msg
    }
    else {
      // Prepara dado para ser publicado na fila
      Msg.state_led = 1;
      sprintf(Msg.txt,"LED BOARD: ON"); //copia texto no membro txt da estrutura Msg
    }
    // Publica dado na fila
    xQueueSendFromISR(Fila_Button_para_LED, &Msg, pdFALSE);
  }
}
/**
 * @brief 
 */
void setup() {
  // Inicializa a Serial 
  Serial.begin(115200);
  Serial.printf("\n\rFreeRTOS - Fila\n\r");
  
  pinMode(BUTTON, INPUT);
  attachInterrupt(BUTTON, Button_ISR, RISING);
  
  // Inicializa pino do LED on Board
  pinMode(LED_BOARD,OUTPUT);
  digitalWrite(LED_BOARD, LOW);
  
  // Cria Fila de mensagem para comunicação entre as Tarefas A e B
  Fila_Msg_A_para_B = xQueueCreate(NUMBER_OF_ELEMENTS, (MESSAGE_SIZE * sizeof(char))); //32 bytes
  if(Fila_Msg_A_para_B == NULL) {
    Serial.printf("\n\rFalha em criar a fila Msg_A_para_B");
  }
  
  // Cria Fila de mensagem para função da inturrupção do botão para a Tarefa LED
  Fila_Button_para_LED = xQueueCreate(NUMBER_OF_ELEMENTS, sizeof(DataMsg_t));
  if(Fila_Button_para_LED == NULL) {
    Serial.printf("\n\rFalha em criar a fila Button_para_LED");
  }

  // Cria as tarefas da aplicação
  xTaskCreatePinnedToCore(Tarefa_A, "Tarefa_A", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL,0);
  xTaskCreatePinnedToCore(Tarefa_B, "Tarefa_B", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL,0);
  xTaskCreatePinnedToCore(Tarefa_LED, "Tarefa_LED", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 2, NULL,1); 
}
/**
 * @brief 
 */
void loop() {
  Serial.printf("\n\rDeleta tarefa LOOP");
  vTaskSuspend(NULL);
}
/**
 * @brief Tarefa A
 * 
 * @param parameters 
 */
void Tarefa_A(void *parameters) {
  // Variavel locais
  char txt_local[MESSAGE_SIZE] = {0};
  uint8_t msg_counter = 0;
  
  // Imprime informação da tarefa no barramento serial
  //Serial.printf(COLOR_GREEN);
  Serial.printf("\n\r%s", pcTaskGetTaskName(NULL));
  //Serial.printf(COLOR_RESET); 
  while (1) {  
    // Prepara dado para ser publicado na fila
    sprintf(txt_local,"Msg number: %3d, task A to B", msg_counter++);
    
    // Imprime o conteudo a ser publicado no barramento serial
    //Serial.print(COLOR_GREEN);
    Serial.printf("\n\r%s Envia  -> %s", pcTaskGetTaskName(NULL), txt_local);
    //Serial.print(COLOR_RESET); 
    // Publica dado na fila
    if(xQueueSend(Fila_Msg_A_para_B, (void*)&txt_local, (TickType_t)1000 ) != pdTRUE) {
      //Serial.print(COLOR_GREEN);
      Serial.printf("\n\rFalha em enviar os dados da fila");
      //Serial.print(COLOR_RESET); 
    }
    
    // Pausa a Tarefa A por 05 segundos 
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
}
/**
 * @brief Tarefa B
 * 
 * @param parameters 
 */
void Tarefa_B(void *parameters) {
  // Variavel locais
  char txt_local[MESSAGE_SIZE] = {0};
  // Imprime informação da tarefa 
  //Serial.print(COLOR_YELLOW);
  Serial.printf("\n\r%s", pcTaskGetTaskName(NULL) );
  //Serial.print(COLOR_RESET); 
  while (1) {
    // Checa se há dados na fila - e pausa a tarefa por 01 segundo
    if(xQueueReceive(Fila_Msg_A_para_B, &txt_local, pdMS_TO_TICKS(1000)) == pdPASS) {
      // Imprime conteudo da fila no barramento serial
      //Serial.print(COLOR_YELLOW);
      Serial.printf("\n\r%s Recebe -> %s", pcTaskGetTaskName(NULL), txt_local);
      //Serial.print(COLOR_RESET); 
    }
    
    // Imprime informação da tarefa no barramento serial
    //Serial.print(COLOR_YELLOW);
    Serial.printf("\n\r%s - time: %d s", pcTaskGetTaskName(NULL), (uint)(millis()/1000) );
    //Serial.print(COLOR_RESET); 
  }
}
/**
 * @brief Tarefa LED
 * 
 * @param parameters 
 */
void Tarefa_LED(void *parameters) {
  DataMsg_t Msg;
  // Imprime informação da tarefa 
  //Serial.print(COLOR_RED);
  Serial.printf("\n\r%s", pcTaskGetTaskName(NULL) );
  //Serial.print(COLOR_RESET); 
  while (1) {
    // Checa se há dados na fila - e suspende a tarefa enquato a fila estiver vazia
    if(xQueueReceive(Fila_Button_para_LED, &Msg, portMAX_DELAY) == pdPASS) {
      // Imprime conteudo da fila no barramento serial
      //Serial.print(COLOR_RED);
      Serial.printf("\n\r%s Recebe -> %s | state_led: %d", pcTaskGetTaskName(NULL), Msg.txt, Msg.state_led);
      //Serial.print(COLOR_RESET); 
      // Altera o valor do pino do LED on Board
      digitalWrite(LED_BOARD, Msg.state_led);
    }
    // Imprime informação da tarefa no barramento serial
    //Serial.print(COLOR_RED);
    Serial.printf("\n\r%s - time: %d s", pcTaskGetTaskName(NULL),(uint)(millis()/1000) );
    //Serial.print(COLOR_RESET); 
  }
}