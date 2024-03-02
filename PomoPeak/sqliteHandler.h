#ifndef SQLITEHANDLER_H
#define SQLITEHANDLER_H

#include <QSqlDatabase>
#include <vector>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
class SqliteHandler
{
public:
    SqliteHandler(const QString& connectionName);
    template<typename T>
    std::vector<T> GetData(const QString queryStr)
    {
        QSqlQuery query(db);
        query.prepare(queryStr);

        if(!query.exec())
        {
            qWarning() << "Failed to execute query:" << query.lastError();
            return {};
        }

        std::vector<T> data;

        while(query.next())
        {
            QSqlRecord record = query.record();
            data.push_back(T::FromSqlRecord(record));
        }

        return data;
    }
private:
    QSqlDatabase db;
};

#endif // SQLITEHANDLER_H
