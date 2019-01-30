/* This is code to start injecting at a selected frequency on startup and run forever


*/

/*
 * AD9954 stuff
 * 
 * 
 */

int CS_Freq = 1000;


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


/*
 * Switch and indicator stuff
 * 
 * 
 */
#include "PCBPinsRev1.h" // Pins to use the Rev 1 shield - Indicator pins are same as some used on CS board


int sinkpin = 1; // pin that the sink is connected to
int sourcepin = 4;

const int chnmax = 199; // maximum number of channels

int NumBoard = 1;
int TotalPins = 40 * NumBoard;

//int Protocol[][2] = { {1,17},{2,18},{3,19},{4,20},{5,21},{6,22}};
//int nprt = 6;
int Protocol[][2] = { {1, 2}, {2, 3}, {3, 4}, {4, 1}};
int nprt = 4;

int injTime = 1000; // Injection time in ms

// this writes the digital pin faster for due - only 2 clock cycles!
//taken from http://forum.arduino.cc/index.php?topic=129868.15
inline void digitalWriteDirect(int pin, int val) {
  if (val) g_APinDescription[pin].pPort->PIO_SODR = g_APinDescription[pin].ulPin;
  else    g_APinDescription[pin].pPort->PIO_CODR = g_APinDescription[pin].ulPin;
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

  Serial.println("Starting up and getting going");
  singleTone(CS_Freq);
  updateAD9954();


  Serial.println("Doing switch stuff");
  init_pins();
  ind_low();

  Serial.println("Pins initialised, should ALL be closed now");
  delay(1000);
  SwitchesPwrOn();
  


}

void loop() {

Serial.print("Looping through protocol steps, injection period: ");
  Serial.println(injTime);
  
  for (int i = 0; i < nprt; i++) {

    ind_high(); // indicators going high - to double check that using these pins dont break anything
    
    sourcepin = Protocol[i][0];
    sinkpin = Protocol[i][1];


    Serial.print("Setting switches to source chn: ");
    Serial.print(sourcepin);
    Serial.print(" sink chn: ");
    Serial.println(sinkpin);

    programswitches(sourcepin, sinkpin, TotalPins); //program dem switches
    digitalWriteDirect(SYNC, HIGH); // switch dat!

    ind_low(); // ind low again
    delay(injTime);


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
