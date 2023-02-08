const byte interruptPin = 2; // pin for Globus sync input
const byte interruptPin_debug = 3; // pin for electric sync input
const int genPin = 7; // output pin for laser
const int allowPin = 6; // output pin for 5V blocking
const int allowWidth = 350000; //microseconds to hold 5V enable signal
const int highDelay = 15; //microseconds to hold high level
const int period = 3030; //microseconds between impulses 1/330[Hz] = 3030.(30)[mks]
const int periodCorrection = 8; // microseconds code correction for period

const int periodFixed = period - periodCorrection;
const int lowDelay = periodFixed - highDelay;
volatile unsigned long phase = 0;
volatile unsigned long allowStart = 0;

const int led = 13;

void setup() {
  pinMode(genPin, OUTPUT);
  pinMode(allowPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  pinMode(interruptPin_debug, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), phaseShift, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPin_debug), phaseShift, RISING);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  //Serial.begin(9600);
}

void loop() {
  if(phase){
    digitalWrite(allowPin, HIGH);
    int correction = period - (micros() - phase)%period;
    //Serial.println(correction);
    allowStart = phase;
    phase = 0;
    //Serial.println(correction);
    if(correction){
      //digitalWrite(led, HIGH);
      delayMicroseconds(correction);
      //digitalWrite(led, LOW);
    }
  }
  if(allowStart && (micros() - allowStart > allowWidth)){
    allowStart = 0;
    digitalWrite(allowPin, LOW);
  }
  digitalWrite(genPin, HIGH);
  delayMicroseconds(highDelay);
  digitalWrite(genPin, LOW);
  delayMicroseconds(lowDelay);
}

void phaseShift() {
  phase = micros();
}
