/*

  Code to test switch network only - connected to PCB

  Contionusly loops through protocol

  CHANGE TERMINATOR TO NEWLINE IN SERIAL MONITOR


  Jimmy 2014/2015

*/

//#include "BreadboardPins.h" // Pins for breadboard version - used by kirill and me (testing)
#include "PCBPinsRev2.h" // Pins to use the Rev 1 shield - Indicator pins are same as some used on CS board


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


  Serial.begin(115200);
  Serial.println("#############################");
  Serial.println("Hello There! This is a ScouseTom Switch Test");
  Serial.print("The number of boards in the daisy chain is : ");
  Serial.println(NumBoard);
  Serial.println("#############################");
  Serial.println("MAKE SURE YOU HAVE SET TERMINATOR TO NEWLINE :)");

  Serial.println("The switches will be powered on, source and sink pins opened, then turned off");
  Serial.println("This is repeated before allowing the user to select a pin");
  Serial.println("#############################");
  delay(1000);

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
