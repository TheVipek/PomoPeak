#ifndef TASK_H
#define TASK_H
#include "string"
struct Task
{
public:
    Task(std::string title, std::string description, int pomodorosToDo);
    std::string title;
    std::string description;
    int pomodorosDone;
    int pomodorodsToDo = 0;
};

#endif // TASK_H
