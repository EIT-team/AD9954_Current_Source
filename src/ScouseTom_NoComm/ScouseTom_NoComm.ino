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

/*############ Indicator Pin things - consts in Pins.h and PCBPins.h ############*/

int pulses[NumInd] = { 0 }; //pulses left to do on the indicator channels
int indpinstate[NumInd] = { 0 }; //current state of the indicator pins
int iInd = 0; // counter for for loop in ind pins ISR (save defining all the time)
int IndinterruptCtr[NumInd] = { 0 }; // iterations of the interrupt routine for each pin channel

int pulsesleft = 0; // number of pulses left to do - used in indpins_check
int pulseleftptr = 0; // pointer for for loop in indpins_check, defined here for speed
int indpulseson = 0; // are pulses active? this flag is used to prevent checking pulses left when we know there are none
int iTrigChk = 0; //iteration of indicator pin check loop


/*
 * Switch and indicator stuff
 *
 *
 */


int sinkpin = 0; // pin that the sink is connected to
int sourcepin = 0;


void setup() {

	//SETUP TIMERS
	//number comes from here https://github.com/ivanseidel/DueTimer/blob/master/TimerCounter.md

	pmc_set_writeprotect(false);		 // disable write protection for pmc registers
	pmc_enable_periph_clk(ID_TC8); // enable TC8 or instance T8 on timer TC2 channel 2 - this is the timer for indicator pins

	// set up timers and interupts - set channel on timers, set to "wave mode" meaning an output rather than "capture" to read ticks
	TC_Configure(TC2, 2, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK2); // use TC2 channel 2 in "count up mode" using MCLK /8 clock1 to give 10.5MHz

	TC_SetRC(TC2, 2, 105); // count 105 ticks on the 10.5MHz clock before calling the overflow routine - this gives an interupt every 10 uS
	
	//enable timer interupts on timer setup for indicators
	TC2->TC_CHANNEL[2].TC_IER = TC_IER_CPCS;   // IER = interrupt enable register
	TC2->TC_CHANNEL[2].TC_IDR = ~TC_IER_CPCS;  // IDR = interrupt disable register
	//Enable the interrupt in the nested vector interrupt controller
	// TC8_IRQn where 8 is the timer number * timer channels (3) + the channel number (=(2*3)+2) for timer2 channel2
	NVIC_EnableIRQ(TC8_IRQn);



	init_pins();

	Serial.begin(115200);
	Serial.setTimeout(10);

	AD9954_Init();

	Serial.println("Hello Simple ScouseTom Here");
	delay(1000);

	singleTone(CS_Freq);
	updateAD9954();

	ind_low();

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

void TC8_Handler() // this is the ISR for the indicator pins - cycles through all of the pins if they should change their state - this runs every 10uS
{
	// We need to get the status to clear it and allow the interrupt to fire again
	TC_GetStatus(TC2, 2); //here TC2,2 means TIMER 2 channel 2

	for (iInd = 0; iInd < NumInd; iInd++) // cycle through the indicator pins
	{
		if (pulses[iInd] > 0) //if we have some pulses left to do
		{
			if (!indpinstate[iInd] && (IndinterruptCtr[iInd] < IndPulseWidth)) // set pin high if less than pulse width
			{
				digitalWriteDirect(IndPins[iInd], 1);
				indpinstate[iInd] = 1;
				digitalWriteDirect(IND_EX_3, 0); //dummy pulse for Actichamp
			}
			else if (indpinstate[iInd] && (IndinterruptCtr[iInd] > IndPulseWidth)) //set pin low if greater than pulse wifth
			{
				digitalWriteDirect(IndPins[iInd], 0);
				indpinstate[iInd] = 0;
				digitalWriteDirect(IND_EX_3, 1); //dummy pulse for Actichamp
			}
			IndinterruptCtr[iInd]++; //increment tick counter
			if (IndinterruptCtr[iInd] == IndPulseWidthTotal) // if total pulse length (HIGH and LOW) happened then decrement pulses left
			{
				pulses[iInd]--;
				IndinterruptCtr[iInd] = 0;
			}

		}
	}
}