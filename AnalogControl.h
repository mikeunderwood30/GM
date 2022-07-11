#ifndef AnalogControl_h
#define AnalogControl_h

#define NUM_AtoD_CONTROLS	4

typedef struct
{
	unsigned char channel;
	unsigned char ccNum;
	int atoDNum;
	int currAdcValue;
} AtoDItem;

// extern int noteDurationFromPot;
extern AtoDItem AtoD[];

#endif
