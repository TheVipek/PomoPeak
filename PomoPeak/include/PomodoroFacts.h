#ifndef POMODOROFACTS_H
#define POMODOROFACTS_H
#include "GPTHelper.h"
#include <QStringList>
#include <random>
#include <iostream>
class PomodoroFacts
{
public:
    PomodoroFacts(GPTHelper* gptHelper);
    QString GetNextFact();
private:
    GPTHelper* gptHelper;
    void RefillFacts();
    QStringList avaliableFacts;
};

#endif // POMODOROFACTS_H
