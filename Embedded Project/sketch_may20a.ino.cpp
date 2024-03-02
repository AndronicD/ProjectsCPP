#include <Arduino.h>
#line 1 "C:\\Users\\andro\\Documents\\Arduino\\sketch_may20a\\sketch_may20a.ino"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const int buttonPin1 = 2;   // Button pin connected to Arduino digital pin 2
const int buttonPin2 = 3;   // Button pin connected to Arduino digital pin 2
const int buttonPin3 = 4;   // Button pin connected to Arduino digital pin 2
const int buttonPin4 = 5;   // Button pin connected to Arduino digital pin 2
const int buttonPin5 = 6;   // Button pin connected to Arduino digital pin 2
const int buttonPin6 = 7;   // Button pin connected to Arduino digital pin 2
long ts = 0;
long ts1 = 0;

static const uint8_t PIN_MP3_TX = 8; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 9; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

DFRobotDFPlayerMini player;


#line 20 "C:\\Users\\andro\\Documents\\Arduino\\sketch_may20a\\sketch_may20a.ino"
void setup();
#line 47 "C:\\Users\\andro\\Documents\\Arduino\\sketch_may20a\\sketch_may20a.ino"
void loop();
#line 20 "C:\\Users\\andro\\Documents\\Arduino\\sketch_may20a\\sketch_may20a.ino"
void setup() {
  DDRD &= ~(1 << buttonPin1);
  PORTD |= (1 << buttonPin1);
  DDRD &= ~(1 << buttonPin2);
  PORTD |= (1 << buttonPin2);
  DDRD &= ~(1 << buttonPin3);
  PORTD |= (1 << buttonPin3);
  DDRD &= ~(1 << buttonPin4);
  PORTD |= (1 << buttonPin4);
  DDRD &= ~(1 << buttonPin5);
  PORTD |= (1 << buttonPin5);
  DDRD &= ~(1 << buttonPin6);
  PORTD |= (1 << buttonPin6);

  Serial.begin(9600);          
  softwareSerial.begin(9600);

  if (player.begin(softwareSerial)) {
    player.volume(15);
    player.play(1);
  }
  else{
    
  }
}


void loop() {
  int val = analogRead(A0);
  int vol = map(val, 1, 1024, 0, 30);

  player.volume(vol);
  if((millis() - ts1) >= 245000){
    ts1 = millis();
    player.play(1);
  }
  if ((millis() - ts) >= 500) {
    ts = millis();
    byte buttonState = (PIND & (1 << buttonPin1));

    if (buttonState == 0) {
      Serial.write(1);
    }

    buttonState = (PIND & (1 << buttonPin2));

    if (buttonState == 0) {
      Serial.write(2);
    }

    buttonState = (PIND & (1 << buttonPin3));

    if (buttonState == 0) {
      Serial.write(3);
    }

    buttonState = (PIND & (1 << buttonPin4));

    if (buttonState == 0) {
      Serial.write(4);
    }

    buttonState = (PIND & (1 << buttonPin5));

    if (buttonState == 0) {
      Serial.write(5);
    }

    buttonState = (PIND & (1 << buttonPin6));

    if (buttonState == 0) {
      Serial.write(6);
    }
  
  
  }
}
#line 1 "C:\\Users\\andro\\Documents\\Arduino\\sketch_may20a\\receiver.ino"
// char Mymessage[10]; //Initialized variable to store recieved data
// void setup() {
//   // Begin the Serial at 9600 Baud
//   Serial.begin(9600);
// }

// void loop() {
//   Serial.readBytes(Mymessage,5); //Read the serial data and store in var
//   Serial.println(Mymessage); //Print data on Serial Monitor
//   delay(1000);
// }
