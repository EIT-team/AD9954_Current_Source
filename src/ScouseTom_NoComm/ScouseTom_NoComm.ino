/* This is code to start injecting at a selected frequency on startup and run forever


*/

/* * AD9954 stuff *  *  */


#include <SPI.h>
#include "Settings.h"

//#include "AD9954.h" // Library for communication with AD9954 DDS chip
#include "AD9954_Comm.h" // Clock settings and commonly used registers  
#include "PCBPinsRev2.h" // Pins to use the Rev 2 shield - Indicator pins are same as some used on CS board
#include "IO_Pins.h"
#include "digitalDirect.h"
#include "Switches.h"




/*
 * Switch and indicator stuff
 *
 *
 */


int sinkpin = 0; // pin that the sink is connected to
int sourcepin = 0;


void setup() {
	init_pins();

	Serial.begin(115200);
	Serial.setTimeout(10);

	AD9954_Init();


	Serial.println("Starting up and getting going");
	delay(1000);

	singleTone(CS_Freq);
	updateAD9954();


	Serial.println("Doing switch stuff");

	ind_low();

	Serial.println("Pins initialised, should ALL be closed now");
	delay(1000);
	SwitchesPwrOn();



}

void loop() {

	Serial.print("Looping through protocol steps, injection period: ");
	Serial.println(injTime);

	for (int iPrt = 0; iPrt < nprt; iPrt++) {

		ind_high(); // indicators going high - to double check that using these pins dont break anything

		sourcepin = Protocol[iPrt][0];
		sinkpin = Protocol[iPrt][1];

		Serial.print("i is now");
		Serial.println(iPrt);
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

