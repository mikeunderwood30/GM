#include "ParmConfig.h"
#include "Encoder.h"
#include "MIDIUSB.h"
#include "Common.h"
#include "AnalogControl.h"

// Uses Sparkfun Pro Micro Qwiic

//------- Constants -------//

#define FTSW_0	29
#define FTSW_1	31
#define FTSW_2	33
#define FTSW_3	35

void InitTimers();
void StartLatchDebounce(byte str, unsigned int duration);
void ServiceTimers();

void outputGmCount(byte); // test code

void sendChannelMsg(byte midiStatus, byte data1, byte data2);

// --------------------- Debug ---------------------------

bool dbgMidiIn = true;
bool dbgMidiOut = true;
 
 // ------------------------ Arduino hardware ----------------------------------
int ledPin = 13;

//const int numDigitalIns = 4;
// PinDebounce debounce[numDigitalIns] = {29, false, false, 0, 31, false, false, 0, 33, false, false, 0, 35, false, false, 0};
//unsigned long debounceDelay = 5;	// milliseconds

// --------------------- footswitch definitions ----------------------
const int FTSW_ADVANCE = 3;
const int FTSW_ALT1 =    1;
const int FTSW_ALT2 =    2;
const int FTSW_ALT3 =    0;

// ------------------ MIDI messages ----------------
const int MIDI_NOTE_ON = 0x90;
const int MIDI_NOTE_OFF = 0x80;
const int MIDI_CC = 0xB0;
const int MIDI_PRG_CHG = 0xC0;
const int MIDI_PITCH_BEND = 0xE0;
const int MIDI_SYSEX_START = 0xF0;
const int MIDI_SYSEX_STOP = 0xF7;

const int MIDI_TIMING_CLOCK = 0xF8;
const int MIDI_START = 0xFA;
const int MIDI_CONTINUE = 0xFB;
const int MIDI_STOP = 0xFC;

const int MIDI_ACTIVE_SENSING = 0xFE;

int dataByteCount;  // start at 2 to make sure we get a status byte before we call StartNote()

byte statusByte;
byte dataByte1;
byte dataByte2;

byte gChannel;  //  Channel numbers are 0 based

void noteOn(byte channel, byte pitch, byte velocity) {
	Serial.print("Sending note on, pitch = ");	
	Serial.println(pitch);

	midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
	MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
	Serial.print("Sending note off, pitch = ");	
	Serial.println(pitch);

	midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
	MidiUSB.sendMIDI(noteOff);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
	midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
	MidiUSB.sendMIDI(event);

	// Serial.print("channel ");
	// Serial.print(channel);
	// Serial.print(", CC num ");
	// Serial.print(control);
	// Serial.print(", value ");
	// Serial.println(value);
}

// ***************************************** setup() ***********************************
void setup()
{ 
	Serial.begin(115200);
	Serial1.begin(31250);
	//Serial.begin(9600);

	Serial.println("Hello");

	// initialize the digital pins
	pinMode(D0_pin, OUTPUT);
	pinMode(D1_pin, OUTPUT);
	pinMode(D2_pin, OUTPUT);
	pinMode(D3_pin, OUTPUT);
	pinMode(D4_pin, OUTPUT);
	pinMode(D5_pin, OUTPUT);

	pinMode(ledPin, OUTPUT);

	pinMode(StrobeLHC, INPUT);
	digitalWrite(StrobeLHC, HIGH);       // turn on pullup resistor

	InitTimers();

	InitEncoders();
	InitPresetSelect();
	InitAnalogControls();

	ExecutePreset(0);
}

int incomingByte;
int prevStrobeVal = LOW;
float prevVal[NUM_GTR_STRINGS] = {0,0,0,0};

bool temp;

// ***************************************** loop() *********************************** 
void loop()
{
	midiEventPacket_t rx;
	do {
	rx = MidiUSB.read();
	if (rx.header != 0) {
		//Serial.print("Header: ");
		//Serial.print(rx.header, HEX);
		//Serial.print("-");
    
		//Serial.print(rx.byte1, HEX);
		//Serial.print("-");
		//Serial.print(rx.byte2, HEX);
		//Serial.print("-");
		//Serial.println(rx.byte3, HEX);
    
		//AssembleSysExIn(rx.byte1);
		//AssembleSysExIn(rx.byte2);
		//AssembleSysExIn(rx.byte3);

		bool isPressed = rx.byte1 == 0x90 ? true : false;

		switch (rx.byte2)
		{
			case 0x40:
				rhcStr[0].inNoteOn = isPressed;
				break;

			case 0x41:
				rhcStr[1].inNoteOn = isPressed;
				break;

			case 0x42:
				rhcStr[2].inNoteOn = isPressed;
				break;

			case 0x43:
				rhcStr[3].inNoteOn = isPressed;
				break;
		}
	}
	} while (rx.header != 0);

  
	incomingByte = 0;
	int currStrobeVal = LOW;

	// For each string, scan. If anything changed, update .currFret and set a flag.
	scanBasic();

	for (int ss = 0; ss < NUM_GTR_STRINGS; ss++)
	{
		switch (lhEncodeBasic[ss].encMode)
		{
			case ENC_MODE_PRESET_SELECT:
				EncoderPresetSelect(ss);
				break;

			case ENC_MODE_STRINGWISE_INT:
			case ENC_MODE_STRINGWISE_EXT:
				EncodePreprocess(ss);
				EncodeStringwise(ss);
				break;

			case ENC_MODE_STRINGWISE_ORGAN:
				EncodeStringwiseOrgan(ss);
				break;

			case ENC_MODE_AUTOCHORD:
				EncodeAutochord(ss);
				break;	
		}
	}

	//ServiceTimers();

	// check A-to-D values for on-board controls, such as pots, etc
	// read pot1
// 	int adcValue = analogRead(ccSource[0][CC_SRC_POT1].atodNum);
// 	adcValue = adcValue >> 3;	// 0-1024 range to 0-127
// 	if (adcValue > 127)	// should never be, but just in case.
// 		adcValue = 127;

// 	noteDurationFromPot = adcValue * 15;	// 127 * 15 = 1905 ms. About 2 seconds max duration.
  
//   if (noteDurationFromPot < 10)
//     noteDurationFromPot = 10;

	// ---------------- maybe use later --------------------
	//checkSerialAvailable();
	//checkFootSwitchToggle();
}
// ***************************** UserHitAKey() *************************************
bool UserHitAKey()
{
  
	if (Serial.available())
	{
		incomingByte = Serial.read();
		return true;
	}

	return false;
}
// ***************************************** checkFootSwitchToggle() ***********************************
void checkFootSwitchToggle()
{
	/*
  for (int ii = 0; ii < numDigitalIns; ii++)
  {
	// process this input if it's known to be changing.
    if (debounce[ii].debounceInProgress)
    {
      if ((millis() - debounce[ii].debounceStartTime) > debounceDelay)
      {
        // sample input for this pin. If truly has changed, take appropriate action
        bool reading = digitalRead(debounce[ii].digInputPin);
        if (reading != debounce[ii].lastSteadyValue)
        {
          debounce[ii].lastSteadyValue = reading;
		  ProcessFtswChanged(ii, !debounce[ii].lastSteadyValue);
        }
        debounce[ii].debounceInProgress = false;
      }
    }
	// else, debounce is not in progress on this pin. Check if it has changed since last time, and if so, start debounce process on it.
	else
	{
		bool reading = digitalRead(debounce[ii].digInputPin);

		//Serial.print("Input ");
		//Serial.print(debounce[ii].digInputPin);
		//Serial.print(" value is  ");
		//Serial.println(reading);

		if (reading != debounce[ii].lastSteadyValue)
		{
			debounce[ii].debounceInProgress = true;
			debounce[ii].debounceStartTime = millis();
		}
	}
  }
  */
}
// ***************************************** ProcessFtswChanged() ***********************************
void ProcessFtswChanged(int ftswNum, bool newValue)
{
	Serial.print("Input ");
	Serial.println(ftswNum);
	Serial.print(" has changed. New value is  ");
	Serial.println(newValue);
 
	/*  
	switch(ftswNum)
	{
		case 0:
			break;

		case 1:
			break;

		case 2:
			break;

		case 3:
			break;
	}
 */
}
