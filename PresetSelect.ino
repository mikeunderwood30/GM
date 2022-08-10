
#include "Encoder.h"
#include "AnalogControl.h"
#include "Trigger.h"

// ************************** InitPresetSelect() ****************************
void InitPresetSelect()
{ 
	
}

// ***************************** EncoderPresetSelect() *************************************
void EncoderPresetSelect(int str)
{
	// str is not used. Scans all 4 strings every time this is called.
	for (int ss = 0; ss < NUM_GTR_STRINGS; ss++)
	{
		if (!lhEncode[ss].isOpen)
		{
			if (lhEncode[ss].changed && lhEncode[ss].currFret != -1)
			{
				int presetNum = ss * 16 + lhEncode[ss].currFret;
				ExecutePreset(presetNum);

				lhEncode[ss].changed = false;
			}
		}
	}
}
// ****************************** ExecutePreset() *************************************
void ExecutePreset(int preset)
{
	Serial.print("preset ");
	Serial.println(preset);	

	switch (preset)
	{
		case 0:
			SetEncoderMode(ENC_MODE_STRINGWISE_INT, ENC_MODE_STRINGWISE_INT, ENC_MODE_STRINGWISE_INT, ENC_MODE_STRINGWISE_INT);
			SetTuning(53, 48, 43, 38);

			trigger[0].evtType = TE_TYPE_BUTTON_PRESS;
			trigger[0].evtParm = 0; // which button
			trigger[0].action = TrigActn_RHC_PRESS;
			trigger[0].actionParm = 0; // which string
			trigger[0].isActive = true;

			trigger[1].evtType = TE_TYPE_BUTTON_RELEASE;
			trigger[1].evtParm = 0; // which button
			trigger[1].action = TrigActn_RHC_RELEASE;
			trigger[1].actionParm = 0; // which string
			trigger[1].isActive = true;
			break;

		case 1:
			SetEncoderMode(ENC_MODE_STRINGWISE_ORGAN, ENC_MODE_STRINGWISE_ORGAN, ENC_MODE_STRINGWISE_ORGAN, ENC_MODE_STRINGWISE_ORGAN);

			trigger[0].evtType = TE_TYPE_BUTTON_PRESS;
			trigger[0].evtParm = 0; // which button
			trigger[0].action = TrigActn_SendEvent;
			trigger[0].actionParm = 0; // which event
			trigger[0].isActive = true;

			trigger[1].evtType = TE_TYPE_BUTTON_RELEASE;
			trigger[1].evtParm = 0; // which button
			trigger[1].action = TrigActn_SendEvent;
			trigger[1].actionParm = 1; // which event
			trigger[1].isActive = true;
			break;

		// These are at the top of string 0
		case 14:	// octave down
			SetTuning(53, 48, 43, 38);
			break;

		case 15:	// octave up
			SetTuning(65, 60, 55, 50);
			break;

		// These are at the top of string 1
		case 30:
			SetEncoderMode(ENC_MODE_STRINGWISE_ORGAN, ENC_MODE_STRINGWISE_ORGAN, ENC_MODE_STRINGWISE_ORGAN, ENC_MODE_STRINGWISE_ORGAN);
			break;

		case 31:
			SetEncoderMode(ENC_MODE_GATED_AUTO_RHC, ENC_MODE_GATED_AUTO_RHC, ENC_MODE_GATED_AUTO_RHC, ENC_MODE_GATED_AUTO_RHC);
			break;

		default:
			break;
	}
}
// ***************************** SetEncoderMode() *************************************
// sets encoder mode for all strings. This is stored in the lhEncodeBasic[] structure for now.
// sets it in the 'backup' because that's what will be used when it comes out of override mode.
void SetEncoderMode(int e0, int e1, int e2, int e3)
{
	lhEncode[0].encModeBackup = e0;
	lhEncode[1].encModeBackup = e1;
	lhEncode[2].encModeBackup = e2;
	lhEncode[3].encModeBackup = e3;
}
// ***************************** SetTuning() *************************************
// sets tuning for all strings.
void SetTuning(int e0, int e1, int e2, int e3)
{
	lhEncode[0].pitchOffset = e0;
	lhEncode[1].pitchOffset = e1;
	lhEncode[2].pitchOffset = e2;
	lhEncode[3].pitchOffset = e3;
}
// ***************************** SetAtoDChannel() *************************************
// Set channel for every A to D. 
void SetAtoD_Channel(int p0, int p1, int p2, int p3)
{
	AtoD[0].channel = p0;
	AtoD[1].channel = p1;
	AtoD[2].channel = p2;
	AtoD[3].channel = p3;
}
// ***************************** SetAtoDCcNum() *************************************
// Set CC # for every A to D.
void SetAtoD_CcNum(int p0, int p1, int p2, int p3)
{
	AtoD[0].ccNum = p0;
	AtoD[1].ccNum = p1;
	AtoD[2].ccNum = p2;
	AtoD[3].ccNum = p3;
}
