#ifndef SQLITEHANDLER_H
#define SQLITEHANDLER_H

#include <QSqlDatabase>
#include <vector>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QString>
#include <QList>
#include <QPair>
class SqliteHandler
{
public:
    SqliteHandler(const QString& connectionName);
    ~SqliteHandler();
    template<typename T>
    std::vector<T> GetData(const QString queryStr)
    {
        db.open();
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
        db.close();
        return data;
    }
    bool Exist(QString tableName, QList<QPair<QString,QVariant>> map);
    bool SetData(QString tableName, QList<QPair<QString,QVariant>> map);
    bool Update(QString tableName, QList<QPair<QString,QVariant>> updates, QList<QPair<QString,QVariant>> conditions);
private:
    QSqlDatabase db;

    void CheckIfDatabaseIsInCorrectState();
};

#endif // SQLITEHANDLER_H
