#include "flowhandler.h"

FlowHandler::FlowHandler(const Settings& _settings) : settings(_settings)
{
    currentToShortBreak = settings.ShortBreakAfterSessions;
    currentToLongBreak = settings.LongBreakAfterShortBreaks;
}

FlowSequence FlowHandler::GetCurrentSequence()
{
    if (currentToShortBreak > 0) {
        return FlowSequence::Session;
    } else if (currentToLongBreak > 0) {
        return FlowSequence::ShortBreak;
    } else {
        return FlowSequence::LongBreak;
    }
}

void FlowHandler::Next()
{
    if(GetCurrentSequence() == FlowSequence::Session)
    {
        currentToShortBreak--;
    }
    else
    {
        if (GetCurrentSequence() == FlowSequence::LongBreak) {
            currentToLongBreak = settings.ShortBreakAfterSessions;
            currentToShortBreak = settings.LongBreakAfterShortBreaks;
        } else if (GetCurrentSequence() == FlowSequence::ShortBreak) {
            currentToShortBreak = settings.ShortBreakAfterSessions;
            currentToLongBreak--;
        }
    }


}
