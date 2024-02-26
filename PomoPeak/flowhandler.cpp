#include "flowhandler.h"

FlowHandler::FlowHandler(int shortBreakEvery, int longBreakEvery)
                                                                : toShortBreak(shortBreakEvery)
                                                                , toLongBreak(longBreakEvery)
{
    currentToShortBreak = toShortBreak;
    currentToLongBreak = toLongBreak;
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
            currentToLongBreak = toLongBreak;
            currentToShortBreak = toShortBreak;
        } else if (GetCurrentSequence() == FlowSequence::ShortBreak) {
            currentToShortBreak = toShortBreak;
            currentToLongBreak--;
        }
    }


}
