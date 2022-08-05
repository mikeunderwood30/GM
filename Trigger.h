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
    // TRIG_SRC_TB0_PRESS,
    // TRIG_SRC_TB1_PRESS,
    // TRIG_SRC_TB2_PRESS,
    // TRIG_SRC_TB3_PRESS,
    // TRIG_SRC_TB0_RELEASE,
    // TRIG_SRC_TB1_RELEASE,
    // TRIG_SRC_TB2_RELEASE,
    // TRIG_SRC_TB3_RELEASE,
    // TRIG_SRC_NTON_0,
    // TRIG_SRC_NTON_1,
    // TRIG_SRC_NTON_2,
    // TRIG_SRC_NTON_3,
    // TRIG_SRC_NTOF_0,
    // TRIG_SRC_NTOF_1,
    // TRIG_SRC_NTOF_2,
    // TRIG_SRC_NTOF_3,
	NUM_TRIG_TYPES
};

// Trigger actions.
enum eTrigAction {
    TrigActn_StartPhrase,   // parm1 specifies which phrase
    TrigActn_SendEvent,     // parm1 specifies which event
    TrigActn_RHC_PRESS,      // parm1 specifies which string
    TrigActn_RHC_RELEASE    // parm1 specifies which string
};


 
typedef struct
{
    // can have various Trigger sources. Current sources are MIDI Note messages and touch buttons.
	eTrigEvtType evtType;
    int evtParm;  // for example, if evtType is 'button press', this indicates which button.

	eTrigAction action;
    byte actionParm;
} triggerItem;


extern triggerItem trigger[];

#endif