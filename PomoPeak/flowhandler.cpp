#include "flowhandler.h"

FlowHandler::FlowHandler(int shortBreakEvery, int longBreakEvery)
{
    toShortBreak = shortBreakEvery;
    currentToShortBreak = toShortBreak;
    toLongBreak = longBreakEvery;
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
    if (currentToShortBreak > 0) {
        currentToShortBreak--;
    }
    if (currentToShortBreak == 0) {
        currentToShortBreak = toShortBreak;
        currentToLongBreak--;
    }
    if (currentToLongBreak == 0) {
        currentToLongBreak = toLongBreak;
    }
}
