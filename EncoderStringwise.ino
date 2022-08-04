
/*	Stringwise1 encoder.
	Within each virtual string, scan top-to-bottom. Highest held note takes precedence.
*/

#include "Encoder.h"

void outputGmCount(byte);
//void ExecutePreset(int);

int gmMapByString[4][16] = {
61, 59, 63, 57, 62, 56, 58, 60, 21, 19, 23, 17, 22, 16, 18, 20,
45, 43, 47, 41, 46, 40, 42, 44, 37, 35, 39, 33, 38, 32, 34, 36,
13, 11, 15, 9, 14, 8, 10, 12, 53, 51, 55, 49, 54, 48, 50, 52,
29, 27, 31, 25, 30, 24, 26, 28, 5, 3, 7, 1, 6, 0, 2, 4
};

byte acChord[AC_NUM_CHORDS][AC_NOTES_PER_CHORD] = {
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0
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
//lhcBasicItem lhEncodeBasic[NUM_GTR_STRINGS];
lhEncodeItem lhEncode[NUM_GTR_STRINGS];

int noteDurationFromPot;
byte monoCurrPitch;

// ************************** InitEncoders() ****************************
void InitEncoders()
{
	rhcStr[0].pinNumber = 15;
	rhcStr[1].pinNumber = 16;	
	rhcStr[2].pinNumber = 10;	
	rhcStr[3].pinNumber = 14;	

	for (int ii = 0; ii < NUM_GTR_STRINGS; ii++)
	{
		pinMode(rhcStr[ii].pinNumber, INPUT);
		digitalWrite(rhcStr[ii].pinNumber, HIGH);       // turn on pullup resistor

		lhEncode[ii].encMode = ENC_MODE_STRINGWISE_ORGAN;    //   ENC_MODE_STRINGWISE_ORGAN  ENC_MODE_STRINGWISE_INT ENC_MODE_GATED_AUTO_RHC
		lhEncode[ii].currFret = 0;
		lhEncode[ii].isOpen = false;
		lhEncode[ii].changed = false;

		//lhEncode[ii].currFret = -1;
		//lhEncode[ii].changed = false;

		rhcStr[ii].isPressed = false;
	}
}
// ***************************** EncodeStringwise() *************************************
// Supports the following encoder modes: ENC_MODE_STRINGWISE_INT, ENC_MODE_STRINGWISE_EXT.
// These require both left and right hand actions.
void EncodeStringwise(int ss)
{
	if (!rhcStr[ss].isPressed)		// if RHC wasn't pressed,
	{
		if (RhcCurrentlyPressed(ss))		// and now it is,
		{	
			rhcStr[ss].isPressed = true;	// so it only does this once

			lhEncode[ss].msgPitch = lhEncode[ss].currFret + lhEncode[ss].pitchOffset;
			
			noteOn(0, lhEncode[ss].msgPitch, 64);   // Channel, pitch, velocity
			MidiUSB.flush();
		}
	}
	else	// if it was pressed,
	{
		// Serial.println("was pressed");
		if (!RhcCurrentlyPressed(ss))		// no longer is
		{	
			// Serial.println("no longer");	
			rhcStr[ss].isPressed = false;

			if (lhEncode[ss].msgPitch != 0)
			{
				noteOff(0, lhEncode[ss].msgPitch, 64); 	// Channel, pitch, velocity
				MidiUSB.flush();
			}
			else{
				Serial.println("msgPitch was 0 so note off not sent");	
			}
		}
		else	// still is
		{
			// Serial.println("still is");	
			// if LH has changed
			if (lhEncode[ss].changed)
			{	
				lhEncode[ss].changed = false;	// so does this once only

				if (!lhEncode[ss].isOpen)		// don't change if 'open'
				{
					// send a noteOff for the fret that is sounding
					noteOff(0, lhEncode[ss].msgPitch, 64); 	// Channel, pitch, velocity
					MidiUSB.flush();

					// calc the the new fret value and store it so a noteOff can be sent
					lhEncode[ss].msgPitch = lhEncode[ss].currFret + lhEncode[ss].pitchOffset;
					// send a noteOn for the new fret
					noteOn(0, lhEncode[ss].msgPitch, 64);   // Channel, pitch, velocity
					MidiUSB.flush();
				}
			}
		}
	}
}
// ***************************** RhcCurrentlyPressed() *************************************
// called from EncodeStringwise
bool RhcCurrentlyPressed(int ss)
{
	if (lhEncode[ss].encMode == ENC_MODE_STRINGWISE_INT)
	{
		// ternary operator
		return(digitalRead(rhcStr[ss].pinNumber) ? true : false);
	}
	else	// must be ENC_MODE_STRINGWISE_EXT
	{
		return(rhcStr[ss].inNoteOn ? true : false);
	}
}
// ***************************** EncodeStringwiseOrgan() *************************************
// Supports the following encoder modes: ENC_MODE_STRINGWISE_ORGAN.
void EncodeStringwiseOrgan(int ss)
{
	if (lhEncode[ss].changed)
	{
		if (!lhEncode[ss].isOpen)		// ignore if 'open'
		{
			byte pitch = lhEncode[ss].currFret + lhEncode[ss].pitchOffset;
			AddNoteToTimerPool(200, pitch);
			noteOn(0, pitch, 64);   // Channel, pitch, velocity
			MidiUSB.flush();
		}

		lhEncode[ss].changed = false;
	}
}
// ***************************** EncodeAutochord() *************************************
// In this AC mode, notes are 'always on'. User needs to have a means of manually controlling the volume, such as a pedal.
void EncodeAutochord(int ss)
{
	if (lhEncode[ss].changed && lhEncode[ss].currFret != -1)
	{
		int index = lhEncode[ss].currFret;
    Serial.print("New AC chord. Index = ");
    Serial.println(index);
      
		if (index < AC_NUM_CHORDS)
		{
			// send note offs for any existing chord
			// look up notes based on index.
			// send noteOns for these notes.
			// store them so can send noteOffs later. 
		}

		lhEncode[ss].changed = false;
	}
}
// ******************************************** Support Methods *********************************************

// ***************************** scanBasic() *************************************
// Called from loop(). Since it is common to all encoders, it is called regardless of which encoder mode is in effect.
// For each string, scan and determine what is pressed. If something has changed, set a flag.
// Scan top-to-bottom for each string, and stop when we find one pressed. The 'highest fretted note wins' rule applies.
void scanBasic()
{
	for (byte ss = 0; ss < NUM_GTR_STRINGS; ss++)
	{
		for (int ff = MAX_FRETS-1; ff >= -1; ff--)
		{
			if (ff == -1)
			{
				// Nothing was pressed. String is open. But if it was pressed before, we still need to 
				// record the change.
				if (ff != lhEncode[ss].currFret)
				{
					lhEncode[ss].changed = true;
					lhEncode[ss].isOpen = true;
				}

				break;
			}

			//Serial.print("outputting count on string ");
			//Serial.print(ss);
			//Serial.print(", fret ");
			//Serial.println(ff);

			outputGmCount(gmMapByString[ss][ff]);
			//delay(3000);

			// At this point, ff has not reached -1.
			// Check whether this fret seems to be pressed.
			if (digitalRead(StrobeLHC) == LOW)
			{		
				//Serial.print("detected key pressed on string ");
				//Serial.print(ss);
				//Serial.print(", fret ");
				//Serial.println(ff);
		
				//Serial.print("Count = ");
				//Serial.println(gmMapByString[ss][ff]);
				
				if (ff != lhEncode[ss].currFret)
				{
					lhEncode[ss].changed = true;
					lhEncode[ss].isOpen = false;
					lhEncode[ss].currFret = ff;
				}
				// No need to check frets below this one.
				break;
			}
		}
	}
}
// ***************************** PickGatedStrings() *************************************
// Called from loop(), where we read MIDI in. 
// Called every 16th note when we are receiving MIDI clks.
void PickGatedStrings()
{
	for (int ss = 0; ss < NUM_GTR_STRINGS; ss++)
	{
		// if gated by RH
		if (digitalRead(rhcStr[ss].pinNumber))
		{
			// retrieve the current LH value and send a timed msg.
			byte pitch = lhEncode[ss].currFret + lhEncode[ss].pitchOffset;
			AddNoteToTimerPool(200, pitch);
			noteOn(0, pitch, 64);   // Channel, pitch, velocity
			MidiUSB.flush();
		}
	}
}
// ***************************** SetInNoteOnFlag() *************************************
// Called from loop(), where we read MIDI in. When we get a noteOn having a certain pitch, we set the .inNoteOn flag
// for the associated string, so the ENC_MODE_STRINGWISE_EXT encoder can use it (if the string happens to be in that enc mode).
void SetInNoteOnFlag(byte pitchByte, bool isPressed)
{
	switch (pitchByte)
	{
		case 0x40:
			rhcStr[0].inNoteOn = isPressed;
			break;

		case 0x41:
			rhcStr[1].inNoteOn = isPressed;
			break;

		case 0x42:
			rhcStr[2].inNoteOn = isPressed;
			break;

		case 0x43:
			rhcStr[3].inNoteOn = isPressed;
			break;
	}
}
// ***************************** EncodeGatedAutoRHC() *************************************
// void EncodeGatedAutoRHC(int ss)
// {
// }