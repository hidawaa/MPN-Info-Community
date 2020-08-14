#ifndef PLUGIN_H
#define PLUGIN_H

#include <QWidget>
#include <QtPlugin>

#include "interface.h"

class Plugin : public QObject, Interface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IID_INTERFACE FILE "plugin.json")
    Q_INTERFACES(Interface)

public:
    int type();
    void init(CoreEngine *);
    QString author();
    QString authorEmail();
    QString group();
    QString name();
    QString version();
    QString title();
    QString description();
    Object *object();

private:
    CoreEngine *mEngine;
};


#endif
