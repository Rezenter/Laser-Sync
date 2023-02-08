const byte interruptPin = 2; // pin for Globus sync input
const int genPin = 10; // output pin for laser
const int allowPin = 6; // output pin for 5V blocking

volatile unsigned int count = 0;
volatile unsigned int pulse_count = 0;
bool detached = false;


void setup() {
  pinMode(genPin, OUTPUT);
  pinMode(allowPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), phaseShift, FALLING);
  TCCR0A = B00000010;
  TCCR0B = B00000001;
  OCR0A = 0x20; //Value for period 
  
  TIMSK0 = B00000010;   //Set the interrupt requests
  sei(); //Enable interrupt
}

void loop() {
  if(PORTD >> 7){
    if(pulse_count == 0){
      detachInterrupt(digitalPinToInterrupt(interruptPin));
      detached = true;
    }else if(pulse_count > 99){
      PORTD &= ~(1 << 7);
    }
  }
  if(detached && pulse_count > 1000){
    attachInterrupt(digitalPinToInterrupt(interruptPin), phaseShift, FALLING);
    detached = false;
  }
}

void phaseShift() {
  TCNT0 = 0;
  PORTB &= ~(1 << 6);
  count = 0;
  pulse_count = 0;
  PORTD |= 1 << 7;
}

ISR(TIMER0_COMPA_vect){    //end of period interrupt
  if(count > 740){
    PORTB |= 1 << 6;
    count = 0;
    pulse_count += 1;
    return;
  }
  if(count > 3){
    PORTB &= ~(1 << 6);
  }
  count += 1;
}
