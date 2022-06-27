#include "Encoder.h"

void sendChannelMsg(byte midiStatus, byte data1, byte data2);

// based on
//  https://forum.arduino.cc/t/using-millis-for-timing-a-beginners-guide/483573

unsigned long baselineMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

unsigned int pulseTimer;

#define PULSE_TIMER_RELOAD  20

// ***************************************** InitTimers()() ***********************************
// called from Setup()
void InitTimers()
{
  baselineMillis = millis();  //initial start time

  // Plotter timer is cyclical.
  pulseTimer = PULSE_TIMER_RELOAD;
}

// ***************************************** AddNoteToTimerPool() ***********************************
void AddNoteToTimerPool(unsigned int duration, byte msgPitch)
{ 
    // monoRhcNoteDuration.count = duration;
    // monoRhcNoteDuration.msgPitch = msgPitch;
    // monoRhcNoteDuration.isActive = true;

    //noteBuffer[NOTE_BUFFER_SIZE];
    for (int ii = 0; ii < NOTE_BUFFER_SIZE; ii++)
    {
      if (!noteBuffer[ii].isActive)
      {
        noteBuffer[ii].msgPitch = msgPitch;
        noteBuffer[ii].count = duration;
        noteBuffer[ii].isActive = true;

        break;
      }
    }
}
// ***************************************** ServiceTimers() ************************************
// called from loop()
void ServiceTimers()
{
  currentMillis = millis();  //get the current number of milliseconds since the program started
  if (currentMillis == baselineMillis)   // everything divides the 1 ms clock
  {
      return;
  }

  baselineMillis = currentMillis;

  // 1 ms has elapsed

  // plotter will have a '0' entry every 1 ms.
  //Serial.print(0);  

  // ----------------- Piezo --------------
  /*
  for (byte ss = 0; ss < NUM_GTR_STRINGS; ss++)
  {
    if (rhcDebounce[ss].isActive)
    {
      rhcDebounce[ss].count--;
      if(rhcDebounce[ss].count == 0)
      {
        //sendChannelMsg(0x80, rhcDebounce[ss].msgPitch, 0x7f);

        rhcDebounce[ss].isActive = false;
      }
    }
  }
  */
/*
  // Note Pool
  for (byte ss = 0; ss < NOTE_BUFFER_SIZE; ss++)
  {
    if (noteBuffer[ss].isActive)
    {
      noteBuffer[ss].count--;
      if(noteBuffer[ss].count == 0)
      {
        noteBuffer[ss].isActive = false;
        // Serial.print("Sending note off, pitch = ");	
        // Serial.println(noteBuffer[ss].msgPitch);	
        noteOff(0, noteBuffer[ss].msgPitch, 64);  // Channel 0, middle C, normal velocity
        MidiUSB.flush();
      }
    }
  }
*/
 // ----------------- Cyclical --------------
  // Pulse timer is just for reference. Idea is to have a constant pulse on the plotter display.
  pulseTimer--;

  if(pulseTimer == 0)
  {
    //Serial.print(1);
    pulseTimer = PULSE_TIMER_RELOAD;
  }
}
