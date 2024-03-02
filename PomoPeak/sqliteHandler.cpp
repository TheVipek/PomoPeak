#include "sqliteHandler.h"

SqliteHandler::SqliteHandler(const QString& connectionName)
{
    db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
}

