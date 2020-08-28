#include "cdatabase.h"

#include <QSqlQuery>
#include <QDebug>

CDatabase::CDatabase(const QSqlDatabase &db)
{
    mData.reset(new CDatabaseData);
    mData->database = db;
    mData->query = QSqlQuery(db);
}

CDatabase::~CDatabase()
{
    QString connectionName = mData->database.connectionName();
    mData->database.close();
    mData.reset();

    QSqlDatabase::removeDatabase(connectionName);
}

QString CDatabase::driverName()
{
    return mData->database.driverName();
}

void CDatabase::prepare(const QString &query)
{
    mData->query.prepare(query);
}

void CDatabase::addBindValue(const QVariant &value)
{
    mData->query.addBindValue(value);
}

void CDatabase::bindValue(const QString &placeHolder, const QVariant &value)
{
    mData->query.bindValue(placeHolder, value);
}

void CDatabase::exec(const QString &query)
{
    mData->query.exec(query);
}

void CDatabase::exec()
{
    mData->query.exec();
}

int CDatabase::numRowsffected()
{
    return mData->query.numRowsAffected();
}

bool CDatabase::next()
{
    return mData->query.next();
}

QVariant CDatabase::value(int i)
{
    return mData->query.value(i);
}


QVariant CDatabase::value(const QString &name)
{
    return mData->query.value(name);
}

int CDatabase::size()
{
    return mData->query.size();
}
