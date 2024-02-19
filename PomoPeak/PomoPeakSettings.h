#ifndef POMOPEAKSETTINGS_H
#define POMOPEAKSETTINGS_H


struct Settings
{
    int SessionDuration;
    int ShortBreakDuration;
    int LongBreakDuration;

    Settings(int sessionDuration, int shortBreakDuration, int longBreakDuration)
    {
        SessionDuration = sessionDuration * 60;
        ShortBreakDuration = shortBreakDuration * 60;
        LongBreakDuration = longBreakDuration * 60;
    }
};
#endif
