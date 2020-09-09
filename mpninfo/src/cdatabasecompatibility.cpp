#include "cdatabasecompatibility.h"

CDatabaseCompatibility::CDatabaseCompatibility(CDatabase *_db) :
    db(_db) {}

void CDatabaseCompatibility::truncateTable(const QString &table)
{

    if (db->driverName() == "QSQLITE" || db->driverName() == "QSQLCIPHER") {
        db->exec(QString("DELETE FROM `%1`").arg(table));
        db->exec("VACUUM");
    }
    else
       db->exec(QString("TRUNCATE `%1`").arg(table));
}

void CDatabaseCompatibility::beginTransaction()
{
    if (db->driverName() == "QSQLITE" || db->driverName() == "QSQLCIPHER")
        db->exec("BEGIN TRANSACTION");
}

void CDatabaseCompatibility::endTransaction()
{
    if (db->driverName() == "QSQLITE" || db->driverName() == "QSQLCIPHER")
        db->exec("COMMIT TRANSACTION");
}
