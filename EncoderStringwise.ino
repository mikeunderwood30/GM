
/*	Stringwise1 encoder.
	Within each virtual string, scan top-to-bottom. Highest held note takes precedence.
*/

#include "Encoder.h"

void outputGmCount(byte count);

int gmMapByString[4][16] = {
61, 59, 63, 57, 62, 56, 58, 60, 21, 19, 23, 17, 22, 16, 18, 20,
45, 43, 47, 41, 46, 40, 42, 44, 37, 35, 39, 33, 38, 32, 34, 36,
13, 11, 15, 9, 14, 8, 10, 12, 53, 51, 55, 49, 54, 48, 50, 52,
29, 27, 31, 25, 30, 24, 26, 28, 5, 3, 7, 1, 6, 0, 2, 4
};

// define pins that select the count. Pin numbers here are for the Sparkfun Pro Micro Qwiic.
const int D0_pin = 2;		// p5. Mega p27
const int D1_pin = 3;		// p6. Mega p26
const int D2_pin = 4;		// p7. Mega p25
const int D3_pin = 5;		// p8. Mega p24
const int D4_pin = 6;		// p9. Mega p23
const int D5_pin = 7;		// p10. Mega p22

const int StrobeLHC = 8;	// p11. Mega p28.
const int EncodeOverrideButton = 9;	// p12

// ------------------------------------ Encoder data ---------------------------

rhcStrItem rhcStr[NUM_GTR_STRINGS];
lhcBasicItem lhEncodeBasic[NUM_GTR_STRINGS];
lhEncodeSwItem lhEncodeSw[NUM_GTR_STRINGS];

// ------------------------------------ Poly Encoder ---------------------------

int pitchOffsetStringwise[NUM_GTR_STRINGS];
int channelStringwise[NUM_GTR_STRINGS];
//bool sustainStringwise[NUM_GTR_STRINGS];

//rhcNoteDurationItem noteBuffer[NOTE_BUFFER_SIZE];

int noteDurationFromPot;
byte monoCurrPitch;

// ************************** InitEncoders() ****************************
void InitEncoders()
{ 
	pitchOffsetStringwise[3] = 38;
	pitchOffsetStringwise[2] = 43;
	pitchOffsetStringwise[1] = 48;
	pitchOffsetStringwise[0] = 53;

	channelStringwise[0] = 8;
	channelStringwise[1] = 9;
	channelStringwise[2] = 10;
	channelStringwise[3] = 11;

	for (byte ss = 0; ss < NUM_GTR_STRINGS; ss++)
	{
		lhEncodeBasic[ss].currFret = -1;	// open
		lhEncodeSw[ss].currFret = -1;
		lhEncodeBasic[ss].changed = false;
	}
}
// ***************************** EncodeStringwise() *************************************
void EncodeStringwise(int ss)
{
	if (lhEncodeBasic[ss].changed)
	{
		// Serial.print(lhEncodeBasic[ss].currFret);
		// Serial.print(", ");
		// Serial.println(lhEncodeSw[ss].currFret);

		if (lhEncodeBasic[ss].currFret != -1)		// ignore if 'open'
		{
			if (lhEncodeSw[ss].currFret != lhEncodeBasic[ss].currFret)
			{
				lhEncodeSw[ss].currFret = lhEncodeBasic[ss].currFret;

				// This 'changed' flag is only for the case where something changes while RHC is pressed.
				// If we set it even when RHC is not pressed, we'll get an extra off/on when it does get pressed.
				if (rhcStr[ss].isPressed)
				{
					lhEncodeSw[ss].changed = true;
				}

				// Serial.print("Current fret is now ");	
				// Serial.println(lhEncodeSw[ss].currFret);
			}
		}

		lhEncodeBasic[ss].changed = false;
	}

	if (!rhcStr[ss].isPressed)		// if it wasn't pressed,
	{
		if (digitalRead(rhcStr[ss].pinNumber))		// and now it is,
		{	
			rhcStr[ss].isPressed = true;	// so it only does this once
			lhEncodeSw[ss].msgPitch = lhEncodeSw[ss].currFret + pitchOffsetStringwise[ss];
			
			noteOn(0, lhEncodeSw[ss].msgPitch, 64);   // Channel, pitch, velocity
			MidiUSB.flush();	
		}
	}
	else	// if it was pressed,
	{
		if (!digitalRead(rhcStr[ss].pinNumber))		// no longer is
		{	
			rhcStr[ss].isPressed = false;

			noteOff(0, lhEncodeSw[ss].msgPitch, 64); 	// Channel, pitch, velocity
			MidiUSB.flush();
		}
		else	// still is
		{
			// if LH has changed
			if (lhEncodeSw[ss].changed)
			{	
				lhEncodeSw[ss].changed = false;	// so does this once only

				// send a noteOff for the fret that is sounding
				noteOff(0, lhEncodeSw[ss].msgPitch, 64); 	// Channel, pitch, velocity
				MidiUSB.flush();

				// calc the the new fret value and store it so a noteOff can be sent
				lhEncodeSw[ss].msgPitch = lhEncodeSw[ss].currFret + pitchOffsetStringwise[ss];
				// send a noteOn for the new fret
				noteOn(0, lhEncodeSw[ss].msgPitch, 64);   // Channel, pitch, velocity
				MidiUSB.flush();
			}
		}
	}
}

// ***************************** EncodePresetSelect() *************************************
void EncodePresetSelect(int str)
{
	int preset;

	// ignore ss. Scan all 4 strings every time this is called.
	for (int ss = 0; ss < NUM_GTR_STRINGS; ss++)
	{
		if (lhEncodeBasic[ss].changed && lhEncodeBasic[ss].currFret != -1)
		{
			preset = ss * 16 + lhEncodeBasic[ss].currFret;

			Serial.print("preset ");
			Serial.println(preset);	

			switch (preset)
			{
				case 0:
					//encModeBackup[ss]
					break;

				case 1:
					break;

				case 2:
					break;

				default:
					break;
			}

			lhEncodeBasic[ss].changed = false;
		}
	}
}

// ***************************** scanBasic() *************************************
// This scanner is common to all encoders.
// For each string, scan and determine what is pressed. If something has changed, set a flag.
// Scan top-to-bottom for each string, and stop when we find one pressed, since we observe
// a 'highest fretted note wins' rule per string.
void scanBasic()
{
	for (byte ss = 0; ss < NUM_GTR_STRINGS; ss++)
	{
		for (int ff = MAX_FRETS-1; ff >= -1; ff--)
		{
			if (ff == -1)
			{
				if (ff != lhEncodeBasic[ss].currFret)
				{
					lhEncodeBasic[ss].changed = true;
					lhEncodeBasic[ss].currFret = ff;
				}

				break;
			}

			//Serial.print("outputting count on string ");
			//Serial.print(ss);
			//Serial.print(", fret ");
			//Serial.println(ff);

			outputGmCount(gmMapByString[ss][ff]);
			//delay(3000);

			//check whether this fret seems to be pressed.
			if (digitalRead(StrobeLHC) == LOW)
			{		
				//Serial.print("detected key pressed on string ");
				//Serial.print(ss);
				//Serial.print(", fret ");
				//Serial.println(ff);
		
				//Serial.print("Count = ");
				//Serial.println(gmMapByString[ss][ff]);
				
				if (ff != lhEncodeBasic[ss].currFret)
				{
					lhEncodeBasic[ss].changed = true;
					lhEncodeBasic[ss].currFret = ff;
				}
				// No need to check frets below this one.
				break;
			}
		}
	}
}
/*
// ***************************** scanStrStringwise() *************************************
void scanStrStringwise(int ss)
{
	byte pitch;
	
	for (int ff = MAX_FRETS-1; ff >= 0; ff--)
	{
			//Serial.print("outputting count on string ");
			//Serial.print(ss);
			//Serial.print(", fret ");
			//Serial.println(ff);

		outputGmCount(gmMapByString[ss][ff]);
		//delay(3000);

		//check whether this fret seems to be pressed.
		if (digitalRead(StrobeLHC) == LOW)
		{		
			//Serial.print("detected key pressed on string ");
			//Serial.print(ss);
			//Serial.print(", fret ");
			//Serial.println(ff);
	
			//Serial.print("Count = ");
			//Serial.println(gmMapByString[ss][ff]);

			monoCurrPitch = ff + pitchOffsetStringwise[ss];
			// Serial.print("monoCurrPitch = ");
			// Serial.println(monoCurrPitch);
			
			// this will be the case while a note is held and no other pressed above it.
			if (ff == currFret[ss])
			{	// do nothing. We already know this fret is pressed.
				//Serial.print(".");
			}	
			else
			{
				// fret has changed

				currFret[ss] = ff;

				// if a note is sounding, need to change it.
				if (rhcStr[ss].isPressed)
				{
					// send a noteOff for the fret that is sounding
					noteOff(0, rhcStr[ss].msgPitch, 64); 	// Channel, pitch, velocity
					MidiUSB.flush();

					// send a noteOn for the new fret
					pitch = currFret[ss] + pitchOffsetStringwise[ss];
					noteOn(0, pitch, 64);   // Channel, pitch, velocity
					MidiUSB.flush();

					// store the new fret value so a noteOff can be sent for it later.
					rhcStr[ss].msgPitch = pitch;
				}
			}
			// No need to check frets below this one.
			break;
		}
		else	// fret is found not to be pressed
		{
			// 
			if (ff == currFret[ss])
			{
				//currFret[ss] = -1;
				// This was pressed before, but now is not.
				//Serial.print("-");
				//Serial.print(ss);
				//Serial.print(",");
				//Serial.println(ff);
			}
		}
	}

	// RHC
	if (!rhcStr[ss].isPressed)
	{
		if (digitalRead(rhcStr[ss].pinNumber))
		{
			rhcStr[ss].isPressed = true;
			pitch = currFret[ss] + pitchOffsetStringwise[ss];

			// Serial.print("Sending note on, pitch = ");	
			// Serial.println(pitch);
			
			noteOn(0, pitch, 64);   // Channel, pitch, velocity
			MidiUSB.flush();

			rhcStr[ss].msgPitch = pitch;

			//Serial.print("Calling AddNoteToTimerPool(). duration = ");  
			//Serial.println(noteDurationFromPot);
			//AddNoteToTimerPool(noteDurationFromPot, pitch);
		}
	}
	else
	{
		if (!digitalRead(rhcStr[ss].pinNumber))
		{
			rhcStr[ss].isPressed = false;
			// Serial.print("Sending note off, pitch = ");	
			// Serial.println(rhcStr[ss].msgPitch);

			noteOff(0, rhcStr[ss].msgPitch, 64); 	// Channel, pitch, velocity
			MidiUSB.flush();
		}
	}
}
*/
