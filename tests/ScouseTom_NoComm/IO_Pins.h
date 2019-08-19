// Indicator_Pins.h

#ifndef _IO_PINS_h
#define _IO_PINS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "PCBPinsRev2.h"

const int IndPulseWidth = 50; //this is in number of 10uS interrupts - 50 gives just above 500uS pulse width
const int IndPulseWidthTotal = IndPulseWidth * 2; //this is the total size of the pulse 100 is 1ms. two variables to save clock cycles in ISR

const int NumInd = 4; // number of indicator pins - CHANGING THIS WOULD MEAN CHANING THE INDPINS FUNCTIONS TOO
const int IndPins[NumInd] = { IND_START, IND_STOP, IND_SWITCH, IND_FREQ }; // pin numbers for the indicators


const int TrigCheckHighTime = 20000; //us pins are high for in checking trigger
const int TrigCheckLowTime = TrigCheckHighTime * 10; //us pins are LOW in checking trigger
const int MaxTrigCheckLoops = 20; // number of times to run the trig check loop - i.e. how many times to pulse indicators before stopping






// Globals - I know I shouldnt use these
//extern int pulsesleft

extern int pulses[]; //pulses left to do on the indicator channels
extern int indpinstate[]; //current state of the indicator pins
extern int iInd; // counter for for loop in ind pins ISR (save defining all the time)
extern int IndinterruptCtr[]; // iterations of the interrupt routine for each pin channel

extern int pulsesleft; // number of pulses left to do - used in indpins_check
extern int pulseleftptr; // pointer for for loop in indpins_check, defined here for speed
extern int indpulseson; // are pulses active? this flag is used to prevent checking pulses left when we know there are none
extern int iTrigChk; //iteration of indicator pin check loop



void init_pins();
void reset_pins();
void reset_ind();
void ind_low();
void ind_high();
void indpins_check();
void indpins_pulse(int StartChn, int StopChn, int SwitchChn, int FreqChn);
void indpins_init();
void indChnIdent();
void ind_pulse_with_delay(int chn, int del, int repeats);

#endif


