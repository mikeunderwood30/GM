
#include "Common.h"
#include "AnalogControl.h"

AtoDItem AtoD[NUM_AtoD_CONTROLS];

// ************************** InitAnalogControls() ****************************
void InitAnalogControls()
{ 
	AtoD[0].atoDNum = A0;
	AtoD[0].channel = 1;
	AtoD[0].ccNum = 5;

	AtoD[1].atoDNum = A1;
	AtoD[1].channel = 2;
	AtoD[1].ccNum = 6;

	AtoD[2].atoDNum = A2;
	AtoD[2].channel = 3;
	AtoD[2].ccNum = 7;

	AtoD[3].atoDNum = A3;
	AtoD[3].channel = 4;
	AtoD[3].ccNum = 8;
}
// ************************** CheckAtoDControls() ****************************
void CheckAtoDControls()
{ 
    // check A-to-D values for on-board controls, such as pots, etc
	for (int ii = 0; ii < NUM_AtoD_CONTROLS; ii++)
	{
		int adcValue = analogRead(AtoD[ii].atoDNum);
		adcValue = adcValue >> 3;	// 0-1024 range to 0-127
		if (adcValue > 127)	// should never be, but just in case.
		adcValue = 127;

		if (adcValue != AtoD[ii].currAdcValue)
		{
			AtoD[ii].currAdcValue = adcValue;

			// send MIDI CC msg
			controlChange(AtoD[ii].channel, AtoD[ii].ccNum, AtoD[ii].currAdcValue);
		}
	}
}
