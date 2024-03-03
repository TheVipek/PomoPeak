#include "sqliteHandler.h"
#include "databasetables.h"
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
                                           "ShortBreakAfterSessions INTEGER,"
                                           "LongBreakAfterShortBreaks INTEGER,"
                                           "QuickActionShortcut TEXT,"
                                           "SessionAlarm BLOB,"
                                           "BreakAlarm BLOB);").arg(DatabaseTables::SETTINGS);
    query.prepare(createSettingsTableQ);

    if(!query.exec())
    {
        qDebug() << "Error creating table s:" << query.lastError().text();
    }
    else
    {
        qDebug() << "Successfully created settings table";
    }
    db.close();
}
