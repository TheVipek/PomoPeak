#include "taskmanager.h"
#include <QDebug>
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
    for(auto iterator = tasks.begin(); iterator != tasks.end(); iterator++)
    {
        if(*iterator == task)
        {
            tasks.erase(iterator);
            qDebug() << "Sucessfuly erased";
            return;
        }
    }
    qDebug() << "Not erased";
}
void TaskManager::MoveTask(int from, int to)
{
   //TODO
}

