/*Stuff for communication with the current source(CS), as well as incrementing channel and frequency during injections  */

void CS_next_chn() // setup next channel for multi frequency injection
{
	//Serial.println("Changing channel, iPrt is CURRENTLY"); //debug info
	//Serial.println(iPrt);


	//increment prt counter
	iPrt++;
	if (iPrt == NumInj) // if complete protocol done then reset and increment repetiton counter
	{
		iPrt = 0;
		iRep++;
		//CompProcessMulti();// send compliance status to PC
		//CompStatusReset();

		if (iRep != NumRep)
		{
			indpins_pulse(0, 0, 1, 0); // Pulse for complete protocol - this is to make a double pulse to indicate complete protocol during processing 
		}
	}

	if (iRep != NumRep) // if we arent finished then shuffle and send new info to pc
	{
		//shuffle freq order
		shuffle(FreqOrder, NumFreq);
		//		reset frequency counter
		iFreq = 0;
		//send info to PC
		PC_sendupdate();
		indpins_pulse(0, 0, 1, 0); // indicate we are changing prt now
	}
}

void CS_next_freq() // set up next frequency of injection
{
	/*long tstart = 0;
	long tset = 0;
	long tdisp = 0;
	long tsw = 0;
	long ttot = 0;
	*/

	//we are not doing this now
	//CS_stop(); // stop current source - changing amp and freq may mean we set current too high for the new/old frequency

	indpins_pulse(0, 0, 0, 1); // indicate that this freq inj is done - THIS CAN SOMETIMES HAPPEN *AFTER* THE SWITCHCHN PULSE BELOW!

	//tstart = micros();
	if (iRep != NumRep) // check if we arent finished - do nothing if we are finished (this is to prevent an eroneous short injection at the end
	{
		//Serial.println("changing frequency"); //debug info

		curFreqIdx = FreqOrder[iFreq]; // get the new frequency from the shuffled freqorder array

		if (StimMode) //initialise stimulator trigger if we are in stim mode
		{
			stim_init(Freq[curFreqIdx]);
			Stimflag = 1;
		}

		//Serial.print("Previous Freq was ");
		//Serial.println(prevFreq);
		//Serial.print("The Next Freq is: ");
		//Serial.println(Freq[curFreqIdx]);
		//Serial.println(iFreq);
		/*
		sprintf(PC_outputBuffer, "Setting Amp %duA and Freq %dHz", Amp[curFreqIdx], Freq[curFreqIdx]);
		Serial.println(PC_outputBuffer); */

		singleTone(Freq[curFreqIdx]);
		updateAD9954();

		//tset = micros();

		/*Serial.print("Channels I am about to program: ");
		Serial.print(Injection[iPrt][0]);
		Serial.print(" and ");
		Serial.println(Injection[iPrt][1]);*/


		/*#################
		Switching stuff moved to *after* CS start to limit the amount of time wasted waiting for CS to actually start in delay routine below
		*/
		StartTime_CS = micros();

		programswitches(Injection[iPrt][0], Injection[iPrt][1], TotalPins); //programme the switches
		SwitchChn(); // open switches to CS


		int DelayBeforeSwitch = StartDelay_MultiFreq;

		iFreq++; //increment the frequency counter (as all has gone well)

		prevFreq = Freq[curFreqIdx]; //store the value we just set for future comparison

		// delay the start of injection to give the current source time to get ready
		StartElapsed_CS = micros() - StartTime_CS;

		if (StartElapsed_CS < (DelayBeforeSwitch - 10))
		{
			delayMicroseconds(DelayBeforeSwitch - StartElapsed_CS);
		}

		lastFreqSwitch = micros(); // record time we switched freq
		
		indpins_pulse(0, 0, 0, curFreqIdx + 2); // send new freq pulse - equal to the freq number (extra one because zero ind) - so we can check this is processing - and extra one so the DIFF of the pulses is the freq order, this makes processing way easier

	}
	/*ttot = micros();
	sprintf(PC_outputBuffer, "Timing: %d set, %d disp, %d tsw, %d tot", tset - tstart, tdisp - tstart, tsw - tstart, ttot - tstart);
	Serial.println(PC_outputBuffer);
	*/
}
