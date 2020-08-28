#ifndef INTERFACE_H
#define INTERFACE_H

#include <QString>
#include <QVariant>
#include <QWidget>

enum AddOnTypes {
    AddOnObject = 0,
    AddOnPage,
    AddOnProcess
};

enum AddOnFlags {
    AddOnCreateMenu          = 1,
    AddOnExecAtStart         = 2,
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

class Page : public QWidget
{
public:
    Page(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~Page() {}

    virtual void setArgs(const QVariant &) {}
};

typedef  Page* PagePtr;

class Object
{
public:
    virtual ~Object() {}

    virtual QStringList keys() = 0;
    virtual QVariant exec(const QString &key, const QVariant &arg = QVariant()) = 0;
};

typedef  QSharedPointer<Object> ObjectPtr;

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
    virtual int permission() = 0;
    virtual ObjectPtr newObject() = 0;
    virtual Page *newPage() = 0;
    virtual ProcessPtr newProcess() = 0;

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
    virtual QList<AddOnPtr> addOns() = 0;
};

QT_BEGIN_NAMESPACE

#define IID_INTERFACE "MPN-Info.AddOns"
Q_DECLARE_INTERFACE(Interface, IID_INTERFACE)

QT_END_NAMESPACE

//! [0]
#endif
