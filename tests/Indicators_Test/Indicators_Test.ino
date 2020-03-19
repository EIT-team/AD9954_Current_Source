
//#include "BreadboardPins.h" // Pins for breadboard version - used by kirill and me (testing)
#include "PCBPinsRev2.h" // Pins to use the Rev 1 shield - Indicator pins are same as some used on CS board

int del = 100 * 1000; // time to delay between hgih and low

void setup() {

  Serial.begin(115200);
  Serial.println("#############################");
  Serial.println("Hello There! This is a trigger pin test");
  Serial.print("All indicator pins should go high and low");
  Serial.println("#############################");
  init_pins();
  ind_low();

}

void loop() {

  ind_low();
  delayMicroseconds(del);
  ind_high();
  delayMicroseconds(del);

}


// this writes the digital pin faster for due - only 2 clock cycles!
//taken from http://forum.arduino.cc/index.php?topic=129868.15
inline void digitalWriteDirect(int pin, int val) {
  if (val) g_APinDescription[pin].pPort->PIO_SODR = g_APinDescription[pin].ulPin;
  else    g_APinDescription[pin].pPort->PIO_CODR = g_APinDescription[pin].ulPin;
}
