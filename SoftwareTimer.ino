#include "Encoder.h"
#include "Timer.h"

// based on
//  https://forum.arduino.cc/t/using-millis-for-timing-a-beginners-guide/483573

unsigned long baselineMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

unsigned int pulseTimer;
//unsigned int encOverrideTimer;

#define PULSE_TIMER_RELOAD  20
//#define ENC_OVERRIDE_TIMER_RELOAD  20

noteDurationItem noteBuffer[NOTE_BUFFER_SIZE];

// ***************************************** InitTimers()() ***********************************
// called from Setup()
void InitTimers()
{
  baselineMillis = millis();  //initial start time

  // cyclical timers
  pulseTimer = PULSE_TIMER_RELOAD;
  //encOverrideTimer = ENC_OVERRIDE_TIMER_RELOAD;
}
// ***************************************** AddNoteToTimerPool() ***********************************
void AddNoteToTimerPool(unsigned int duration, byte msgPitch)
{ 
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
  //static bool encOverridePressed = false;

  //static int currAdcValue0 = -1;
  //static int currAdcValue1 = -1;
  //static int currAdcValue2 = -1;

  currentMillis = millis();  //get the current number of milliseconds since the program started
  if (currentMillis == baselineMillis)   // everything divides the 1 ms clock
  //if (currentMillis < baselineMillis + 50)
  {
      return;
  }

  baselineMillis = currentMillis;

  // 1 ms has elapsed

    // plotter will have a '0' entry every 1 ms.
  //Serial.print(0);

// ----------------- One-shot --------------  
/*
  for (int ii = 0; ii < NOTE_BUFFER_SIZE; ii++)
  {
    if (noteBuffer[ii].isActive)
    {
      noteBuffer[ii].count--;
      if (noteBuffer[ii].count == 0)
      {
        noteOff(0, noteBuffer[ii].msgPitch, 64); 	// Channel, pitch, velocity
        noteBuffer[ii].isActive = false;
      }
    }
  }
*/

for (int ss = 0; ss < NUM_GTR_STRINGS; ss++)
{
  if (encoderDebounce[ss].isActive)
  {
    encoderDebounce[ss].count--;
    if (encoderDebounce[ss].count == 0)
    {
      encoderDebounce[ss].isActive = false;
      // check whether the fret we think is pressed is really pressed. 
      if (!FretIsPressed(ss, encoderDebounce[ss].fret))
      {
        // send a noteoff for it. 
      }
      // If so, allow it to remain on.
    }
  }
}
// ----------------- Cyclical --------------

  // 'Check override' timer is for checking if user has pressed or released the encoder override button.
//  encOverrideTimer--;   // use diff name since used for A to D also.
/*
  if(encOverrideTimer == 0)
  {
    if (encOverridePressed)
    {
      if (digitalRead(EncodeOverrideButton) == LOW)
      {
        Serial.println("Encoder override was released");	
        encOverridePressed = false;

        for (int ss = 0; ss < NUM_GTR_STRINGS; ss++)
        {
          lhEncode[ss].encMode = lhEncode[ss].encModeBackup;
        }
      }
    }
    else
    {
      if (digitalRead(EncodeOverrideButton) == HIGH)
      {
        Serial.println("Encoder override was pressed");	
        encOverridePressed = true;

        for (int ss = 0; ss < NUM_GTR_STRINGS; ss++)
        {
          // backup the current encoder mode so it can be restored when override is over.
          lhEncode[ss].encModeBackup = lhEncode[ss].encMode;

          // override the encoder mode. This will allow user to select a Preset using the fretboard.
          lhEncode[ss].encMode = ENC_MODE_PRESET_SELECT;
        }
      }
    }

    // should be on its own timer eventually
    //CheckAtoDControls();

    encOverrideTimer = ENC_OVERRIDE_TIMER_RELOAD;
  }
*/
  // Pulse timer is just for reference. Idea is to have a constant pulse on the plotter display.
  pulseTimer--;

  if(pulseTimer == 0)
  {
    //Serial.print(1);
    pulseTimer = PULSE_TIMER_RELOAD;
  }
}
