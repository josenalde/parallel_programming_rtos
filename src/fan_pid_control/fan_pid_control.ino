/* Alterado por Josenalde Oliveira em 06.12.2023 */
                            
unsigned long lastTime;
double u=0, y=0, intSetpoint, rpmSetpoint; // u: input, y: output (and sensor measure), sp: setpoint
double iTerm=0, lastY=0;
double kp, ki, kd;
unsigned long sampleTime = 1000; // 1000 ms or 1 sec
double uMin, uMax; // control signal saturation
double voltageY;

double rpmInput;
volatile double aux; //Váriavel que armazena as rotações. manipulada pela interrupção

double VCC = 3.3; // For ESP32 3.3, For Uno 5
int yPin = 26; //GPIO26  PWM output to fan blue wire
int sensorPin = 25; // digital tachmeter (hall efect sensor)
/* pins used only for driver L298N
//int in1Pin = 12;
//int in2Pin = 14;
*/

const int rpmMax = 1920; // apply 255 to PWM wire and measure the rpm max

// ISR fan rotations
void countRPM() {
   aux++;//Incrementa a váriavel.
}

void computeU() {
    unsigned long now = millis();
    int timeChange = (now - lastTime);
    if (timeChange >= sampleTime) {
        
        rpmInput = aux;
        aux = 0; //Reseta a váriavel para proxima leitura.
        
        // read speed signal and convert it to RPM (bipolar hall effect sensor)
        // rpmInput*60/2, since its 2 readings each cycle
        y = map(rpmInput*30, 0, rpmMax, 0, 255); // 8 bits

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
  
  // ---------- configurations --------------------
  
  // configure fan speed sensor reading
  attachInterrupt(sensorPin, countRPM, RISING);//Habilita a interrupção 0 no pino 2. O UNO tem 02 interrupções
  
  // configure pwm output
  ledcAttachPin(yPin, 0); //channel 0 (pin 26)
  ledcSetup(0, 25000, 8); //channel, freq (25 kHz), resolution (bits)
  
  // set pin direction
  pinMode(sensorPin, INPUT);
  pinMode(yPin, OUTPUT); //PWM
  // for L298N
  //pinMode(in1Pin, OUTPUT);
  //pinMode(in2Pin, OUTPUT);
  
  
  rpmSetpoint = 1200; // max 1920 (avoid below 900)
  intSetpoint = map(rpmSetpoint, 0, rpmMax, 0, 255); // 8 bits

  setControlLimits(0, 255); // 255 = 3.3V PWM
  setSampleTime(1000);
  setTunings(1.41, 1.41/4.04, kp*2.92 );

  // set forward movement L298N
  //digitalWrite(in1Pin, LOW);
  //digitalWrite(in2Pin, HIGH);
  
}  
                            
void loop() { //super loop
  computeU();
  ledcWrite(0, u); //ESP32 PWM
  Serial.print(rpmMax);
  Serial.print(" ");
  Serial.print(0);
  Serial.print(" ");
  Serial.print(rpmInptut*30);
  Serial.print(" ");
  Serial.print(rpmSetpoint);
  Serial.println();  
  delay(10);
}
