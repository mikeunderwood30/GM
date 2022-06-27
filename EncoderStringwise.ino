
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
rhcStrItem rhcStr[NUM_GTR_STRINGS];

//const int ResetRHC0 = 14;	// p15.
//const int ResetRHC1 = 15;	// p16.
//const int ResetRHC2 = 0;	// pin TBD
//const int ResetRHC3 = 0;	// pin TBD

// ------------------------------------ GM Stringwise1 encoder ---------------------------
int currPressed[NUM_GTR_STRINGS];
//int lastPressed;	// mono mode uses this. It's the last pressed, no matter which string.

int candidate[NUM_GTR_STRINGS];
int debounceTimer[NUM_GTR_STRINGS];
int pitchOffsetStringwise[NUM_GTR_STRINGS];
int channelStringwise[NUM_GTR_STRINGS];
bool sustainStringwise[NUM_GTR_STRINGS];

rhcNoteDurationItem noteBuffer[NOTE_BUFFER_SIZE];

int noteDurationFromPot;
byte monoCurrPitch;

// ************************** InitEncoderStringwise() ****************************
void InitEncoderStringwise()
{ 
	pitchOffsetStringwise[3] = 38;
	pitchOffsetStringwise[2] = 43;
	pitchOffsetStringwise[1] = 48;
	pitchOffsetStringwise[0] = 53;

	channelStringwise[0] = 8;
	channelStringwise[1] = 9;
	channelStringwise[2] = 10;
	channelStringwise[3] = 11;

	//rhcDebounce[0].aToDNum = A0;
	//rhcDebounce[1].aToDNum = A1;
	//rhcDebounce[2].aToDNum = A2;
	//rhcDebounce[3].aToDNum = A3;

	for (byte ss = 0; ss < NUM_GTR_STRINGS; ss++)
	{
		currPressed[ss] = -1;  // nothing pressed initially
		candidate[ss] = -1;  // nothing pressed initially
		sustainStringwise[ss] = false;
	}
}
// ***************************** scanStrStringwise() *************************************
void scanStrStringwise(int ss)
{
/*
	Scan top-to-bottom for each string, and stop when we find one pressed, since we observe a 'highest fretted note wins' rule per string.
  Legato mode is supported by sending the noteOn for new note before the noteOff for the old note.
*/
	//boolean anyPressed;
	//boolean currPressedStillPressed;
  
	// if this string was pressed last time we checked, see if the same note is still pressed.
	//outputGmCount(gmMapByString[ss][currPressedStringwise[ss]]);
	//delay(1);
	//currPressedStillPressed = (digitalRead(strobePin) == HIGH);
	
	//anyPressed = false;

	//Serial.print("Scan string ");
	//Serial.println(ss);
	
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

      //delay(1000);
			
			// this will be the case while a note is held and no other pressed above it.
			if (ff == currPressed[ss])
			{	// do nothing. We already know this fret is pressed.
				//Serial.print(".");
			}	
			else
			{
				// A new keypress has been detected.
				// turn off the lower fret if player has fingered something higher.
				//if (currPressed[ss] != -1)
				////{
					//sendChannelMsg(0x80, currPressed[ss] + pitchOffsetStringwise[ss], 0x7f);
					//Serial.println("Send note off");
				//}
				currPressed[ss] = ff;
			}
			// No need to check frets below this one.
			break;
		}
		else	// fret is found not to be pressed
		{
			// 
			if (ff == currPressed[ss])
			{
				//currPressed[ss] = -1;
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
			byte pitch = currPressed[ss] + pitchOffsetStringwise[ss];

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
