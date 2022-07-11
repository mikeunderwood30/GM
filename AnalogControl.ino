
#include "Common.h"
#include "AnalogControl.h"

AtoDItem AtoD[NUM_AtoD_CONTROLS];

// ************************** InitAnalogControls() ****************************
void InitAnalogControls()
{ 
	
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

			if (ii != 3)	// ftse may not be connected
			{
				Serial.print("A to D #");
				Serial.print(ii);
				Serial.print(": ");
				Serial.println(AtoD[ii].currAdcValue);
			}
		}
	}
}
