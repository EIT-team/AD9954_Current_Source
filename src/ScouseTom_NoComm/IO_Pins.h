// Indicator_Pins.h

#ifndef _IO_PINS_h
#define _IO_PINS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void init_pins();
void reset_pins();
void reset_ind();
void ind_low();
void ind_high();

#endif


