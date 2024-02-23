#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include <vector>
#include <memory>
#include "task.h"
class TaskManager
{
public:
    TaskManager();
    void AddTask(std::shared_ptr<Task> task);
    void RemoveTask(int index);
    void MoveTask(int from, int to);
    void RemoveTask(std::shared_ptr<Task> task);
private:
    std::vector<std::shared_ptr<Task>> tasks;
};

#endif // TASKMANAGER_H
