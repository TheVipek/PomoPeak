#include "flowhandler.h"

FlowHandler::FlowHandler(std::shared_ptr<Settings> _settings) : settings(_settings)
{
    sessionsCount = 0;
    _currentFlowSequence = FlowSequence::Session;
    isBreak = false;
}
FlowSequence FlowHandler::Next()
{
    isBreak = !isBreak;

    if(isBreak)
    {
        sessionsCount++;
        if(sessionsCount % settings->LongBreakFrequency == 0)
        {
            _currentFlowSequence = FlowSequence::LongBreak;
        }
        else
        {
            _currentFlowSequence = FlowSequence::ShortBreak;
        }
    }
    else
    {
        _currentFlowSequence = FlowSequence::Session;
    }

    return _currentFlowSequence;

}
FlowSequence FlowHandler::GetCurrentFlowSequence()
{
    return _currentFlowSequence;
}
