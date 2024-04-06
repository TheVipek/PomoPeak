#include "sqliteHandler.h"
#include "databasetables.h"
#include <QVariantList>
SqliteHandler::SqliteHandler(const QString& connectionName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(connectionName);
    CheckIfDatabaseIsInCorrectState();

}

void SqliteHandler::CheckIfDatabaseIsInCorrectState()
{
    db.open();
    QSqlQuery query(db);
    QString createSettingsTableQ = QString("CREATE TABLE IF NOT EXISTS %1 ("
                                           "UserID INTEGER PRIMARY KEY,"
                                           "SessionDuration INTEGER,"
                                           "ShortBreakDuration INTEGER,"
                                           "LongBreakDuration INTEGER,"
                                           "SessionAlarmVolume INTEGER,"
                                           "BreakAlarmVolume INTEGER,"
                                           "BreakAlarmRepetitions INTEGER,"
                                           "LongBreakFrequency INTEGER,"
                                           "QuickActionShortcut TEXT,"
                                           "SessionAlarm BLOB,"
                                           "SessionAlarmName TEXT,"
                                           "SessionAlarmExt TEXT,"
                                           "BreakAlarm BLOB,"
                                           "BreakAlarmName TEXT,"
                                           "BreakAlarmExt TEXT,"
                                           "Notifications INT,"
                                           "AlarmSound INT,"
                                           "Skin INT,"
                                           "ChatGPTApiKey TEXT);").arg(DatabaseTables::SETTINGS);
    query.prepare(createSettingsTableQ);

    if(!query.exec())
    {
        qDebug() << "Error creating table s:" << query.lastError().text();
    }
    else
    {
        qDebug() << "Successfully created settings table";
    }

    QString createStatsTableQ = QString("CREATE TABLE IF NOT EXISTS %1 ("
                                        "UserID INTEGER PRIMARY KEY,"
                                        "TaskStats TEXT);").arg(DatabaseTables::STATS);
    query.prepare(createStatsTableQ);

    if(!query.exec())
    {
        qDebug() << "Error creating table s:" << query.lastError().text();
    }
    else
    {
        qDebug() << "Successfully created stats table";
    }


    db.close();
}

bool SqliteHandler::Exist(QString tableName, QList<QPair<QString,QVariant>> conditions)
{
    if(conditions.isEmpty())
    {
        qDebug() << "map is empty.";
        return false;
    }

    db.open();
    QSqlQuery query(db);


    QStringList whereClauses;

    for(int i = 0; i < conditions.length(); i++)
    {
        whereClauses << QString("%1 = :%1").arg(conditions[i].first);
    }

    QString _q = QString("SELECT 1 FROM %1 WHERE %2 LIMIT 1").arg(tableName).arg(whereClauses.join(" AND "));

    query.prepare(_q);

    for(int i = 0; i < conditions.length(); i++)
    {
        query.bindValue(i, conditions[i].second);
    }

    if(!query.exec())
    {
        qDebug() << "Failed to check data in table:" << tableName << "Error:" << query.lastError().text();
        db.close();
        return false;
    }
    bool next = query.next();
    db.close();
    return next;



}

bool SqliteHandler::Update(QString tableName, QList<QPair<QString,QVariant>> updates, QList<QPair<QString,QVariant>> conditions)
{
    if (updates.isEmpty() || conditions.isEmpty()) {
        qDebug() << "Update or condition map is empty.";
        return false;
    }

    db.open();
    QSqlQuery query(db);


    QStringList setClauses;
    QStringList whereClauses;

    for(int i = 0; i < updates.length(); i++)
    {
        setClauses << QString("%1 = :update_%1").arg(updates[i].first);
    }
    for(int i = 0;i < conditions.length(); i++)
    {
        whereClauses << QString("%1 = :condition_%1").arg(conditions[i].first);
    }


    QString _q = QString("UPDATE %1 SET %2 WHERE %3").arg(tableName).arg(setClauses.join(", ")).arg(whereClauses.join(" AND "));
    query.prepare(_q);

    for(int i = 0; i < updates.length(); i++)
    {
        query.bindValue(":update_" + updates[i].first, updates[i].second);
    }

    for(int i = 0; i < conditions.length(); i++)
    {
        query.bindValue(":condition_" + conditions[i].first, conditions[i].second);
    }
    if(!query.exec())
    {
        qDebug() << "Failed to check data in table:" << tableName << "Error:" << query.lastError().text();
        db.close();
        return false;
    }
    db.close();
    return query.next();
}

bool SqliteHandler::SetData(QString tableName, QList<QPair<QString,QVariant>> map)
{
    if(map.isEmpty())
    {
        qDebug() << "map is empty.";
        return false;
    }

    db.open();
    QSqlQuery query(db);

    QString keys = "(";
    QString values = "VALUES(";
    QStringList valuePlaceholders;
    QVariantList valueData;

    for(int i = 0; i < map.length(); i++)
    {
        keys += map[i].first + ",";
        valuePlaceholders << "?";
        valueData << map[i].second;
    }

    keys.chop(1);
    keys += ")";

    values += valuePlaceholders.join(",") + ")";

    QString _q = "INSERT INTO " + tableName + " " + keys + " " + values;


    query.prepare(_q);

    int idx = 0;
    for(const auto& value : valueData)
    {
        query.bindValue(idx, value);
        idx++;
    }

    if(!query.exec())
    {
        qDebug() << "Failed to insert data into table:" << tableName << "Error:" << query.lastError().text();

        db.close();
        return false;
    }
    else
    {
        qDebug() << "Successfuly inserted data into table:" << tableName;
        db.close();
        return true;
    }
}
