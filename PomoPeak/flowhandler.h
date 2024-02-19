#ifndef FLOWHANDLER_H
#define FLOWHANDLER_H

enum FlowSequence { Session = 1, ShortBreak = 2, LongBreak = 6 };

class FlowHandler
{
public:
    FlowHandler(int shortBreakEvery = 2, int longBreakEvery = 6);
    void Next();
    FlowSequence GetCurrentSequence();


private:
    int toShortBreak;
    int currentToShortBreak;
    int toLongBreak;
    int currentToLongBreak;
};
#endif // FLOWHANDLER_H
