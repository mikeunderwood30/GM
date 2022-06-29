
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
// ***************************************** AccumSysExInput() ************************************
void AccumSysExInput(byte bb)
{	
	//Serial.print(F("AccumSerialInput(). bb = "));
	//Serial.println(bb);

	//if (receivingSerial)
	//{
		if (cfgByteCnt < 20)
		{
			SerMsgBuffer[cfgByteCnt] = bb;
			cfgByteCnt++;
		}
	//}
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
