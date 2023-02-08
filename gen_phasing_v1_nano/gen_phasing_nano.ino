const byte interruptPin = 2; // pin for Globus sync input
const int genPin = 3; // output pin for laser
const int highDelay = 15; //microseconds to hold high level
const int period = 3030; //microseconds between impulses 1/330[Hz] = 3030.(30)[mks]
const int periodCorrection = 27; // microseconds code correction for period

const int periodFixed = period - periodCorrection;
const int lowDelay = periodFixed - highDelay;
volatile unsigned long phase = 0;

const int led = 13;

void setup() {
  pinMode(genPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), phaseShift, RISING);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  //Serial.begin(9600);
}

void loop() {
  if(phase){
    int correction = period - (micros() - phase)%period;
    //Serial.println(correction);
    phase = 0;
    //Serial.println(correction);
    if(correction){
      //digitalWrite(led, HIGH);
      delayMicroseconds(correction);
      //digitalWrite(led, LOW);
    }
  }
  digitalWrite(genPin, HIGH);
  delayMicroseconds(highDelay);
  digitalWrite(genPin, LOW);
  delayMicroseconds(lowDelay);
}

void phaseShift() {
  phase = micros();
}
