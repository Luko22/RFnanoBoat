//https://www.fambach.net/rf-nano/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(10, 9); // CE, CSN
const byte address[6] = "00001"; // the address the the module
///pins D10,D9,D13,D4,D5 : DO NOT USE THESE PINS
#include <Servo.h>
#define ESC1_PIN 6
#define ESC2_PIN 5
Servo esc1;
Servo esc2;
Servo fin;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  fin.attach(3);
  for(int i=180;i>0;i--){
    fin.write(i);
    delay(10);
  }
  for(int i=0;i<180;i++){
    fin.write(i);
    delay(10);
  }

  esc1.attach(ESC1_PIN, 1000, 2000);
  esc1.write(180);
  delay(5000);
  esc1.write(0);
  delay(2000);
  esc1.write(10);  

  esc2.attach(ESC2_PIN, 1000, 2000);
  esc2.write(180);
  delay(5000);
  esc2.write(0);
  delay(2000);
  esc2.write(10);

  
}

void loop() {
  if (radio.available()) {
    int receivedData[2];
    radio.read(&receivedData, sizeof(receivedData));

    int throttleCW = receivedData[0];
    int throttleCCW = receivedData[1];

    int motorCW = map(throttleCW, 300, 0, 0, 180);
    int motorCCW = map(throttleCCW, 300, 0, 0, 180);
    int rudder;

    // if (motorCW>(motorCCW*2)) rudder= map(motorCW, 0, 180, 90, 135);
    // else if(motorCCW>(motorCW*2)) rudder= map(motorCCW, 0, 180, 90, 45);
    // else rudder=90;

    // Control the motors
    esc1.write(motorCW);
    esc2.write(motorCCW);
    // fin.write(rudder);

    Serial.print(throttleCW);
    Serial.print("  |  ");
    Serial.print(motorCW);
    Serial.print("  |  ");
    Serial.print(throttleCCW);
    Serial.print("  |  ");
    Serial.println(motorCCW);
  }

}