#include "sqliteHandler.h"

SqliteHandler::SqliteHandler(QSqlDriver* driver, const QString& connectionName)
{
    db = QSqlDatabase::addDatabase(driver, connectionName);
}
template<typename T>
std::vector<T> SqliteHandler::GetData(const QString queryStr)
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
        QVariantMap rowMap;

        for(int i = 0; i < record.count(); ++i)
        {
            rowMap[record.fieldName(i)] = query.value(i);
        }

        data.push_back(T::fromVariantMap(rowMap));
    }


}
