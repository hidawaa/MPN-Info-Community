#include "plugin.h"

#include <QSqlQuery>

int Plugin::type()
{
    return InterfaceObject;
}

void Plugin::init(CoreEngine *engine)
{
    mEngine = engine;
}

QString Plugin::author()
{
    return "your name";
}

QString Plugin::authorEmail()
{
    return "your email";
}

QString Plugin::group()
{
    return "your plugin group";
}

QString Plugin::name()
{
    return "your plugin name";
}

QString Plugin::version()
{
    return ""your plugin version";
}

QString Plugin::title()
{
    return "your plugin display title";
}

QString Plugin::description()
{
    return "your plugin discription";
}

Object *Plugin::object()
{
    // create your object here
    return nullptr;
}

