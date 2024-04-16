#ifndef FLOWHANDLER_H
#define FLOWHANDLER_H

#include "settings.h"
enum FlowSequence { Session = 1, ShortBreak = 2, LongBreak = 6 };

class FlowHandler
{
public:
    FlowHandler(std::shared_ptr<Settings> _settings);
    FlowSequence GetCurrentFlowSequence();
    FlowSequence Next();


private:
    std::shared_ptr<Settings> settings;
    FlowSequence _currentFlowSequence;
    bool isBreak;
    int sessionsCount;

};
#endif // FLOWHANDLER_H
