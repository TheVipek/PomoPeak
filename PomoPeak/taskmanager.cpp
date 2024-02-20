#include "taskmanager.h"

TaskManager::TaskManager()
{

}
void TaskManager::AddTask(std::unique_ptr<Task> task)
{
    tasks.push_back(task);
}

void TaskManager::RemoveTask(int index)
{
    if(index >= 0 && index < tasks.size())
    {
        tasks.erase(tasks.begin() + index);
    }
}
void TaskManager::MoveTask(int from, int to)
{
   //TODO
}

