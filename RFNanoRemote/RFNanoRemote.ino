#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pin assignments
const uint8_t CE_PIN = 10;
const uint8_t CSN_PIN = 9;
const uint8_t JOYSTICK_X_PIN = A0;
const uint8_t JOYSTICK_Y_PIN = A1;

// RF24 Radio setup
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

// Structure to hold joystick data
struct JoystickData {
  int vertical;
  int horizontal;
} joystickData;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
}

void loop() {
  readJoystick();
  sendJoystickData();
  delay(50); // Delay to avoid overwhelming the radio
}

void readJoystick() {
  joystickData.vertical = analogRead(JOYSTICK_X_PIN);
  joystickData.horizontal = analogRead(JOYSTICK_Y_PIN);
  Serial.print("Vertical: ");
  Serial.print(joystickData.vertical);
  Serial.print(" | Horizontal: ");
  Serial.println(joystickData.horizontal);
}

void sendJoystickData() {
  // Send joystick data over NRF24L01+
  radio.write(&joystickData, sizeof(joystickData));
}
