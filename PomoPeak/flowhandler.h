#ifndef FLOWHANDLER_H
#define FLOWHANDLER_H

#include "settings.h"
enum FlowSequence { Session = 1, ShortBreak = 2, LongBreak = 6 };

class FlowHandler
{
public:
    FlowHandler(const Settings& _settings);
    void Next();
    FlowSequence GetCurrentSequence();


private:
    const Settings& settings;
    int currentToShortBreak;
    int currentToLongBreak;


};
#endif // FLOWHANDLER_H
