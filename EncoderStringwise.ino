
/*	Stringwise1 encoder.
	Within each virtual string, scan top-to-bottom. Highest held note takes precedence.
*/

#include "Encoder.h"

void outputGmCount(byte);
void ExecutePreset(int);

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
lhcBasicItem lhEncodeBasic[NUM_GTR_STRINGS];
lhEncodeSwItem lhEncodeSw[NUM_GTR_STRINGS];

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

		lhEncodeBasic[ii].encMode = ENC_MODE_STRINGWISE_ORGAN;
		lhEncodeBasic[ii].currFret = -1;	// init to 'open'
		lhEncodeBasic[ii].changed = false;

		lhEncodeSw[ii].currFret = -1;
		lhEncodeSw[ii].changed = false;

		rhcStr[ii].isPressed = false;
	}
}
// ***************************** EncodePreprocess() *************************************
// assumes that scanBasic() was run prior
void EncodePreprocess(int ss)
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
}
// ***************************** EncodeStringwise() *************************************
// assumes that EncodePreprocess() was run prior
void EncodeStringwise(int ss)
{
	if (!rhcStr[ss].isPressed)		// if it wasn't pressed,
	{
		// Serial.print(".");	
		if (RhcCurrentlyPressed(ss))		// and now it is,
		{	
			// Serial.println("now it is");	
			rhcStr[ss].isPressed = true;	// so it only does this once
			lhEncodeSw[ss].msgPitch = lhEncodeSw[ss].currFret + lhEncodeSw[ss].pitchOffset;
			
			noteOn(0, lhEncodeSw[ss].msgPitch, 64);   // Channel, pitch, velocity
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

			noteOff(0, lhEncodeSw[ss].msgPitch, 64); 	// Channel, pitch, velocity
			MidiUSB.flush();
		}
		else	// still is
		{
			// Serial.println("still is");	
			// if LH has changed
			if (lhEncodeSw[ss].changed)
			{	
				lhEncodeSw[ss].changed = false;	// so does this once only

				// send a noteOff for the fret that is sounding
				noteOff(0, lhEncodeSw[ss].msgPitch, 64); 	// Channel, pitch, velocity
				MidiUSB.flush();

				// calc the the new fret value and store it so a noteOff can be sent
				lhEncodeSw[ss].msgPitch = lhEncodeSw[ss].currFret + lhEncodeSw[ss].pitchOffset;
				// send a noteOn for the new fret
				noteOn(0, lhEncodeSw[ss].msgPitch, 64);   // Channel, pitch, velocity
				MidiUSB.flush();
			}
		}
	}
}
// ***************************** RhcCurrentlyPressed() *************************************
// called from EncodeStringwise
bool RhcCurrentlyPressed(int ss)
{
	if (lhEncodeBasic[ss].encMode == ENC_MODE_STRINGWISE_INT)
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
// since we skip the PreProcess() step, have to use lhEncodeBasic[].currFret not lhEncodeSw[].currFret
void EncodeStringwiseOrgan(int ss)
{
	if (lhEncodeBasic[ss].changed)
	{
		if (lhEncodeBasic[ss].currFret != -1)		// ignore if 'open'
		{
			lhEncodeSw[ss].currFret = lhEncodeBasic[ss].currFret;

			if (lhEncodeSw[ss].msgPitch != 0)
			{
				noteOff(0, lhEncodeSw[ss].msgPitch, 64); 	// Channel, pitch, velocity
				MidiUSB.flush();
			}

			// calc the the new fret value and store it so a noteOff can be sent
			lhEncodeSw[ss].msgPitch = lhEncodeSw[ss].currFret + lhEncodeSw[ss].pitchOffset;
			// send a noteOn for the new fret
			noteOn(0, lhEncodeSw[ss].msgPitch, 64);   // Channel, pitch, velocity
			MidiUSB.flush();
		}

		lhEncodeBasic[ss].changed = false;
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
// ***************************** EncodeAutochord() *************************************
// In this AC mode, notes are 'always on'. User needs to have a means of manually controlling the volume, such as a pedal.
void EncodeAutochord(int ss)
{
	if (lhEncodeBasic[ss].changed && lhEncodeBasic[ss].currFret != -1)
	{
		int index = lhEncodeBasic[ss].currFret;
    Serial.print("New AC chord. Index = ");
    Serial.println(index);
      
		if (index < AC_NUM_CHORDS)
		{
			// send note offs for any existing chord
			// look up notes based on index.
			// send noteOns for these notes.
			// store them so can send noteOffs later. 
		}

		lhEncodeBasic[ss].changed = false;
	}
}
