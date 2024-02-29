#ifndef SQLITEHANDLER_H
#define SQLITEHANDLER_H

#include <QSqlDatabase>
#include <vector>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
class sqliteHandler
{
public:
    sqliteHandler(QSqlDriver* driver, const QString& connectionName);
    template<typename T>
    std::vector<T> GetData(const QString query);
private:
    QSqlDatabase db;
};

#endif // SQLITEHANDLER_H
