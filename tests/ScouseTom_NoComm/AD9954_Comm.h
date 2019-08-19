#ifndef _AD9954_COMM_H
#define _AD9954_COMM_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define CLOCK 10e6 // this is set by the resistor on the board 

// Register Commands
#define SET_SDIO 0x02
#define CFR1_WRITE 0x00
#define CFR1_READ 0x80
#define CFR2_WRITE 0x01
#define CFR2_READ 0x81
#define FREQ_WRITE 0x04
#define MAX_FREQ 4294967296

void AD9954_Init();
void resetAD9954();
void singleTone(unsigned long freq);
void updateAD9954();
void printInvalidFrequency();




#endif