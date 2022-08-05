#include "Trigger.h"

/*
Touch Buttons as Triggers
----------------------

Every time around the loop(), do a digitalRead() on each touchButton to find out its status (pressed or not). We only take an action when something changes,
so we will compare the current value that we read to the .isPressed variable. If it is different, then any Trigger that has a button press or a button
release as its source will get fired. The eTrigEvtType enum defines two trigger events for each button; one for press, and one for release. This is the 
same way MIDI works; there is a one message for NoteOn and another for NoteOff.

MIDI Notes as Triggers
----------------------

MIDI NoteOn and NoteOff msgs can also act as Triggers.
When receive one of these, go through a list of known status/channel/pitch values and try to make a match. 

Invoking a Trigger
------------------

So once an event occurs we go through the list of Triggers and try to make a match against the eTrigEvtType (for example. button press) and the
value (for example, which button). If found, execute the respective action.
Say we have 16 buttons. we want the ability to assign any one of them as RHC for string 0. How can we do that?
This is where actionParm comes into play. When we configure the Trigger, we set actionParm to 0 in this case, indicating string 0.


If eTrigAction for a Trigger is set to 'TrigActn_RHC'
----------------------------------------------------

    TBD: Need to rework the RHC encoders so they are event-driven. They do not have to detect a change themselves.

*/

triggerItem trigger[NUM_TRIGGERS];

// ************************** CheckTriggers() ****************************
// go through all Triggers. Wherever the event matches, execute the corresponding action.
void CheckTriggers(eTrigEvtType evtType, int evtParm)
{
    for (int tt = 0; tt < NUM_TRIGGERS; tt++)
	{
        if (trigger[tt].evtType == evtType && trigger[tt].evtParm == evtParm)
        {
            switch (trigger[tt].action)
            {
                case TrigActn_StartPhrase:
                    break;

                case TrigActn_SendEvent:
                    break;

                case TrigActn_RHC_PRESS:
                    // Here we could check the encoder mode to make sure we're in a mode which uses RHC,
                    // but we'll assume that we are. Be sure to clean up unused Triggers, or may get unexpected behavior.
                    EncodeStringwiseRhc(true, trigger[tt].actionParm); // actionParm indicates which string was plucked
                    break;

                case TrigActn_RHC_RELEASE:
                    EncodeStringwiseRhc(false, trigger[tt].actionParm);
                    break;
            }
        }
    }
}
