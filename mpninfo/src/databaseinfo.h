#ifndef DATABASEINFO_H
#define DATABASEINFO_H

#include <QVariant>

#include <coreengine.h>

class DatabaseInfo : public Settings
{
public:
    DatabaseInfo(CoreEngine *engine) : mEngine(engine) {}
    ~DatabaseInfo() {}

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key);
    QVariant remove(const QString &key);

private:
    CoreEngine *mEngine;
};

#endif // DATABASEINFO_H
