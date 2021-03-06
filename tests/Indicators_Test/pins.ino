/*
Stuff for initilalising pins and indicator pins*/

void init_pins()
{
	//Set all pins as outputs and set initial values

	// pins for switching
	pinMode(DINp, OUTPUT);
	pinMode(DINn, OUTPUT);
	pinMode(SCLK, OUTPUT);
	pinMode(SYNC, OUTPUT);
	pinMode(RESET, OUTPUT);

	//pins for indicator channels
	pinMode(IND_SWITCH, OUTPUT);
	pinMode(IND_STIM, OUTPUT);
	pinMode(IND_FREQ, OUTPUT);
	pinMode(IND_START, OUTPUT);
	pinMode(IND_STOP, OUTPUT);

	pinMode(IND_EX_1, OUTPUT);
	pinMode(IND_EX_2, OUTPUT);
	pinMode(IND_EX_3, OUTPUT);



}

void reset_pins()
{
	//set all pins to default value - this is called by init but also when stopping injection

	digitalWriteDirect(DINp, LOW);
	digitalWriteDirect(DINn, LOW);
	digitalWriteDirect(SCLK, LOW);
	digitalWriteDirect(SYNC, LOW);
	digitalWriteDirect(RESET, LOW);


}

void reset_ind()
{
	//reset all indicator pins 
	digitalWriteDirect(IND_SWITCH, LOW);
	digitalWriteDirect(IND_STIM, LOW);
	digitalWriteDirect(IND_FREQ, LOW);
	digitalWriteDirect(IND_START, LOW);
	digitalWriteDirect(IND_STOP, LOW);

  digitalWriteDirect(IND_EX_1, LOW);
  digitalWriteDirect(IND_EX_2, LOW);
  digitalWriteDirect(IND_EX_3, LOW);

}

void ind_low()
{
  //reset all indicator pins
  digitalWriteDirect(IND_SWITCH, LOW);
  digitalWriteDirect(IND_STIM, LOW);
  digitalWriteDirect(IND_FREQ, LOW);
  digitalWriteDirect(IND_START, LOW);
  digitalWriteDirect(IND_STOP, LOW);
  digitalWriteDirect(IND_EX_1, LOW);
  digitalWriteDirect(IND_EX_2, LOW);
  digitalWriteDirect(IND_EX_3, LOW);

}

void ind_high()
{
  //reset all indicator pins
  digitalWriteDirect(IND_SWITCH, HIGH);
  digitalWriteDirect(IND_STIM, HIGH);
  digitalWriteDirect(IND_FREQ, HIGH);
  digitalWriteDirect(IND_START, HIGH);
  digitalWriteDirect(IND_STOP, HIGH);
  digitalWriteDirect(IND_EX_1, HIGH);
  digitalWriteDirect(IND_EX_2, HIGH);
  digitalWriteDirect(IND_EX_3, HIGH);
}
