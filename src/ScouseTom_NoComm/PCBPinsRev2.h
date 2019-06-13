 /*Changed numbering so we can use the CS on the Rev1 shield but still use the indicators*/

// switch network pin definitions
const int DINp = 10;
const int DINn = 11;
const int SCLK = 12;
const int SYNC = 8;
const int RESET = 9;
const int TEST_1 = 7;
const int TEST_2 = 13;

// indicator pins
const int IND_SWITCH = 3;
const int IND_STIM = 4;
const int IND_START = 5;
const int IND_STOP = 6;
const int IND_FREQ = 16;

const int IND_EX_1= 15;
const int IND_EX_2= 14;
const int IND_EX_3= 2;

//AD9954 pins

const int SS_PIN = 17;
const int RESET_PIN = 21;
const int UPDATE_PIN = 18;
const int PS0 = 20;
const int PS1 = 19;
const int OSK = 27;


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
