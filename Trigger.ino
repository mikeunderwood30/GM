#include "Trigger.h"

/*
Touch Buttons as Triggers
----------------------

Every time around the loop(), do a digitalRead() on each touchButton to find out its status (pressed or not). We only take an action when something changes,
so we will compare the current value that we read to the .isPressed variable. If it is different, then any Trigger that has a button press or a button
release as its source will get fired. The eTrigSrc enum defines two sources for each button; one for press, and one for release. This is the same way
MIDI works; there is a one message for NoteOn and another for NoteOff.

So let's say now that something has changed, and we have an eTrigSrc value for it (which implies we've identified which button it is, and whether it's
a press or release). We can now attempt to invoke a Trigger for it (see below).

MIDI Notes as Triggers
----------------------

MIDI NoteOn and NoteOff msgs can also act as Triggers.
When receive one of these, go through a list of known status/channel/pitch values and try to make a match. if successful, it will return one of the
TRIG_SRC_NTON_x for a NoteOn, TRIG_SRC_NTOF_x for a NoteOff (see enum eTrigSrc for the list). This means the MIDI message is recognized, but does not
tell whether it's used by any Trigger, or if so, which one. 

If the MIDI message is recognized, attempt to invoke a Trigger for it (see below).

Invoking a Trigger
------------------

So now that you have a eTrigSrc value, go through the list of Triggers and try to make a match against this value. If found, execute the respective action.
See eTrigAction in Trigger.h for the list of possible Actions.

If eTrigAction for a Trigger is set to 'TrigActn_RHC'
----------------------------------------------------

If the action for a Trigger is 'TrigActn_RHC' ...
TBD: Need to rework the RHC encoders so they are event-driven. They do not have to detect a change themselves.

*/

triggerItem trigger[NUM_TRIGGERS];

// ************************** CheckTriggers() ****************************
// go through all Triggers. Wherever src is found as a source, execute the corresponding action.
void CheckTriggers(eTrigSrc src)
{
    for (int tt = 0; tt < NUM_TRIGGERS; tt++)
	{
    }
}
