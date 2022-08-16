#ifndef Trigger_h
#define Trigger_h

#define NUM_TRIGGERS    12

// When a Trigger Event occurs, like receiving a certain MIDI note message, or user touching a certain button, we go through the list of triggers to
// see if there is an action associated with it.
// Events consist of two parameters, an eTrigEvtType and an integer value. For example, for a button press, the type is 'button press' and the value
// specifies which one.

enum eTrigEvtType {
    TE_TYPE_BUTTON_PRESS,
    TE_TYPE_BUTTON_RELEASE,
    TE_TYPE_MIDI_NTON,
    TE_TYPE_MIDI_NTOF,
	NUM_TRIG_TYPES
};

// Trigger actions.
enum eTrigAction {
    TrigActn_StartPhrase,   // actionParm specifies which phrase
    TrigActn_SendEvent,     // actionParm specifies which event
    TrigActn_RHC_PRESS,      // actionParm specifies which string
    TrigActn_RHC_RELEASE,    // actionParm specifies which string
    TrigActn_EncodeOverrideOn,
    TrigActn_EncodeOverrideOff
};

extern void InitTriggers();
extern void CheckTriggers(eTrigEvtType, int);

typedef struct
{
    bool isActive;
    
    // can have various Trigger sources. Current sources are MIDI Note messages and touch buttons.
	eTrigEvtType evtType;
    int evtParm;  // for example, if evtType is 'button press', this indicates which button.

	eTrigAction action;
    byte actionParm;
} triggerItem;


extern triggerItem trigger[];

#endif
