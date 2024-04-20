#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// Pin definitions
const uint8_t CE_PIN = 10;
const uint8_t CSN_PIN = 9;
const uint8_t ESC1_PIN = 6;
const uint8_t ESC2_PIN = 5;
const uint8_t FIN_PIN = 3;

// Radio and address
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

// Servo objects
Servo esc1, esc2, fin;

// Initialization of components
void setup() {
  Serial.begin(9600);
  initializeRadio();
  initializeServos();
}

// Main loop listening for RF data
void loop() {
  processReceivedData();
}

// Initialize the NRF24L01 radio
void initializeRadio() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

// Attach servos and move fin for setup
void initializeServos() {
  fin.attach(FIN_PIN);
  cycleFin();

  esc1.attach(ESC1_PIN, 1000, 2000);
  initializeESC(esc1);

  esc2.attach(ESC2_PIN, 1000, 2000);
  initializeESC(esc2);
}

// Simple sweep for fin initialization
void cycleFin() {
  for (int pos = 180; pos >= 0; pos--) {
    fin.write(pos);
    delay(10);
  }
  for (int pos = 0; pos <= 180; pos++) {
    fin.write(pos);
    delay(10);
  }
}

// General ESC initialization sequence
void initializeESC(Servo &esc) {
  esc.write(180);
  delay(5000);
  esc.write(0);
  delay(2000);
  esc.write(10);
}

// Process received data and control servos
void processReceivedData() {
  if (radio.available()) {
    int receivedData[2];
    radio.read(&receivedData, sizeof(receivedData));
    controlMotors(receivedData);
  }
}

// Map and write control values to ESCs
void controlMotors(const int *data) {
  int motorCW = map(data[0], 300, 0, 0, 180);
  int motorCCW = map(data[1], 300, 0, 0, 180);

  esc1.write(motorCW);
  esc2.write(motorCCW);

  // Debugging output
  Serial.print("CW: "); Serial.print(data[0]); Serial.print(" -> "); Serial.print(motorCW);
  Serial.print(" | CCW: "); Serial.print(data[1]); Serial.print(" -> "); Serial.println(motorCCW);
}

