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

void setup() {
  // put your setup code here, to run once:
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
  resetAD9954();


}

void loop() {
  // put your main code here, to run repeatedly:

  //AD9954 DDS( SS_PIN, RESET_PIN, UPDATE_PIN, PS0, PS1, OSK);
  long freq = 10000;



  while (1) {
    //Serial.println("Module");
    //DDS.initialize(10000000, 0 );
    //DDS.setFreq(10000);
    //delay(3000);


    //  Serial.println("test");
    testWriteRead();


    resetAD9954();
    delay(1000);
    Serial.println("single tone");

    singleTone(freq);

    while (1) {

      delay(3000);
    }
  }

}



void testWriteRead() {

  SPI.transfer(CFR1_WRITE);

  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x02);
  SPI.transfer(0x00);
  updateAD9954();

  SPI.transfer(CFR1_READ);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(CFR2_READ);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);

}

void singleTone(long freq) {
  /*
    // 1.
    // Instruction byte
    SPI.transfer(0x00);

    //Data
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(0x00);


    //2
    SPI.transfer(0x01);

    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
  */

  //3
  // Frequency word is (Freq_Hz / Clock) * 2^32
  
  SPI.transfer(0x04);

  SPI.transfer(0x00);
  SPI.transfer(0x20);
  SPI.transfer(0xC4);
  SPI.transfer(0x9B);
  updateAD9954();


}

void updateAD9954() {
  // Send a pulse on the IO/Update pin to update frequency output
  Serial.println("I/O Updating");
  digitalWrite(UPDATE_PIN, HIGH);
  delay(1);
  digitalWrite(UPDATE_PIN, LOW);
}

void resetAD9954() {
  // Send a pulse on the reset pin
  Serial.println("Resetting AD9954");
  digitalWrite(RESET_PIN, HIGH);
  delay(1);
  digitalWrite(RESET_PIN, LOW);
}


