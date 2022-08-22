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

// used for per-string capacitive touch sensor
typedef struct
{
	// When MIDI note msgs serve as RHC, this tracks whether noteOn or noteOff RHC trigger msg was the last received.
	// When a button serves as RHC, this tracks whether the button is currently pressed or not.
	bool rhcActive;

	byte rhcNotePitch;	// the note value that gets recognized as a RHC event.
	byte rhcNoteVelocity;	// stores the incoming RHC note's velocity so the encoder can use it when it sends its msg.
} rhcStrItem;

typedef struct
{
	int pitchOffset;
	int channel;
	// bool sustain;

	int currFret;	// Value ignored when isOpen == true.
	bool isOpen;
	bool changed;	// set if currFret has changed.
	int encMode;	// the current encoder mode, unless 'override' is in effect.
	int encModeBackup;	// for saving encMode value when override is in effect.
	unsigned char msgPitch;	// stored here for use in noteOff msg when it occurs.
} lhEncodeItem;

// this is used only in OrganMode for now. May be incorporated later into others.
// it sets a min on time for notes.
// typedef struct
// {
// 	bool isActive; // set while timer is running
// 	unsigned int count;	
// 	int fret;	// fret which is being debounced
// } encoderDebounceItem;

extern int noteDurationFromPot;
extern byte monoCurrPitch;

extern const int D0_pin;
extern const int D1_pin;
extern const int D2_pin;
extern const int D3_pin;
extern const int D4_pin;
extern const int D5_pin;

extern const int StrobeLHC;

extern rhcStrItem rhcStr[];
extern lhEncodeItem lhEncode[];
// extern encoderDebounceItem encoderDebounce[];

// define pins that select the count. Pin numbers here are for the Sparkfun Pro Micro Qwiic.
const int D0_pin = 2;		// p5. Mega p27
const int D1_pin = 3;		// p6. Mega p26
const int D2_pin = 4;		// p7. Mega p25
const int D3_pin = 5;		// p8. Mega p24
const int D4_pin = 6;		// p9. Mega p23
const int D5_pin = 7;		// p10. Mega p22

const int StrobeLHC = 8;	// p11. Mega p28.

#endif
