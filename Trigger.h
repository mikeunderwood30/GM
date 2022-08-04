#ifndef Trigger_h
#define Trigger_h

// Trigger sources.
enum eTrigSrc {
    TRIG_SRC_TB_0,
    TRIG_SRC_TB_1,
    TRIG_SRC_TB_2,
    TRIG_SRC_TB_3,
	NUM_TRIGGERS
};

// Trigger actions.
enum eTrigAction {
    TrigActn_StartPhrase,
    TrigActn_SendEvent,
    TrigActn_RHC
};

typedef struct
{
    // can have various Trigger sources. For now the only sources we have are the 16 RHC buttons, which have ids in the range 0 - 15
	eTrigSrc srcId;
	eTrigAction action;
    byte parm1;
} triggerItem;

#endif