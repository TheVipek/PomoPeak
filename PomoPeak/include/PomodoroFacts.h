#ifndef POMODOROFACTS_H
#define POMODOROFACTS_H
#include "GPTHelper.h"
#include <QStringList>
#include <random>
#include <iostream>
class PomodoroFacts
{
public:
    PomodoroFacts(std::shared_ptr<GPTHelper> gptHelper);
    QString GetNextFact();
    void TryRefillFacts();
private:
    std::shared_ptr<GPTHelper> gptHelper;
    void RefillFacts();
    QStringList avaliableFacts;
};

#endif // POMODOROFACTS_H
