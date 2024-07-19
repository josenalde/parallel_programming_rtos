/* Alterado por Josenalde Oliveira em 16.07.2024 */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Settings
static const TickType_t rpsInterval = 1000 / portTICK_PERIOD_MS;  // 1000ms = 1s (timer for getting pulses) - rotation per second
static const TickType_t controlSampleTime = 10 / portTICK_PERIOD_MS;  

unsigned long lastTime;
double u = 0, y = 0, intSetpoint, rpmSetpoint; // u: input, y: output (and sensor measure), sp: setpoint
double iTerm = 0, lastY = 0;
double kp, ki, kd;
unsigned long sampleTime; //after getting a new speed calculate a new control signal MV
double uMin, uMax; // control signal saturation
double voltageY;

int rpmInput;
volatile int pulse; //Variable for counting rotating pulses

double VCC = 3.3; // For ESP32 3.3
int yPin = 25; //GPIO25 PWM
//A0 for sensor pin (pv sensor)
int sensorPin = 27; 
int in1Pin = 12;
int in2Pin = 14;

long rpmMax = 4080; //it requires validation - apply 255 to ENABLE A and measure speed RPM

// ISR fan rotations
void IRAM_ATTR countPulses() {
   pulse++;//increments each RISING edge.
}

// Globals
static TimerHandle_t getRPMTimer = NULL;
static TimerHandle_t generatePIDControlTimer = NULL;

void setTunings(double kP, double kI, double kD);
void setSampleTime(int newSampleTime);
void setControlLimits(double min, double max);
float float_map(float x, float in_min, float in_max, float out_min, float out_max);

//*****************************************************************************
// Callbacks

// get a new sample
void getRPMCallback(TimerHandle_t xTimer) {
  rpmInput = (pulse/2) * 60; // two pulses each rotation - hall effect sensor
  pulse = 0;
}

void generatePIDControlCallback(TimerHandle_t xTimer) {
    // get PID parameters - potentiometer
    setTunings(float_map(analogRead(33), 0, 4095, 0.1, 4.0), float_map(analogRead(32), 0, 4095, 0.1, 4.0), 0.0);
    y = map(rpmInput, 0, rpmMax, 0, 255);
    double e = intSetpoint - y; // tracking error
    iTerm += (ki * e); // integral term I
    // integral term is always increasing, therefore it is limited
    if (iTerm > uMax) iTerm = uMax;
    else if (iTerm < uMin) iTerm = uMin;
    // y difference term
    double dY = (y - lastY);
    u = kp*e + iTerm - (kd*dY);
    if (u > uMax) u = uMax;
    else if (u < uMin) u = uMin;
    analogWrite(yPin, u); //ESP32 PWM - ENABLE A L298D
}

void taskPrintGraph(void *parameters) {
  while (true) {
    Serial.print(255);
    Serial.print(" ");
    Serial.print(0);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.print(intSetpoint);
    Serial.println(); 
    //Serial.println(rpmInput);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
             
void setup() {
  Serial.begin(115200);
  
  getRPMTimer = xTimerCreate(
    "getRPMTimer",    // Name of timer
    rpsInterval,           // Period of timer (in ticks)
    pdTRUE,                     // Auto-reload TRUE, one_shot FALSE
    (void *)0,                  // Timer ID
    getRPMCallback);  // Callback function

  generatePIDControlTimer = xTimerCreate(
    "generatePIDControlTimer",    // Name of timer
    controlSampleTime,           // Period of timer (in ticks)
    pdTRUE,                     // Auto-reload TRUE, one_shot FALSE
    (void *)1,                  // Timer ID
    generatePIDControlCallback);  // Callback function
  
  // ---------- configurations --------------------
  // configure fan speed sensor reading
  attachInterrupt(sensorPin, countPulses, RISING);
  
  // configure pwm output
  analogWriteResolution(yPin, 8);
  analogWriteFrequency(yPin, 30000);

  //ledcAttach(yPin, 25000, 8); //(pin 25) ledcAttachPin DEPRACTED - Arduino-ESP32 LEDC API
    
  // set pin direction
  pinMode(sensorPin, INPUT);
  pinMode(yPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(33, INPUT);
  pinMode(32, INPUT);
    
  // set forward movement
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  
  rpmSetpoint = 2040; // considering 0-4095 - USER ENTERS THIS VALUE UI
  intSetpoint = map(rpmSetpoint, 0, rpmMax, 0, 255); // 8 bits

  setControlLimits(0, 255); // 255 = 3.3V PWM
  
  //************************/
  setSampleTime(10);
  //********************* */
  
  //setTunings(0.1, 1.2, 0.01);
  //setTunings(2, 5, 0.8); 

  // Start Printing task
  xTaskCreatePinnedToCore(taskPrintGraph,
                          "taskPrintGraph",
                          1024,
                          NULL,
                          2,
                          NULL,
                          0);

  xTimerStart(getRPMTimer, 0);
  xTimerStart(generatePIDControlTimer, 0); 
}  
                            
void loop() { 
  vTaskSuspend(NULL);
}

void setTunings(double kP, double kI, double kD) {
    double sampleTimeInSec = ((double)sampleTime/1000);
    kp = kP;
    ki = kI * sampleTimeInSec;
    kd = kD / sampleTimeInSec;
}

void setSampleTime(int newSampleTime) {
    if (newSampleTime > 0) {
        double ratio = (double)newSampleTime / (double)sampleTime;
        ki *= ratio;
        kd /= ratio;
        sampleTime = (unsigned long)newSampleTime;
    }
}

void setControlLimits(double min, double max) {
    if (min > max) return;
    uMin = min;
    uMax = max;
    if (u > uMax) u = uMax;
    else if (u < uMin) u = uMin;
    if (iTerm > uMax) iTerm = uMax;
    else if (iTerm < uMin) iTerm = uMin;
}

float float_map(float x, float in_min, float in_max, float out_min, float out_max) {
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

 
