#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include <vector>
#include <memory>
#include "task.h"
class TaskManager
{
public:
    TaskManager();
    void AddTask(std::unique_ptr<Task> task);
    void RemoveTask(int index);
    void MoveTask(int from, int to);
private:
    std::vector<std::unique_ptr<Task>> tasks;
};

#endif // TASKMANAGER_H
