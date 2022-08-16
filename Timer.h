#ifndef Timer_h
#define Timer_h

#define NOTE_BUFFER_SIZE	8

extern void AddNoteToTimerPool(unsigned int, byte);

//used for timing note duration
typedef struct
{
	bool isActive;
	unsigned int count;	

	byte msgPitch;	// stored here for use in noteOff msg when it occurs.

} noteDurationItem;

extern noteDurationItem noteBuffer[];

#endif
