//based on Rocha, Carlos R.@https://www.youtube.com/watch?v=pNl9338wKXQ

#define SENSOR 2
#define TIMEBASE 1000000

volatile unsigned long pulse;
unsigned long timeToCount, dt, lastCheck;
double speed; // RPM

void pulseCounterISR();

void setup()
{
  Serial.begin(115200);
  pinMode(SENSOR, INPUT);
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounterISR, FALLING);
  pulse = 0L;
  timeToCount = lastCheck = micros();
}

void loop()
{
  if (micros() >= timeToCount) { // non blocking
  	dt = micros() - lastCheck;
    noInterrupts();
    // 350 rpm = 0.0029093 * k => k = 120304.85 
    // speed = double(pulse) / dt
    speed = (120304.85 * (pulse)) / dt;
    pulse = 0L; //reset for next counting
    interrupts();
    lastCheck = micros();
    Serial.println(speed, 1);
    timeToCount = micros() + TIMEBASE;
  }
}

void pulseCounterISR() {
  	pulse++;
}
