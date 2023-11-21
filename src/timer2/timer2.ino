/**
 * FreeRTOS Solution to LED Dimmer
 * 
 * Turn on LED when entering serial commands. Turn it off if serial is inactive
 * for 5 seconds.
 * 
 * Date: February 1, 2021
 * Author: Shawn Hymel
 * License: 0BSD
 * Adaptado para readString por Josenalde Oliveira
 */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Settings
static const TickType_t dim_delay = 5000 / portTICK_PERIOD_MS; // 5s

// Pins (change this if your Arduino board does not have LED_BUILTIN defined)
static const int led_pin = 2;

// Globals
static TimerHandle_t one_shot_timer = NULL;

//*****************************************************************************
// Callbacks

// Turn off LED when timer expires
void autoDimmerCallback(TimerHandle_t xTimer) {
  digitalWrite(led_pin, LOW);
}

//*****************************************************************************
// Tasks

// Echo things back to serial port, turn on LED when while entering input
void doCLI(void *parameters) {

  String s;

  // Configure LED pin
  pinMode(led_pin, OUTPUT);

  while (1) {

    // See if there are things in the input serial buffer
    while(Serial.available() == 0) {}

      s = Serial.readString();
      Serial.println(s);

      // Turn on the LED
      digitalWrite(led_pin, HIGH);

      // Start timer (if timer is already running, this will act as // xTimerReset() instead)
      xTimerStart(one_shot_timer, portMAX_DELAY);
    
  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {

  // Configure Serial
  Serial.begin(115200);

  // Create a one-shot timer
  one_shot_timer = xTimerCreate(
                      "One-shot timer",     // Name of timer
                      dim_delay,            // Period of timer (in ticks)
                      pdFALSE,              // Auto-reload
                      (void *)0,            // Timer ID
                      autoDimmerCallback);  // Callback function

  // Start command line interface (CLI) task
  xTaskCreatePinnedToCore(doCLI,
                          "Do CLI",
                          1024,
                          NULL,
                          1,
                          NULL,
                          1);

  // Delete "setup and loop" task
}

void loop() {
  vTaskSuspend(NULL); // Execution should never get here
}
