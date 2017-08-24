#include <SPI.h>
#include "AD9954.h"

#define SS_PIN 8
#define RESET_PIN 12
#define UPDATE_PIN 9
#define PS0 11
#define PS1 10
#define OSK 15

// Register Commands
#define SET_SDIO 0x02
#define CFR1_WRITE 0x00
#define CFR1_READ 0x80
#define CFR2_WRITE 0x01
#define CFR2_READ 0x81
#define FREQ_WRITE 0x04

#define CLOCK 10e6
#define MAX_FREQ 4294967296

void resetAD9954() {
  // Send a pulse on the reset pin
  //Serial.println("Resetting AD9954");
  digitalWrite(RESET_PIN, HIGH);
  delay(1);
  digitalWrite(RESET_PIN, LOW);
}

void setup() {

  pinMode(SS_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  pinMode(UPDATE_PIN, OUTPUT);

  digitalWrite(SS_PIN, LOW); // CS of AD9954 always active
  digitalWrite(RESET_PIN, LOW);
  digitalWrite(UPDATE_PIN, LOW);

  SPI.begin();
  SPI.setClockDivider(16);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  Serial.begin(115200);
  Serial.setTimeout(10);
  resetAD9954();


}

void loop() {



  unsigned long freq;
  unsigned long input_value;

  while (1) {

    String what_to_program = "";

    if (Serial.available()) {

      // First part is the command name
      // Get a string, ignoring case
      what_to_program = Serial.readStringUntil(' ');
      what_to_program.toLowerCase();

      // Get an the frequency as an integer
      input_value = Serial.parseInt(); // Get an int

      Serial.read();
    }

    if (what_to_program == "frequency") {

      resetAD9954();
      freq = input_value;
      Serial.print("Frequency set to ");
      Serial.print(freq);
      Serial.println("Hz");
    }

    if (what_to_program == "stim") {

      if (input_value >= 1) {
        if (freq > 0) {
          Serial.println("Starting injection!");
          singleTone(freq);
          updateAD9954();
        }

        else {
          printInvalidFrequency();
        }
      }

      else {
        Serial.println("Stopping injection");
        resetAD9954();
      }
    }
  }

}




void singleTone(unsigned long freq) {
  // Calculates frequency tuning word and sets registers
  // An I/O Update (updateAD9954()) command needs to be issued after this to start the output

  Serial.print("Setting frequency to "); Serial.print(freq); Serial.println("Hz");
  // Calculate frequency word
  unsigned long frequency_word = MAX_FREQ * freq / CLOCK;

  // Split word into 4 bytes
  int num_bytes = 4;
  byte b[4];

  b[3] = lowByte(frequency_word);
  b[2] = lowByte(frequency_word >> 8);
  b[1] = lowByte(frequency_word >> 16);
  b[0] = lowByte(frequency_word >> 24);


  // Access frequency word register
  SPI.transfer(FREQ_WRITE);

  // Transfer frequency word as bytes
  for (int i = 0; i < num_bytes; i++) {
    //Serial.println(b[i]);
    SPI.transfer(b[i]);

  }

}

void updateAD9954() {
  // Send a pulse on the IO/Update pin to update frequency output
  //Serial.println("I/O Updating");
  digitalWrite(UPDATE_PIN, HIGH);
  delay(1);
  digitalWrite(UPDATE_PIN, LOW);
}


void printInvalidFrequency() {

  Serial.println(" Invalid frequency specified.");
  Serial.println(" Use command:");
  Serial.println(" frequency xxx");
  Serial.println(" To set frequency.");
}

