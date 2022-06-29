
byte SerMsgBuffer[20];

static int cfgByteCnt;
static bool isUpperNybble;
static byte sysExByte;

// ***************************************** AssembleSysExIn() ***********************************
// Assembles 2 nybbles into a byte and then calls AccumSysExInput() which accums the byte into a buffer.
// Once get EOX, then call ProcessSysExBuffer().
void AssembleSysExIn(byte inByte)
{
	//if (dbgRawMidiIn)
	//{
	//	Serial.println(inByte, HEX);
	//	Serial.print(F(" "));
	//}
		

	if (inByte == STAT_SOX)
	{
		// prepare to receive data
		isUpperNybble = false;
		cfgByteCnt = 0;
	}
	else if (inByte == STAT_EOX)
	{
		ProcessSysExBuffer();
	}
	else
	{
		if (!isUpperNybble)
		{
			// this is a lower nybble, which arrives first
			sysExByte = inByte;		
			isUpperNybble = true;
		}
		else
		{
			sysExByte |= inByte << 4;
			if (cfgByteCnt < 20)
			{
				// this is needed because SOX and EOX are also sent as individual nybbles.
				if (sysExByte != STAT_SOX && sysExByte != STAT_EOX)
				{
					SerMsgBuffer[cfgByteCnt] = sysExByte;
					cfgByteCnt++;
				}
			}
			isUpperNybble = false;
		}
	}
}

// ***************************************** ProcessSysExBuffer() ************************************
//  At this point, SerMsgBuffer[] has been filled and cfgByteCnt contains the # items.
void ProcessSysExBuffer()
{

	Serial.print(F("ProcessSerialInput(); cfgByteCnt = "));
	Serial.print(cfgByteCnt);
	Serial.println(F(":"));
	

	for (int ii = 0; ii < cfgByteCnt; ii++)
	{
		Serial.print(SerMsgBuffer[ii], HEX);
		Serial.print(F(" "));
	}
	
	Serial.println();
}

/*
// ***************************************** ProcessSysExByte() ************************************
// sysExByte contains reconstituted data. it had been split into low/high nybble for transmission,
// but has been reconstituted by the time it arrives here.
void ProcessSysExByte(byte sysExByte)
{
	if (dbgSysEx)
	{
		Serial.print(F("ProcessSysExByte() - received "));
		Serial.println(sysExByte, HEX);
	}
	
	//if (cfgByteCnt == -1)
	//	cfgByteCnt = 0;
			
	switch (sysExMsgType)
	{				
		case CFG_CAT_WRITE_TIMELINE:
			//ProcessDefTimelineByte(cfgByteCnt, sysExByte);
			cfgByteCnt++;
			break;
			
		case CFG_CAT_WRITE_TIMELINE_EVENT:
			//ProcessDefTimelineEventByte(cfgByteCnt, sysExByte);
			cfgByteCnt++;
			break;
			
		//case CFG_CAT_DEF_FTSW:
		//	ProcessDefFtswByte(cfgByteCnt, sysExByte);
		//	cfgByteCnt++;
		//	break;
			
		//case CFG_CAT_DEF_MIDI_IN:
		//	ProcessDefMidiInByte(cfgByteCnt, sysExByte);
		//	cfgByteCnt++;
		//	break;
			
		//case CFG_CAT_DEF_ROUTE: 
		//	cfgByteCnt++;
		//	break;

		//case CFG_CAT_DEF_CHORD: 
		//	ProcessDefChordByte(cfgByteCnt, sysExByte);
		//	cfgByteCnt++;
		//	break;
			
		//case CFG_CAT_KEY_CHORD: 
		//	ProcessKeyChordByte(cfgByteCnt, sysExByte);
		//	cfgByteCnt++;
		//	break;
			
		//case CFG_CAT_DEBUG: 
			//utilProcessDebugByte_SysEx(cfgByteCnt, sysExByte);
		//	cfgByteCnt++;
		//	break;
	}
}

// ***************************************** ShowSysExMsgType() ***********************************
void ShowSysExMsgType(byte msgType)
{
	//Serial.print(F("ShowSysExMsgType(): Message Type = ");
	
	switch (msgType)
	{
		//case CFG_CAT_WRITE_TIMELINE:
		//	Serial.println("CFG_CAT_DEF_TIMELINE");
		//	break;
			
		//case CFG_CAT_WRITE_TIMELINE_EVENT:
		//	Serial.println("CFG_CAT_DEF_TIMELINE_EVENT");
		//	break;
			
		//case CFG_CAT_WRITE_FTSW:
		//	Serial.println("CFG_CAT_DEF_FTSW");
		//	break;
			
		//case CFG_CAT_WRITE_ROUTE: 
		//	Serial.println("CFG_CAT_DEF_ROUTE");
		//	break;

		//case CFG_CAT_WRITE_CHORD: 
		//	Serial.println("CFG_CAT_DEF_CHORD");
		//	break;
			
		case CFG_CAT_DEBUG: 
			Serial.println("CFG_CAT_DEBUG");
			break;
			
		default:
			Serial.print(F("Message type Undefined - "));
			Serial.println(msgType);
			break;
	}
}
*/

// ***************************************** SendSimSysExMsg() ***********************************
/*
void SendSimSysExMsg(byte sysExMsg[], int size)
{
	for (int ii = 0; ii < size; ii++)
	{
		ProcessSysExByte(sysExMsg[ii]);
	}
	//sysExMsgType = CFG_CAT_UNKNOWN;
	cfgByteCnt = -1;
}
*/
