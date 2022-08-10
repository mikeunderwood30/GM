
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
	for (int ii = 0; ii < NUM_GTR_STRINGS; ii++)
	{
		//pinMode(rhcStr[ii].pinNumber, INPUT);
		//digitalWrite(rhcStr[ii].pinNumber, HIGH);       // turn on pullup resistor

		lhEncode[ii].encMode = ENC_MODE_STRINGWISE_INT;    //   ENC_MODE_STRINGWISE_ORGAN  ENC_MODE_STRINGWISE_INT ENC_MODE_GATED_AUTO_RHC
		lhEncode[ii].currFret = -1;
		lhEncode[ii].isOpen = false;
		lhEncode[ii].changed = false;

		rhcStr[ii].rhcActive = false;
	}
}
// ***************************** EncodeStringwiseLhc() *************************************
// This part of the encoder handles whether the LH has changed. It is only called while
// RHC is pressed.
// Supports the following encoder modes: ENC_MODE_STRINGWISE_INT, ENC_MODE_STRINGWISE_EXT.
// These require both left and right hand actions.
void EncodeStringwiseLhc(int ss)
{
	// if LH has changed
	if (lhEncode[ss].changed)
	{	
		lhEncode[ss].changed = false;	// so does this once only

		if (!lhEncode[ss].isOpen)		// don't change if 'open'
		{
      		Serial.println("LH has changed while RHC is pressed");
        
			// send a noteOff for the fret that is sounding
			noteOff(0, lhEncode[ss].msgPitch, 64); 	// Channel, pitch, velocity
			MidiUSB.flush();

			//Serial.print("Sliding. Sent noteOff. msgPitch = ");
			//Serial.println(lhEncode[ss].msgPitch);

			// calc the the new fret value and store it so a noteOff can be sent
			lhEncode[ss].msgPitch = lhEncode[ss].currFret + lhEncode[ss].pitchOffset;
			// send a noteOn for the new fret
			noteOn(0, lhEncode[ss].msgPitch, 64);   // Channel, pitch, velocity
			MidiUSB.flush();

			// Serial.print("End of slide. New note noteOn. msgPitch = ");
			// Serial.println(lhEncode[ss].msgPitch);
		}
	}
}
// ***************************** EncodeStringwiseRhc() *************************************
// Called because a Trigger has fired.
void EncodeStringwiseRhc(bool pressRelease, int ss)
{
	// .rhcActive tracks whether the RHC on a given string is currently pressed or released.
	// This info is needed by some encoders. For example, on some encoders, if user changes
	// which fret is pressed while the RHC is pressed, we send a noteOff for
	//  the old note and a noteOn for the new note. We don't do this if the RHC is not pressed. 
	rhcStr[ss].rhcActive = pressRelease;

//	Serial.print("EncodeStringwiseRhc(). ss, pressRelease, currFret = ");
//  	Serial.print(ss);
//   Serial.print(",");
//	Serial.print(pressRelease); 
 // Serial.print(",");
//	Serial.print(lhEncode[ss].currFret);
//	Serial.println();  

	if (lhEncode[ss].currFret == -1)
	{
		// this encoder produces no sound if plucked while string is open. 
		return;
	}

	if (pressRelease)
	{
		lhEncode[ss].msgPitch = lhEncode[ss].currFret + lhEncode[ss].pitchOffset;
		
		noteOn(0, lhEncode[ss].msgPitch, 64);   // Channel, pitch, velocity
		MidiUSB.flush();

		// in the case where we get an RHC press while a fret is being pressed,
		// this prevents EncodeStringwiseLhc() from doing an unneeded noteOff/noteOn sequence.
		lhEncode[ss].changed = false;

//    Serial.print("Sent noteOn. msgPitch = ");
//   Serial.println(lhEncode[ss].msgPitch);  
	}
	else
	{
		//if (lhEncode[ss].msgPitch != -1)
		//{
			noteOff(0, lhEncode[ss].msgPitch, 64); 	// Channel, pitch, velocity
			MidiUSB.flush();

    // Serial.print("Sent noteOff. msgPitch = ");
    // Serial.println(lhEncode[ss].msgPitch); 
		//}
		//else{
		//	Serial.println("msgPitch was -1 so note off was not sent");	
		//}
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
					// note: don't set currFret to -1.
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
		//if (digitalRead(rhcStr[ss].pinNumber))
		{
			// retrieve the current LH value and send a timed msg.
			byte pitch = lhEncode[ss].currFret + lhEncode[ss].pitchOffset;
			AddNoteToTimerPool(200, pitch);
			noteOn(0, pitch, 64);   // Channel, pitch, velocity
			MidiUSB.flush();
		}
	}
}
// ***************************** EncodeGatedAutoRHC() *************************************
// void EncodeGatedAutoRHC(int ss)
// {
// }
