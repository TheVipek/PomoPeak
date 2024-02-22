#include "task.h"

Task::Task()
{

}
Task::Task(QString title, QString description, int pomodorosToDo)
                                                                        : title(title)
                                                                        , description(description)
                                                                        , pomodorodsToDo(pomodorosToDo)
{

}
