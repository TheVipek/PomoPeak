#ifndef POMOPEAKSETTINGS_H
#define POMOPEAKSETTINGS_H



struct Settings
{
    float SessionDuration;
    float ShortBreakDuration;
    float LongBreakDuration;

    Settings(float sessionDuration, float shortBreakDuration, float longBreakDuration)
    {
        SessionDuration = sessionDuration * 60;
        ShortBreakDuration = shortBreakDuration * 60;
        LongBreakDuration = longBreakDuration * 60;
    }
};
#endif
