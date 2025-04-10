//based on Rocha, Carlos R.@https://www.youtube.com/watch?v=pNl9338wKXQ

// circuit by Josenalde Oliveira: https://www.tinkercad.com/things/lnz2ALIx4NM-pidcontroldcencoder/editel?returnTo=%2Fthings%2Flnz2ALIx4NM-pidcontroldcencoder
/*
Nome	Quantidade	Componente
U1	     1	Arduino Uno R3
M1	     1	350 Motor CC com codificador
P1	     1	12 , 5 Fonte de energia
U2	     1	0.2 ms Osciloscópio
R1	     1	10 kΩ Resistor
U2       1  L293D
*/
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
  pinMode(6, OUTPUT);
}

void loop()
{
  if (micros() >= timeToCount) {
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
  //analogWrite(6, 180); L293D em ENTRADA1 controla PWM velocidade
}

void pulseCounterISR() {
  	pulse++;
}

void pulseCounterISR() {
  	pulse++;
}
