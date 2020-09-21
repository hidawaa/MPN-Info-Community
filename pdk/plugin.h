#ifndef PLUGIN_H
#define PLUGIN_H

#include <QWidget>
#include <QtPlugin>

#include "interface.h"
#include "exampleaddon.h"

class Plugin : public QObject, Interface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IID_INTERFACE FILE "plugin.json")
    Q_INTERFACES(Interface)

public:
    QString author() { return "Nama Author"; }
    QString authorEmail() { return "Email Author"; }
    QString group() { return "Nama Group"; }
    QString version() { return "Versi"; }
    QString description() { return "Deskripsi dari Addons"; }
    QList<AddOnPtr> addOns() { return QList<AddOnPtr>()); }
};


#endif
