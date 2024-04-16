#include "PomodoroFacts.h"
#include <QtConcurrent>
PomodoroFacts::PomodoroFacts(std::shared_ptr<GPTHelper> helper)
{
    gptHelper = helper;
    RefillFacts();
}

QString PomodoroFacts::GetNextFact()
{
    QString fact;
    if(avaliableFacts.isEmpty())
    {
        fact = "No facts avaliable.";
    }
    else
    {
        fact = avaliableFacts.takeFirst();
    }

    TryRefillFacts();

    return fact;

}
void PomodoroFacts::TryRefillFacts()
{
    if(avaliableFacts.count() < 5)
    {
        QtConcurrent::run([=](){
            RefillFacts();
        });
    }
}
void PomodoroFacts::RefillFacts()
{
    QStringList sl = gptHelper->Ask("Give me 10 random facts about focus and learning",
                                    "Start every fact with 'Did you know that', write each fact in new line")
                         .split("\n");
    avaliableFacts.append(sl);
}
