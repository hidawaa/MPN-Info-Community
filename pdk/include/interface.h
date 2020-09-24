#ifndef INTERFACE_H
#define INTERFACE_H

#include <QString>
#include <QVariant>
#include <QWidget>
#include <QDebug>

enum AddOnTypes {
    AddOnObject = 0,
    AddOnWidget,
    AddOnPage,
    AddOnProcess
};

enum AddOnFlags {
    AddOnCreateMenu      = 1,
    AddOnExecAtStart     = 2,
    AddOnExecBeforeLogin = 4,
    AddOnExecAfterLogin  = 16
};

enum AddOnPermission {
    AddOnAdministrators = 1,
    AddOnUsers          = 2 ,
    AddOnGuest          = 4
};

//! [0]
class CoreEngine;

class Object
{
public:
    virtual ~Object() {}

    virtual QStringList keys() = 0;
    virtual QVariant exec(const QString &key, const QVariant &arg = QVariant()) = 0;

    void setCallback(void (*cb)(const QVariantMap &data, void *userData), void *uData) { callback = cb;  userData = uData; }

private:
    void (*callback)(const QVariantMap &data, void *userData) { nullptr };
    void *userData { nullptr } ;
};

typedef  QSharedPointer<Object> ObjectPtr;

class Widget : public QWidget
{
public:
    Widget(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~Widget() {}

    virtual QStringList keys() = 0;
    virtual QVariant exec(const QString &key, const QVariant &arg = QVariant()) = 0;

    void setCallback(void (*cb)(const QVariantMap &data, void *userData), void *uData) { callback = cb;  userData = uData; }

private:
    void (*callback)(const QVariantMap &data, void *userData) { nullptr };
    void *userData { nullptr } ;
};

typedef  QSharedPointer<Widget> WidgetPtr;

class Page : public Widget
{
public:
    Page(QWidget *parent = nullptr) : Widget(parent) {}
    virtual ~Page() {}

    virtual QStringList keys() { return QStringList(); }
    virtual QVariant exec(const QString &key, const QVariant &arg = QVariant()) { (void) key; (void) arg; return QVariant(); }
};

typedef  QSharedPointer<Page> PagePtr;

class Process
{
public:
    virtual ~Process() {}

    virtual void run() = 0;
};

typedef  QSharedPointer<Process> ProcessPtr;

class AddOn
{
public:
    virtual ~AddOn() {}

    virtual QString name() = 0;
    virtual QString group() = 0;
    virtual QString title() = 0;
    virtual AddOnTypes type() = 0;
    virtual int loadFlags() = 0;
    virtual QString loadAfter() { return QString(); }
    virtual int permission() = 0;
    virtual ObjectPtr newObject() { return nullptr; }
    virtual WidgetPtr *newWidget() { return nullptr; }
    virtual PagePtr newPage() { return nullptr; }
    virtual ProcessPtr newProcess() { return nullptr; }

    CoreEngine *engine;
};

typedef  QSharedPointer<AddOn> AddOnPtr;
typedef  QHash<QString, AddOnPtr> AddOnHash;

class Interface
{
public:
    virtual QString author() = 0;
    virtual QString authorEmail() = 0;
    virtual QString group() = 0;
    virtual QString version() = 0;
    virtual QString description() = 0;
    virtual QStringList dependencies() { return QStringList(); }
    virtual QList<AddOnPtr> addOns() = 0;
};

QT_BEGIN_NAMESPACE

#define IID_INTERFACE "MPN-Info.AddOns"
Q_DECLARE_INTERFACE(Interface, IID_INTERFACE)

QT_END_NAMESPACE

//! [0]
#endif
