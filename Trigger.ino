#include "Common.h"
#include "Trigger.h"
#include "GM.h"

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

// ************************** InitTriggers() ****************************
void InitTriggers()
{
    // set all to false
    for (int tt = 0; tt < NUM_TRIGGERS; tt++)
    {
        if (trigger[tt].isActive)
        {
            trigger[tt].isActive = false;
        }
    }

    // Most Triggers are configured as part of a Preset. Here we define some that won't change.
    trigger[0].evtType = TE_TYPE_BUTTON_PRESS;
    trigger[0].evtParm = EncodeOverrideButton;
    trigger[0].action = TrigActn_EncodeOverrideOn;
    trigger[0].actionParm = 0;  // not used
    trigger[0].isActive = true;

    trigger[1].evtType = TE_TYPE_BUTTON_RELEASE;
    trigger[1].evtParm = EncodeOverrideButton;
    trigger[1].action = TrigActn_EncodeOverrideOff;
    trigger[1].actionParm = 0;  // not used
    trigger[1].isActive = true;
}

// ************************** CheckTriggers() ****************************
// go through all Triggers. Wherever the event matches, execute the corresponding action.
void CheckTriggers(eTrigEvtType evtType, int evtParm)
{
    for (int tt = 0; tt < NUM_TRIGGERS; tt++)
	{
        if (trigger[tt].isActive)
        {
            // Serial.println("Active Trigger (action, actionParm): ");
            // Serial.print(trigger[tt].action);
            // Serial.print(",");
            // Serial.print(trigger[tt].actionParm);
            // Serial.println();

            if (trigger[tt].evtType == evtType && trigger[tt].evtParm == evtParm)
            {
                switch (trigger[tt].action)
                {
                    case TrigActn_StartPhrase:
                    break;

                    case TrigActn_SendEvent:
                    Serial.println("Sending event ");
                    Serial.print(trigger[tt].actionParm);
                    break;

                    case TrigActn_RHC_PRESS:
                        //Serial.println("Trigger = RHC_PRESS");
                        // Here we could check the encoder mode to make sure we're in a mode which uses RHC,
                        // but we'll assume that we are. Be sure to clean up unused Triggers, or may get unexpected behavior.
                        EncodeStringwiseRhc(true, trigger[tt].actionParm); // actionParm indicates which string was plucked
                        break;

                    case TrigActn_RHC_RELEASE:
                        //Serial.println("Trigger = RHC_RELEASE");
                        EncodeStringwiseRhc(false, trigger[tt].actionParm);
                        break;

                    case TrigActn_EncodeOverrideOn:
                        Serial.println("Trigger = EncodeOverrideOn");
                        EncodeOverrideOnOff(true);
                        DumpGtrInfo();
                        break;

                    case TrigActn_EncodeOverrideOff:
                        Serial.println("Trigger = EncodeOverrideOff");
                        EncodeOverrideOnOff(false);
                        DumpGtrInfo();
                        break;
                }
            }
        }
    }
}
