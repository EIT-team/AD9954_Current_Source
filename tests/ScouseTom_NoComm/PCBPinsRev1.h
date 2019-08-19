 /*Changed numbering so we can use the CS on the Rev1 shield but still use the indicators*/

// switch network pin definitions
const int DINp = 39;
const int DINn = 41;
const int SCLK = 43;
const int SYNC = 35;
const int RESET = 37;

// indicator pins
const int IND_SWITCH = 22;
const int IND_STIM = 24;
const int IND_START = 26;
const int IND_STOP = 28;
const int IND_FREQ = 30;

const int IND_EX_1= 32;
const int IND_EX_2= 34;
const int IND_EX_3= 36;


/*
//pins to turn the triggers on and off
const int TRIG_DISABLE_1 = 7;
const int TRIG_DISABLE_2 = 13; 

//interupt pins
const int INTR_PMARK = 25; // pin to decect pmark - MUST BE AT POSITION 0 OF A DUE PORT 25 is 0 on portD - check http://arduino.cc/en/Hacking/PinMappingSAM3X
const int CS_EXTRA=24; // bonus output pin on the CS side, not really used but whatever

//bonus pins
const int BONUS_1 =47;
const int BONUS_2 =49;
const int BONUS_3 =51;
const int BONUS_4 =53;

//Power Pins
const int PWR_STIM = 52;
const int PWR_SWITCH = 22;
*/
