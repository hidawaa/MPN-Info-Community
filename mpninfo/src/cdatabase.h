#ifndef CDATABASE_H
#define CDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <database.h>

class CDatabase : public Database
{
public:
    CDatabase(const QSqlDatabase &db);
    ~CDatabase();

    QString driverName();

    void prepare(const QString &query);
    void bindValue(const QString &placeHolder, const QVariant &value);
    void exec();
    void exec(const QString &query);

    int numARowsffected();
    bool next();
    QVariant value(int i);
    QVariant value(const QString &name);
    int size();

private:
    QString mDriverName;
    QString mConnectionName;
    QSqlQuery mQuery;
};

#endif // CDATABASE_H
