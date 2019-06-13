// Switches.h

#ifndef _SWITCHES_h
#define _SWITCHES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void programswitches(int sourcechn, int sinkchn, int maxpins);
void SwitchesPwrOn(void);
void SwitchesPwrOff(void);



#endif
