#ifndef FLOWHANDLER_H
#define FLOWHANDLER_H

#include "settings.h"
enum FlowSequence { Session = 1, ShortBreak = 2, LongBreak = 6 };

class FlowHandler
{
public:
    FlowHandler(const Settings& _settings);
    FlowSequence GetCurrentFlowSequence();
    FlowSequence Next();


private:
    const Settings& settings;
    FlowSequence _currentFlowSequence;
    bool isBreak;
    int sessionsCount;

};
#endif // FLOWHANDLER_H
