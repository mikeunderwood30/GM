
#include "Encoder.h"

// ************************** InitEncoderPresetSelect() ****************************
void InitPresetSelect()
{ 
	
}

// ***************************** EncoderPresetSelect() *************************************
void EncoderPresetSelect(int str)
{
	// str is not used. Scans all 4 strings every time this is called.
	for (int ss = 0; ss < NUM_GTR_STRINGS; ss++)
	{
		if (lhEncodeBasic[ss].changed && lhEncodeBasic[ss].currFret != -1)
		{
			int presetNum = ss * 16 + lhEncodeBasic[ss].currFret;
			ExecutePreset(presetNum);

			lhEncodeBasic[ss].changed = false;
		}
	}
}
// ***************************** ExecutePreset() *************************************
void ExecutePreset(int preset)
{
	Serial.print("preset ");
	Serial.print(preset);	

	switch (preset)
	{
		case 0:
			// all strings 'stringwise' mode. Pitch: Lower range.
			SetEncoderMode(ENC_MODE_STRINGWISE, ENC_MODE_STRINGWISE, ENC_MODE_STRINGWISE, ENC_MODE_STRINGWISE);
			SetTuning(53, 48, 43, 38);

			break;

		case 1:
			// all strings 'stringwise' mode. Pitch: Up one octave from preset 0.
			SetEncoderMode(ENC_MODE_STRINGWISE, ENC_MODE_STRINGWISE, ENC_MODE_STRINGWISE, ENC_MODE_STRINGWISE);
			SetTuning(65, 60, 55, 50);
			break;

		case 2:
			// Same as 0 except string 3 is AC mode.
			SetEncoderMode(ENC_MODE_STRINGWISE, ENC_MODE_STRINGWISE, ENC_MODE_STRINGWISE, ENC_MODE_AUTOCHORD);
			SetTuning(53, 48, 43, 38);
			break;

		default:
			break;
	}
}
// ***************************** SetEncoderMode() *************************************
// sets encoder mode for all strings.
// sets it in the 'backup' because that's what will be used when it comes out of override mode.
void SetEncoderMode(int e0, int e1, int e2, int e3)
{
	lhEncodeBasic[0].encModeBackup = e0;
	lhEncodeBasic[1].encModeBackup = e1;
	lhEncodeBasic[2].encModeBackup = e2;
	lhEncodeBasic[3].encModeBackup = e3;
}
// ***************************** SetTuning() *************************************
// sets tuning for all strings.
void SetTuning(int e0, int e1, int e2, int e3)
{
	lhEncodeSw[0].pitchOffset = e0;
	lhEncodeSw[1].pitchOffset = e1;
	lhEncodeSw[2].pitchOffset = e2;
	lhEncodeSw[3].pitchOffset = e3;
}