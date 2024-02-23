#include "taskmanager.h"

TaskManager::TaskManager()
{

}
void TaskManager::AddTask(std::shared_ptr<Task> task)
{
    tasks.push_back(std::move(task));
}

void TaskManager::RemoveTask(int index)
{
    if(index >= 0 && index < tasks.size())
    {
        tasks.erase(tasks.begin() + index);
    }
}
void TaskManager::RemoveTask(std::shared_ptr<Task> task)
{

}
void TaskManager::MoveTask(int from, int to)
{
   //TODO
}

