#include "AD9954_Comm.h"
#include "PCBPinsRev2.h"
#include "SPI.h"
#include "Arduino.h"

void AD9954_Init()
{
	SPI.begin();
	SPI.setClockDivider(16);
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	
	digitalWrite(SS_PIN, LOW); // CS of AD9954 always active
	digitalWrite(RESET_PIN, LOW);
	digitalWrite(UPDATE_PIN, LOW);
	resetAD9954();
}



void resetAD9954() {
	// Send a pulse on the reset pin
	//Serial.println("Resetting AD9954");
	digitalWrite(RESET_PIN, HIGH);
	delay(1);
	digitalWrite(RESET_PIN, LOW);
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

