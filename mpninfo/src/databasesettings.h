#ifndef DATABASESETTINGS_H
#define DATABASESETTINGS_H

#include <QVariant>

#include <coreengine.h>

class DatabaseSettings : public Settings
{
public:
    DatabaseSettings(CoreEngine *engine) : mEngine(engine) {}
    ~DatabaseSettings() {}

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key);

private:
    CoreEngine *mEngine;
};

#endif // DATABASESETTINGS_H
