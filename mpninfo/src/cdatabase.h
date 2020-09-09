#ifndef CDATABASE_H
#define CDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include <database.h>

class CDatabaseCompatibility;
struct CDatabaseData {
    QSqlDatabase database;
    QSqlQuery query;
    QScopedPointer<CDatabaseCompatibility> compatibility;
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

    int numRowsAffected();
    bool next();
    QVariant value(int i);
    QVariant value(const QString &name);
    int size();

    DatabaseCompatilbility *compatibility();

private:
    QScopedPointer<CDatabaseData> mData;
};

#endif // CDATABASE_H
