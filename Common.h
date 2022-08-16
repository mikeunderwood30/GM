#ifndef Common_h
#define Common_h

enum OnOff {
	OFF,
	ON
};

#define byte	unsigned char

#define CHANNEL_3  		2
#define CHANNEL_4  		3
#define CHANNEL_5		4
#define CHANNEL_10		9
#define IN_CHAN_OMNI	99	// set midiInList[].matchChannel to this value to match anything.

#define STAT_NOTE_ON  0x90
#define STAT_NOTE_OFF 0x80

#define STAT_PRG_CHG  0xC0
#define STAT_CC 0xB0
#define STAT_SOX 0xF0
#define STAT_EOX  0xF7
#define STAT_ACTIVE_SENSING 254
#define STAT_TIMING_CLOCK 248
#define STAT_START  250
#define STAT_STOP 252
#define STAT_PITCH_BEND 224
#define STAT_CONTINUE 251

#define BYTE_MINUS_ONE	255	// a typically unused value for a byte type
#define TRACE_BUFFER_SIZE	100

#define BEATS_PER_MEASURE	4


// enum eTlPendingStates {
// 	TL_PENDING_STATE_NO_CHANGE,
// 	TL_PENDING_STATE_START_ONE_SHOT,
// 	TL_PENDING_STATE_START_OR_CONTINUE_LOOPING,
// 	TL_PENDING_STATE_STOP_LOOPING,
// 	TL_NUM_PENDING_STATES
// };

// enum eTlStates {
// 	TL_STATE_STOPPED,
// 	TL_STATE_RUNNING_ONE_SHOT,
// 	TL_STATE_RUNNING_LOOPED
// };

// // Sources
// // use these values to index the routes[] array. The value stored there represents the destination for that source.
// // note that MidiIn uses doesn't use the routes[] array, since the midiInList[] array contains a destination property.
// #define SrcScaler1			0		// Scalers are used to scale and offset values. Don't know if it's needed, since MidiIn and A to D will have it built in.
// #define SrcScaler2			1
// // #define SrcChordBuffer1		0
// // #define SrcAutochordType1	0
// // #define SrcHammeredDulcimer	0
// #define NUM_SOURCES			2

// enum eMidiInDest {
// 	MiDest_PassThrough,	// Note basically passes through, but may get transposed. Also, channel may get changed.
// 	MiDest_AcLh1,
// 	MiDest_AcLh1Rh1,
// 	MiDest_AcLh1_Timer,	// chord is sent out directly, not keyed
// 	MiDest_AcLh2,	
// 	MiDest_AcLh2Rh1,
// 	MiDest_AcLh3,
// 	MiDest_Mono1,
// 	Num_MiDest
// };


// specific to CC messages
enum eMidiInCcDest {
	CcDestSetTimerDuration,
	CcDestSetVolume,
	CcDest_PassThrough
};

#endif
