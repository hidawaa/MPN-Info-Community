#ifndef DATABASE_H
#define DATABASE_H

#include <QVariant>
#include <QSharedPointer>

class DatabaseCompatilbility
{
public:
    virtual ~DatabaseCompatilbility() {}

    virtual void truncateTable(const QString &table) = 0;
    virtual void beginTransaction() = 0;
    virtual void endTransaction() = 0;
};

class Database
{
public:
    virtual ~Database() {}

    virtual QString driverName() = 0;

    virtual void prepare(const QString &query) = 0;
    virtual void addBindValue(const QVariant &value) = 0;
    virtual void bindValue(const QString &placeHolder, const QVariant &value) = 0;
    virtual void exec() = 0;
    virtual void exec(const QString &query) = 0;

    virtual QString lastExecuted() = 0;

    virtual int numRowsAffected() = 0;
    virtual bool next() = 0;
    virtual QVariant value(int i) = 0;
    virtual QVariant value(const QString &name) = 0;
    virtual int size() = 0;

    virtual DatabaseCompatilbility *compatibility() = 0;
};

typedef QSharedPointer<Database> DatabasePtr;

#endif // DATABASE_H
