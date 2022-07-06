#ifndef Encoder_h
#define Encoder_h

#define MAX_FRETS	16
#define NUM_GTR_STRINGS	4
#define NOTE_BUFFER_SIZE	8

// #define PIEZO_ONESHOT_0	0
// #define PIEZO_ONESHOT_1	1
// #define PIEZO_ONESHOT_2	2
// #define PIEZO_ONESHOT_3	3

#define ENC_MODE_PRESET_SELECT	0
#define ENC_MODE_STRINGWISE	1		

extern void InitEncoders();
//extern void scanStrStringwise(int ss);

// used for timing note duration
// typedef struct
// {
// 	bool isActive;
// 	unsigned int count;	

// 	byte msgPitch;	// stored here for use in noteOff msg when it occurs.
// 	//int aToDNum;
// 	//int pitchOffsetStringwise[NUM_GTR_STRINGS];
// 	//int channelStringwise[NUM_GTR_STRINGS];
// } rhcNoteDurationItem;

typedef struct
{
	int currFret;	// has a value of -1 when open
	bool changed;	// set if currFret has changed.
	int encMode;	// the current encoder mode, unless 'override' is in effect.
	int encModeBackup;	// for saving encMode value when override is in effect.
} lhcBasicItem;

// used for per-string capacitive touch sensor
typedef struct
{
	int pinNumber;
	bool isPressed;
} rhcStrItem;

typedef struct
{
	int currFret;	// never has a value of -1
	bool changed;	// set if currFret has changed.
	byte msgPitch;	// stored here for use in noteOff msg when it occurs.
} lhEncodeSwItem;

extern int pitchOffsetStringwise[];
extern int channelStringwise[];

//extern bool sustainStringwise[];

//extern rhcNoteDurationItem noteBuffer[NOTE_BUFFER_SIZE];

extern int noteDurationFromPot;
extern byte monoCurrPitch;

extern const int D0_pin;
extern const int D1_pin;
extern const int D2_pin;
extern const int D3_pin;
extern const int D4_pin;
extern const int D5_pin;

extern const int StrobeLHC;
extern const int EncodeOverrideButton;

extern rhcStrItem rhcStr[];
extern lhcBasicItem lhEncodeBasic[];
extern lhEncodeSwItem lhEncodeSw[];

// if decide to use these, replace the pin mode etc in Setup() which was removed.
//extern const int ResetRHC0;
//extern const int ResetRHC1;
//extern const int ResetRHC2;
//extern const int ResetRHC3;

#endif
