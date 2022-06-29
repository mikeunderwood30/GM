

/*
// ***************************************** checkSerialAvailable() ***********************************
void checkSerialAvailable()
{
    byte inByte;
    byte midiStatus;
    
    int numBytes = Serial1.available();
    
    while (numBytes > 0)
    {
      inByte = Serial1.read();
      
      if (inByte == MIDI_ACTIVE_SENSING)
        return;
          
      if (inByte >= 128)
      {
        if (dbgMidiIn)
        {
          Serial.println(inByte,HEX);
        }
        
        switch(inByte)
        {
          case MIDI_TIMING_CLOCK:
              //ProcessMidiClock();
              break;
          case MIDI_START:
          //Serial.println("MIDI_START");
            break;
          case MIDI_STOP:
          //Serial.println("MIDI_STOP");
            break;
          case MIDI_SYSEX_START:
            break;
          default:  // this covers all channel messages, which can't be compared directly becasue of channel nybble.              
            statusByte = inByte & 0xF0;
            gChannel = inByte & 0x0F;
            dataByteCount = 0;
            break;
        }
      } else   // a data byte
      {
        if (statusByte == MIDI_CC)
        {
          if (dataByteCount == 0)
          {
            dataByte1 = inByte;
            dataByteCount++;
          }
          else if (dataByteCount == 1)
          {
            dataByte2 = inByte;
            dataByteCount = 0;
  	      processControlChangeMsg(statusByte, gChannel, dataByte1, dataByte2);
          }
        }
        else if (statusByte == MIDI_PITCH_BEND)
        {
          if (dataByteCount == 0)
          {
            dataByte1 = inByte;
            dataByteCount++;
          }
          else if (dataByteCount == 1)
          {
            dataByte2 = inByte;
            dataByteCount = 0;
  	      processPitchbendMsg(statusByte, gChannel, dataByte1, dataByte2);
          }
        }
        else if (statusByte == MIDI_NOTE_ON || statusByte == MIDI_NOTE_OFF)
        {
          if (dataByteCount == 0)
          {
            dataByte1 = inByte;
            dataByteCount++;
          }
          else if (dataByteCount == 1)
          {
            dataByte2 = inByte;
            dataByteCount = 0;
            
            if (dataByte2 == 0)
              statusByte = MIDI_NOTE_OFF;
            
  	    processChannelNoteMsg(statusByte, gChannel, dataByte1, dataByte2);
          }
        }
        else if (statusByte == MIDI_PRG_CHG)
        {
            dataByte1 = inByte;
            dataByteCount = 0;
            
  	    processPrgChg(gChannel, dataByte1);
        }
      }
      numBytes--;
  }
}
*/
// ***************************************** processPrgChg() ************************************
void processPrgChg(byte channel, byte prgChgValueIn)
{
}
// ***************************************** processChannelNoteMsg() ************************************
void processChannelNoteMsg(byte statusByte, byte channel, byte dataByte1, byte dataByte2)
{
  byte statusWithChannel;
  byte translatedPitch;
  byte tryoutPitch;
  
  if (dbgMidiIn)
  {
    Serial.print("processChannelNoteMsg(). Channel = ");
    Serial.print(channel);
    Serial.print(", dataByte1 = ");
    Serial.println(dataByte1);
  }
}

// ***************************************** processPitchbendMsg() ************************************
void processPitchbendMsg(byte statusByte, byte channel, byte dataByte1, byte dataByte2)
{
  byte statusWithChannel;
  
  //Serial.print("processPitchbendMsg(). dataByte1 = ");
  /*
  if (gEchoChannelMsg)
  {
    statusWithChannel = statusByte + channel;
    sendChannelMsg(statusWithChannel, dataByte1, dataByte2);
  }
  else
  {
  }
  */
}
// ***************************************** processControlChangeMsg() ************************************
void processControlChangeMsg(byte statusByte, byte channel, byte dataByte1, byte dataByte2)
{
  byte statusWithChannel;
  
  //Serial.print("processControlChangeMsg(). dataByte1 = ");

  /*
  if (gEchoChannelMsg)
  {
    statusWithChannel = statusByte + channel;
    sendChannelMsg(statusWithChannel, dataByte1, dataByte2);
    //sendChannelMsg(statusWithChannel, 11, dataByte2);  // SampleModeling clarinet needs CC11 to work
  }
  else if (gDrumTryoutMode)
  {
    // When the drum is hit, this will be used as the pitch value.
    gTryoutMode_PitchVal = dataByte2;
  }
  else
  {
  }
  */
}
// ***************************************** sendChannelMsg() ************************************
void sendChannelMsg(byte midiStatus, byte data1, byte data2)
{
  byte statusByte = midiStatus & 0xF0;
  
  Serial1.write(midiStatus);
  Serial1.write(data1);
  
  if (statusByte != MIDI_PRG_CHG)
    Serial1.write(data2);

  if (dbgMidiOut)
  {
    Serial.print("Sent: ");
    Serial.print(midiStatus, HEX);
    Serial.print(", ");
    Serial.print(data1, HEX);
    
    Serial.print(", ");
    Serial.print(data2, HEX);
    Serial.println();
  }
}


