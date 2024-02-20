#ifndef POMOSETTINGS_H
#define POMOSETTINGS_H
struct PomoSettings
{
public:
    PomoSettings(float sessionDuration, float shortBreakDuration, float longBreakDuration);
    float SessionDuration;
    float ShortBreakDuration;
    float LongBreakDuration;
};

#endif // POMOSETTINGS_H
