#ifndef Trigger_h
#define Trigger_h

#define NUM_TRIGGERS    12

// Trigger sources.
enum eTrigSrc {
    TRIG_SRC_TB0_PRESS,
    TRIG_SRC_TB1_PRESS,
    TRIG_SRC_TB2_PRESS,
    TRIG_SRC_TB3_PRESS,
    TRIG_SRC_TB0_RELEASE,
    TRIG_SRC_TB1_RELEASE,
    TRIG_SRC_TB2_RELEASE,
    TRIG_SRC_TB3_RELEASE,
    TRIG_SRC_NTON_0,
    TRIG_SRC_NTON_1,
    TRIG_SRC_NTON_2,
    TRIG_SRC_NTON_3,
    TRIG_SRC_NTOF_0,
    TRIG_SRC_NTOF_1,
    TRIG_SRC_NTOF_2,
    TRIG_SRC_NTOF_3,
	NUM_TRIG_SOURCES
};

// Trigger actions.
enum eTrigAction {
    TrigActn_StartPhrase,   // parm1 specifies which phrase
    TrigActn_SendEvent,     // parm1 specifies which event
    TrigActn_RHC            // parm1 specifies which string
};

typedef struct
{
    // can have various Trigger sources. For now the only sources we have are the 16 RHC buttons, which have ids in the range 0 - 15
	eTrigSrc srcId;
	eTrigAction action;
    byte parm1;
} triggerItem;

#endif