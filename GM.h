#ifndef Gm_h
#define Gm_h

#define NUM_TOUCH_BUTTONS   4

typedef struct
{
    int pinNumber;  // pin # on Pro Micro uC
	bool isPressed; // remembers last known state so we can detect when it changes and fire a Trigger.
} TouchButtonItem;

extern TouchButtonItem touchButton[];

#endif