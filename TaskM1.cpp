// TaskM1.cpp
// Complete Interrupt-Driven Multi-Sensor System
// PCI + Timer1 + Ultrasonic Measurement

// -------------------- Pin Definitions --------------------
#define PIR_PIN 8        // PCINT0 (PB0)
#define IR_PIN 9         // PCINT1 (PB1)

#define TRIG_PIN 3
#define ECHO_PIN 4

#define LED_PIN 13

// -------------------- Volatile Flags --------------------
volatile bool pirFlag = false;
volatile bool irFlag = false;
volatile bool timerFlag = false;

volatile uint8_t lastPortBState;

// -------------------- Setup --------------------
void setup() {
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(IR_PIN, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_PIN, OUTPUT);

  setupPCI();
  setupTimer1();
}

// -------------------- Main Loop --------------------
void loop() {
  handlePIR();
  handleIR();
  handleTimerTask();
}

// -------------------- PCI Setup --------------------
void setupPCI() {
  
  PCICR |= (1 << PCIE0);

  
  PCMSK0 |= (1 << PCINT0);  // D8
  PCMSK0 |= (1 << PCINT1);  // D9

  lastPortBState = PINB;
}

// -------------------- PCI ISR --------------------
ISR(PCINT0_vect) {
  uint8_t currentState = PINB;
  uint8_t changed = currentState ^ lastPortBState;

  // Check which pin changed
  if (changed & (1 << PB0)) pirFlag = true;  // D8
  if (changed & (1 << PB1)) irFlag = true;   // D9

  lastPortBState = currentState;
}

// -------------------- Timer1 Setup (1 second) --------------------
void setupTimer1() {
  noInterrupts();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  
  OCR1A = 15624;  // 1 second

  TCCR1B |= (1 << WGM12);                 
  TCCR1B |= (1 << CS12) | (1 << CS10);   

  TIMSK1 |= (1 << OCIE1A);              

  interrupts();
}

// -------------------- Timer ISR --------------------
ISR(TIMER1_COMPA_vect) {
  timerFlag = true;  
}

// -------------------- Sensor Handlers --------------------
void handlePIR() {
  if (pirFlag) {
    pirFlag = false;
    Serial.println("PCI Event: Motion detected (PIR)");
  }
}

void handleIR() {
  if (irFlag) {
    irFlag = false;
    Serial.println("PCI Event: Obstacle detected (IR)");
  }
}

void handleTimerTask() {
  if (timerFlag) {
    timerFlag = false;

    
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));

    long distance = measureUltrasonic();

    Serial.print("Timer Event: Distance = ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}

// -------------------- Ultrasonic Function --------------------
long measureUltrasonic() {
  // Send 10us trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  
  long duration = pulseIn(ECHO_PIN, HIGH);


  long distance = duration * 0.034 / 2;

  return distance;
}