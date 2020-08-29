#ifndef DATABASE_H
#define DATABASE_H

#include <QVariant>

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

    virtual int numRowsffected() = 0;
    virtual bool next() = 0;
    virtual QVariant value(int i) = 0;
    virtual QVariant value(const QString &name) = 0;
    virtual int size() = 0;
};

typedef QSharedPointer<Database> DatabasePtr;

#endif // DATABASE_H
