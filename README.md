This project consists of a hardware based, interrupt driven, embedded system based on an Arduino Uno, with an architecture modelled using the Sense - Think - Act concept. All interactions will occur using three types of inputs: PIR (passive infrared motion sensor), IR (infrared obstacle sensor), and ultrasonic (distance measuring sensor).

This project is designed to allow for concurrent operation of event-driven and time-driven behaviour using both Pin Change Interrupts (PCIs) and Timer1 interrupts.

The project consists of three layers and they are as follows:

1) Sense Layer (PIR, IR and Ultrasonic)

- D8 - PIR - Monitored using PCI for motion detection
- D9 - IR - Monitored using PCI for obstacle detection
- Trig/Echo - Distance measured periodically
- To ensure the immediate response of the system for motion or obstacle detection events, both of the sensors are using PCIs configured on PORTB (D8 - D9).

2) Think Layer (Processing)

- The only thing that an interrupt service routine (ISR) will do is set a volatile state flags.
- In the loop() function resides all of the decision-making logic.
- This provides a safe way of managing state information as well as providing an interrupt safe programming technique.

3) Act Layer (Output)

- The Serial Monitor logs event trigger and distance measurement data.
- A Timer 1 interrupt provides periodic updates to the system, e.g., logging or indicating status.

⏱ Configuration of Interrupts
PIN CHANGE INTERRUPT
- Configured in both PCICR and PCMSK0
- Monitors Pins D8 (PCINT0) & D9 (PCINT1).
- Used to detect asynchronous events (motion & obstacles)

TIMER 1 INTERRUPT
- Configured for periodic execution independently of PIN CHANGE INTERRUPT(PCI)
- Demonstrates separation of time-based versus event-based logic.

▶️ How to Execute the Project
- Open file TaskM1.cpp in Arduino IDE
- Connect Sensors according to the schematic wiring diagrams
- Upload the program into the Arduino UNO.
- Open serial monitor (9600 baud)
- Trigger:
    - Motion (PIR)
    - Object close to IR Sensor
    - Object moved away/closer from the ultrasonic sensor
- Serial Monitor should show
    - PCI triggering (motion, obstacle detection)
    - Timer event
    - Distance change
 

Hardware ( Components used ) 
- Arduino Uno
- PIR motion sensor
- IR sensor
- Ultrasonic Sensor (HC-SRO4)
- Jumper wires
- BreadBoard
