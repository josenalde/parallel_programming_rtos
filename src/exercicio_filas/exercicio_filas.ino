#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

// adaptado de https://www.digikey.com/en/maker/projects/introduction-to-rtos-solution-to-part-5-freertos-queue-example/72d2b361f7b94e0691d947c7c29a03c9

/*
* Date: January 18, 2021
* Author: Shawn Hymel
* License: 0BSD
*/

// Settings
static const uint8_t buf_len = 255;     // Size of buffer to look for command
static const char command[] = "delay "; // Note the space!
static const int delay_queue_len = 5;   // Size of delay_queue
static const int msg_queue_len = 5;     // Size of msg_queue
static const uint8_t blink_max = 10;   // Num times to blink before message

// Pins (change this if your Arduino board does not have LED_BUILTIN defined)
static const int led_pin = 22;

// Message struct: used to wrap strings (not necessary, but it's useful to see
// how to use structs here)
struct Message {
  char body[50];
  int count;
};

// Globals
static QueueHandle_t queue_1; // delay_queue
static QueueHandle_t queue_2; //msg_queue

//*****************************************************************************
// Tasks

// Task: command line interface (CLI) Task A
void taskA(void *parameters) {

  Message rcv_msg; // store message from queue_2
  char c;
  char buf[buf_len]; //255
  uint8_t idx = 0;
  uint8_t cmd_len = strlen(command); //6
  int led_delay;

  // Clear whole buffer
  memset(buf, 0, buf_len); // used to fill a memory block starting at buf, buf_len bytes, with 0

  // Loop forever
  while (1) {

    // See if there's a message in the queue (do not block)
    if (xQueueReceive(queue_2, (void *)&rcv_msg, 0) == pdTRUE) {
      Serial.print(rcv_msg.body);
      Serial.println(rcv_msg.count);
    }

    // Read characters from serial
    if (Serial.available() > 0) {
      c = Serial.read();

      // Store received character to buffer if not over buffer limit
      if (idx < buf_len - 1) {
        buf[idx] = c;
        idx++;
      }

      // Print newline and check input on 'enter'
      if ((c == '\n') || (c == '\r')) {

        // Print newline to terminal
        Serial.print("\r\n");

        // Check if the first 6 characters are "delay "
        if (memcmp(buf, command, cmd_len) == 0) {

          // Convert last part to positive integer (negative int crashes)
          char* tail = &buf[cmd_len];
          led_delay = atoi(tail);
          led_delay = abs(led_delay);
          Serial.print("oi");
          Serial.println(led_delay);

          // Send integer to other task via queue 1
          if (xQueueSend(queue_1, (void *)&led_delay, portMAX_DELAY) != pdTRUE) {
            Serial.println("ERROR: Could not put item on delay queue.");
          }
        }

        // Reset receive buffer and index counter
        memset(buf, 0, buf_len);
        idx = 0;

      // Otherwise, echo character back to serial terminal
      } else {
        Serial.print(c);
      }
    } 
  }
}

volatile int count = 1;
// Task B: flash LED based on delay provided, notify other task every 10 blinks
void taskB(void *parameters) {

  Message msg;
  int led_delay = 500; //default
  uint8_t counter = 0; //manage how many times has blinked

  // Set up pin
  pinMode(led_pin, OUTPUT);

  // Loop forever
  while (1) {

    // See if there's a message in the queue_1 (do not block)
    if (xQueueReceive(queue_1, (void *)&led_delay, 0) == pdTRUE) {

      // Best practice: use only one task to manage serial comms
      strcpy(msg.body, "Message received ");
      msg.count = count++;
      xQueueSend(queue_2, (void *)&msg, 10); //taskA prints this message
    }

    // Blink
    digitalWrite(led_pin, HIGH);
    vTaskDelay(pdMS_TO_TICKS(led_delay));
    digitalWrite(led_pin, LOW);
    vTaskDelay(pdMS_TO_TICKS(led_delay));

    // If we've blinked 10 times, send a message to the other task
    counter++;
    if (counter >= blink_max) {
      
      // Construct message and send
      strcpy(msg.body, "Blinked: ");
      msg.count = counter;
      xQueueSend(queue_2, (void *)&msg, 10);

      // Reset counter
      counter = 0;
    }
  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {

  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(pdMS_TO_TICKS(1000));
  Serial.println();
  Serial.println("---FreeRTOS Queue Solution---");
  Serial.println("Enter the command 'delay xxx' where xxx is your desired ");
  Serial.println("LED blink delay time in milliseconds");

  // Create queues
  queue_1 = xQueueCreate(delay_queue_len, sizeof(int));
  queue_2 = xQueueCreate(msg_queue_len, sizeof(Message));

  // Start CLI task
  xTaskCreatePinnedToCore(taskA,
                          "taskA",
                          1024,
                          NULL,
                          1,
                          NULL,
                          1);

  // Start blink task
  xTaskCreatePinnedToCore(taskB,
                          "taskB",
                          1024,
                          NULL,
                          1,
                          NULL,
                          1);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}