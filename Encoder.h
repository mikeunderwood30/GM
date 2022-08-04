#ifndef Encoder_h
#define Encoder_h

#define MAX_FRETS	16
#define NUM_GTR_STRINGS	4
#define NOTE_BUFFER_SIZE	8

// Encoder modes
enum eEncMode {
	ENC_MODE_PRESET_SELECT,
	ENC_MODE_STRINGWISE_INT,
	ENC_MODE_STRINGWISE_EXT,
	ENC_MODE_GATED_AUTO_RHC,
	ENC_MODE_STRINGWISE_ORGAN,
	ENC_MODE_AUTOCHORD
};

// Autochord
#define AC_NUM_CHORDS 5
#define AC_NOTES_PER_CHORD 4

// used for timing note duration
// typedef struct
// {
// 	bool isActive;
// 	unsigned int count;	

// 	byte msgPitch;	// stored here for use in noteOff msg when it occurs.

// } rhcNoteDurationItem;

// typedef struct
// {
// 	int currFret;	// has a value of -1 when string is open
// 	bool changed;	// set if currFret has changed.
// 	int encMode;	// the current encoder mode, unless 'override' is in effect.
// 	int encModeBackup;	// for saving encMode value when override is in effect.
// } lhcBasicItem;

// used for per-string capacitive touch sensor
typedef struct
{
	int pinNumber;
	bool isPressed;

	// used only by ExtRHC encoder
	bool inNoteOn;	// tracks whether noteOn or noteOff RHC trigger msg was the last received.
	byte rhcNotePitch;	// the note value that gets recognized as a RHC event.
	byte rhcNoteVelocity;	// stores the incoming RHC note's velocity so the encoder can use it when it sends its msg.
} rhcStrItem;

typedef struct
{
	int pitchOffset;
	int channel;
	// bool sustain;

	int currFret;	// can never have a value of -1. Value ignored when isOpen == true.
	bool isOpen;
	bool changed;	// set if currFret has changed.
	int encMode;	// the current encoder mode, unless 'override' is in effect.
	int encModeBackup;	// for saving encMode value when override is in effect.
	byte msgPitch;	// stored here for use in noteOff msg when it occurs.
} lhEncodeItem;

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
//extern lhcBasicItem lhEncodeBasic[];
extern lhEncodeItem lhEncode[];

#endif
