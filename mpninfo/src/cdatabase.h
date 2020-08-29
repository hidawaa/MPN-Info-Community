#ifndef CDATABASE_H
#define CDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <database.h>

struct CDatabaseData {
    QSqlDatabase database;
    QSqlQuery query;
};

class CDatabase : public Database
{
public:
    CDatabase(const QSqlDatabase &db);
    ~CDatabase();

    QString driverName();

    void prepare(const QString &query);
    void addBindValue(const QVariant &value);
    void bindValue(const QString &placeHolder, const QVariant &value);
    void exec();
    void exec(const QString &query);

    QString lastExecuted();

    int numRowsffected();
    bool next();
    QVariant value(int i);
    QVariant value(const QString &name);
    int size();

private:
    QScopedPointer<CDatabaseData> mData;
};

#endif // CDATABASE_H
