#ifndef COREENGINE_H
#define COREENGINE_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>

#include "coredata.h"
#include "database.h"
#include "common.h"
#include "window.h"
#include "interface.h"
#include "runnable.h"

class Settings
{
public:
    virtual ~Settings() {}

    virtual void setValue(const QString &key, const QVariant &value) = 0;
    virtual QVariant value(const QString &key) = 0;
    virtual QVariant remove(const QString &key) = 0;
};


class CoreEngine : public QObject
{
public:
    CoreEngine(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~CoreEngine() {}

    virtual QString version() = 0;
    virtual QString workingDirectory() = 0;

    virtual const User &user() = 0;
    virtual const Kantor &kantor() = 0;

    virtual DatabasePtr database() = 0;

    virtual QSettings *settings() = 0;
    virtual Settings *databaseSettings() = 0;
    virtual Settings *databaseInfo() = 0;

    virtual AddOnPtr addOn(const QString &name) = 0;
    virtual QStringList availableAddOns() = 0;

    virtual Common *common() = 0;
    virtual Window *window() = 0;

    virtual void runSync(Runnable *runnable) = 0;
    virtual void runAsync(Runnable *runnable) = 0;
};

#endif // COREENGINE_H
