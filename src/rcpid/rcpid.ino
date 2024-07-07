unsigned long lastTime = 0, t;
double u=0, y=0, sp; // u: input, y: output (and sensor measure), sp: setpoint
double iTerm=0, lastY=0;
double kp, ki, kd;
unsigned long sampleTime = 200; // 1000 ms or 1 sec
double uMin, uMax; // control signal saturation
double voltageY;

double VCC = 3.3; // For ESP32 3.3, For Uno 5V
int yPin = 34; 
int uPin = 25; // PWM or DAC0 (25) and DAC1 (26)
static const uint16_t RESOLUTION = 4096; //also used for the maximum setpoint
static const uint16_t TIME_TO_DISCHARGE = 8000;

void computeU() {
    unsigned long now = millis();
    int timeChange = (now - lastTime);
    if (timeChange >= sampleTime) {
        double e = sp - y; // tracking error
        iTerm += (ki * e); // integral term I
        // integral term is always increasing, therefore it is limited
        if (iTerm > uMax) iTerm = uMax;
        else if (iTerm < uMin) iTerm = uMin;
        // y difference term
        double dY = (y - lastY);

        // compute control signal
        u = kp*e + iTerm - kd*dY;
        if (u > uMax) u = uMax;
        else if (u < uMin) u = uMin;

        lastY = y;
        lastTime = now;
    }
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


                     
void setup() {
  Serial.begin(115200);
  sp = 0.5*RESOLUTION;// considering 0-4095 (50%)
  pinMode(uPin, OUTPUT);
  pinMode(yPin, INPUT);
  setControlLimits(0, 255); // 255 = 3.3V DAC
  setSampleTime(sampleTime);
  setTunings(4.26, 1.72, 0.0);
  dacWrite(uPin, 0);
  vTaskDelay(pdMS_TO_TICKS(TIME_TO_DISCHARGE)); //wait capacitor discharge
}  
                           
void loop() {
  y = analogRead(yPin); // 0 - 2^Resolution
  // capacitor voltage (vo)
  voltageY = (VCC/RESOLUTION)*y; // for plotting
  computeU();
  dacWrite(uPin,u); //ESP32 pure DAC
  Serial.print(((sp+500)*VCC)/(RESOLUTION)); //upper limit
  Serial.print(", ");
  Serial.print((sp*VCC)/RESOLUTION);
  Serial.print(", ");
  Serial.println(voltageY);
  Serial.print(0.0); //lower limit
  Serial.print(", ");
  delay(10); //for plotting
  //setpoint profile
  t = millis();
  if (t >= TIME_TO_DISCHARGE+5000 && t < TIME_TO_DISCHARGE+2*5000) {
    sp = 0.8*RESOLUTION;
  } else if (t >= TIME_TO_DISCHARGE+2*5000) {
    sp = 0.2*RESOLUTION;
  }
}