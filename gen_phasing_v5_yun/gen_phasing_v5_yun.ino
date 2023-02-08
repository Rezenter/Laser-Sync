const byte interruptPin = 2; // pin for Globus sync input
const int genPin = 10; // output pin for laser
const int allowPin = 6; // output pin for 5V blocking

//EDIT HERE!
const unsigned int laserPulseCount = 99; //set the number of allowed laser pulses here
const unsigned int frequencyCounter = 717; // set the frequency here. 717 => 330.3[Hz]

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
  
  TIMSK0 = B00000010;   //Set the timer interrupt requests
  sei(); //Enable interrupt
}

void loop() {
  if(PORTD >> 7){
    if(pulse_count == 0){
      detachInterrupt(digitalPinToInterrupt(interruptPin));
      detached = true;
    }else if(pulse_count > laserPulseCount){  
      PORTD &= ~(1 << 7);
    }
  }
  if(detached && pulse_count > 1000){  //set the delay between sync signals here
    EIFR = B11111111; // clear interrupts
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
  if(count > frequencyCounter){                 
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
