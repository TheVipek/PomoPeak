#ifndef TASK_H
#define TASK_H
#include <QString>
struct Task
{
public:
    Task();
    Task(QString title, QString description, int pomodorosToDo);
    QString title;
    QString description;
    int pomodorosDone = 0;
    int pomodorodsToDo = 0;
};

#endif // TASK_H
