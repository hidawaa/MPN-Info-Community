#include "cdatabase.h"

#include <QSqlQuery>
#include <QDebug>

CDatabase::CDatabase(const QSqlDatabase &db) :
    mDriverName(db.driverName()),
    mConnectionName(db.connectionName()),
    mQuery(db) {}

CDatabase::~CDatabase()
{
    {
        QSqlDatabase db = QSqlDatabase::database(mConnectionName, false);
        db.close();
    }

    QSqlDatabase::removeDatabase(mConnectionName);
}

QString CDatabase::driverName()
{
    return mDriverName;
}

void CDatabase::prepare(const QString &query)
{
    mQuery.prepare(query);
}

void CDatabase::bindValue(const QString &placeHolder, const QVariant &value)
{
    mQuery.bindValue(placeHolder, value);
}

void CDatabase::exec(const QString &query)
{
    mQuery.exec(query);
}

void CDatabase::exec()
{
    mQuery.exec();
}

int CDatabase::numARowsffected()
{
    return mQuery.numRowsAffected();
}

bool CDatabase::next()
{
    return mQuery.next();
}

QVariant CDatabase::value(int i)
{
    return mQuery.value(i);
}


QVariant CDatabase::value(const QString &name)
{
    return mQuery.value(name);
}

int CDatabase::size()
{
    return mQuery.size();
}
