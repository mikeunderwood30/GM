
#include "Encoder.h"

// ************************** InitEncoderPresetSelect() ****************************
void InitEncoderPresetSelect()
{ 
	
}

// ***************************** EncodePresetSelect() *************************************
void EncodePresetSelect(int str)
{
	int preset;

	// str is not used. Scans all 4 strings every time this is called.
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


