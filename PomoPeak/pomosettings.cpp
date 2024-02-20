#include "pomosettings.h"

PomoSettings::PomoSettings(float sessionDuration, float shortBreakDuration, float longBreakDuration)
{
    SessionDuration = sessionDuration * 60;
    ShortBreakDuration = shortBreakDuration * 60;
    LongBreakDuration = longBreakDuration * 60;
}
