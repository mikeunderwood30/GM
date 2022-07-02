#ifndef Encoder_h
#define Encoder_h

#define MAX_FRETS	16
#define NUM_GTR_STRINGS	4
#define NOTE_BUFFER_SIZE	8

// #define PIEZO_ONESHOT_0	0
// #define PIEZO_ONESHOT_1	1
// #define PIEZO_ONESHOT_2	2
// #define PIEZO_ONESHOT_3	3

extern void InitEncoderStringwise();
extern void scanStrStringwise(int ss);

// used for timing note duration
typedef struct
{
	bool isActive;
	unsigned int count;	

	byte msgPitch;	// stored here for use in noteOff msg when it occurs.
	//int aToDNum;
	//int pitchOffsetStringwise[NUM_GTR_STRINGS];
	//int channelStringwise[NUM_GTR_STRINGS];
} rhcNoteDurationItem;

// used for per-string capacitive touch sensor
typedef struct
{
	int pinNumber;
	bool isPressed;
	byte msgPitch;	// stored here for use in noteOff msg when it occurs.
} rhcStrItem;

// typedef struct
// {
// int currPressed[NUM_GTR_STRINGS];
// int candidate[NUM_GTR_STRINGS];
// int debounceTimer[NUM_GTR_STRINGS];

// bool sustainStringwise[NUM_GTR_STRINGS];
// } lhcStrItem;

extern int pitchOffsetStringwise[];
extern int channelStringwise[];
extern int currPressed[];
//extern int lastPressed;

extern int candidate[];
extern int debounceTimer[];

extern bool sustainStringwise[];

extern rhcNoteDurationItem noteBuffer[NOTE_BUFFER_SIZE];

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

// if decide to use these, replace the pin mode etc in Setup() which was removed.
//extern const int ResetRHC0;
//extern const int ResetRHC1;
//extern const int ResetRHC2;
//extern const int ResetRHC3;

#endif
