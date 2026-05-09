#include "engine.h"

#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QPluginLoader>
#include <QSqlQuery>
#include <QThreadPool>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QDebug>

#include <window.h>
#include "cdata.h"
#include "ccommon.h"
#include "cdatabase.h"
#include "databasesettings.h"
#include "databaseinfo.h"

#include "databaseconnectiondialog.h"
#include "settingsdialog.h"

#define IDD_VERSION "0.20.0"

const char *chiperPassword = "MpnInfoDatabase";

const KantorMap emptyKantorMap;
const SeksiMap emptySeksiMap;
const PegawaiHash emptyPegawaiHash;
const WajibPajakHash emptyWajibPajakHash;

Engine::Engine(QObject *parent) :
    CoreEngine(parent),
    mSettings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat)
{
}

Engine::~Engine()
{
    {
        QSqlDatabase db = QSqlDatabase::database(mDatabaseConnectionName);
        db.close();
    }

    QSqlDatabase::removeDatabase(mDatabaseConnectionName);
}

Engine *Engine::instance()
{
    static Engine engine;
    return &engine;
}

bool Engine::connectDatabase()
{
    {
        DatabaseConnectionDialog dialog;
        do {
            QSqlDatabase::removeDatabase(mDatabaseConnectionName);

            int driver =  mSettings.value(IDS_DATABASE_TYPE).toInt();
            mDatabaseConnectionName = common()->randomString(16);

            QSqlDatabase db;
            if (driver == DatabaseMysql) {
                QString driverName = "QMYSQL";
                QString hostName =  mSettings.value(IDS_DATABASE_HOST).toString();
                QStringList authList = common()->decrypt(mSettings.value(IDS_DATABASE_AUTH).toString()).split(";");
                QString databaseName = authList[0];
                QString username = authList.size() > 1? authList[1] : QString();
                QString password = authList.size() > 2? authList[2] : QString();
                QString options =  mSettings.value(IDS_DATABASE_OPTIONS).toString();

                if (options.isEmpty())
                    options = "MYSQL_OPT_RECONNECT=1";

                db = QSqlDatabase::addDatabase(driverName, mDatabaseConnectionName);
                db.setHostName(hostName);
                db.setDatabaseName(databaseName);
                db.setUserName(username);
                db.setPassword(password);
                db.setConnectOptions(options);
                db.open();
            }
            else if (driver == DatabaseSqlite) {
                QString databaseName = mSettings.value(IDS_DATABASE_NAME).toString();
                if (databaseName.isEmpty())
                    databaseName = "test.db";

                bool encrypt = mSettings.value(IDS_DATABASE_ENCRYPTED).toBool();
                QString driverName = encrypt? "QSQLCIPHER" : "QSQLITE";
                QString options =  mSettings.value(IDS_DATABASE_OPTIONS).toString();

                db = QSqlDatabase::addDatabase(driverName, mDatabaseConnectionName);
                db.setDatabaseName(databaseName);
                db.setConnectOptions(options);
                db.open();
            }
            else if (driver == DatabaseOther) {
                QString driverName =  mSettings.value(IDS_DATABASE_DRIVER).toString();
                QString hostName =  mSettings.value(IDS_DATABASE_HOST).toString();
                QStringList authList = common()->decrypt(mSettings.value(IDS_DATABASE_AUTH).toString()).split(";");
                QString databaseName = authList[0];
                QString username = authList.size() > 1? authList[1] : QString();
                QString password = authList.size() > 2? authList[2] : QString();
                QString options =  mSettings.value(IDS_DATABASE_OPTIONS).toString();

                db = QSqlDatabase::addDatabase(driverName, mDatabaseConnectionName);
                db.setHostName(hostName);
                db.setDatabaseName(databaseName);
                db.setUserName(username);
                db.setPassword(password);
                db.setConnectOptions(options);
                db.open();
            }

            if (db.isOpen()) {
                QSqlQuery query(db);
                if (db.driverName() == "QMYSQL") {
                    query.exec("SET storage_engine=MyISAM");
                    query.exec("SET GLOBAL bulk_insert_buffer_size = 1024 * 1024 * 256");
                }
                else if (db.driverName() == "QSQLCIPHER") {
                    query.exec(QString("PRAGMA key='%1'").arg(chiperPassword));
                    query.exec("PRAGMA journal_mode=WAL;");
                }

                mDatabase = db;
                break;
            }

            int result = QMessageBox::warning(nullptr, "Database Connection Error", "Gagal melakukan koneksi dengan database. Coba lagi?", QMessageBox::Reset | QMessageBox::Yes | QMessageBox::No);
            if (result == QMessageBox::Yes)
                continue;
            else if (result == QMessageBox::No)
                return false;

            if (dialog.exec() == QDialog::Rejected)
                break;

            mSettings.setValue(IDS_DATABASE_TYPE, dialog.driver());
            if (dialog.driver() == DatabaseMysql) {
                QString auth = QString("%1;%2;%3").arg(dialog.database(), dialog.username(), dialog.password());

                mSettings.setValue(IDS_DATABASE_HOST, dialog.hostname());
                mSettings.setValue(IDS_DATABASE_AUTH, common()->encrypt(auth));
                mSettings.setValue(IDS_DATABASE_OPTIONS, dialog.options());

                mSettings.remove(IDS_DATABASE_DRIVER);
            }
            else if (dialog.driver() == DatabaseSqlite) {
                bool encrypt = false;
                if (dialog.driverName() == "QSQLCIPHER")
                    encrypt = true;

                if (encrypt)
                    mSettings.setValue(IDS_DATABASE_ENCRYPTED, true);
                else
                    mSettings.remove(IDS_DATABASE_ENCRYPTED);

                mSettings.setValue(IDS_DATABASE_NAME, dialog.database());
                mSettings.setValue(IDS_DATABASE_OPTIONS, dialog.options());

                mSettings.remove(IDS_DATABASE_DRIVER);
                mSettings.remove(IDS_DATABASE_HOST);
                mSettings.remove(IDS_DATABASE_AUTH);
            }
            else {
                QString auth = QString("%1;%2;%3").arg(dialog.database(), dialog.username(), dialog.password());

                mSettings.setValue(IDS_DATABASE_DRIVER, dialog.driverName());
                mSettings.setValue(IDS_DATABASE_HOST, dialog.hostname());
                mSettings.setValue(IDS_DATABASE_AUTH, common()->encrypt(auth));
                mSettings.setValue(IDS_DATABASE_OPTIONS, dialog.options());
            }
        } while (true);
    }

    return mDatabase.isOpen();
}

void Engine::loadAddons()
{
    QDir pluginsDir(QCoreApplication::applicationDirPath() + "/addons");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (!plugin)
            continue;

        Interface *interface = qobject_cast<Interface *>(plugin);
        if (!interface)
            continue;

        QList<AddOnPtr> addOnList = interface->addOns();
        foreach (AddOnPtr addOn, addOnList) {
            if (mAddOnMap.contains(addOn->name()))
                continue;

            addOn->engine = this;
            mAddOnMap[addOn->name()] = addOn;
        }
    }
}

void Engine::processAddOnsAtStart()
{
    foreach (const QString &name, availableAddOns()) {
        AddOnPtr addOnPtr = mAddOnMap[name];

        if (addOnPtr->type() & AddOnProcess && addOnPtr->loadFlags() & AddOnExecAtStart)
            addOnPtr->newProcess()->run();
    }
}

void Engine::processAddOnsBeforeLogin()
{
    foreach (const QString &name, availableAddOns()) {
        AddOnPtr addOnPtr = mAddOnMap[name];

        if (addOnPtr->type() & AddOnProcess && addOnPtr->loadFlags() & AddOnExecBeforeLogin)
            addOnPtr->newProcess()->run();
    }
}

void Engine::processAddOnsAfterLogin()
{
    foreach (const QString &name, availableAddOns()) {
        AddOnPtr addOnPtr = mAddOnMap[name];

        if (addOnPtr->permission() != 0 && !(addOnPtr->permission() & mUser.permission))
            continue;

        if (addOnPtr->type() & AddOnProcess && addOnPtr->loadFlags() & AddOnExecAfterLogin)
            addOnPtr->newProcess()->run();
    }
}

bool Engine::login(const QString &uname, const QString &pwd)
{
    if (uname.isEmpty())
        return false;

    QSqlQuery query(mDatabase);
    query.exec("SELECT `value` FROM `info` WHERE `key`='db.version'");

    QString version;
    if (query.next())
        version = query.value(0).toString();

    query.prepare("SELECT `password`, `id`, `fullname`, `group` FROM `users` WHERE `username`=?");
    query.addBindValue(uname);
    query.exec();

    if (!query.next())
        return false;

    QString password = query.value(0).toString();
    int id = query.value(1).toInt();
    QString fullname = query.value(2).toString();
    int group = query.value(3).toInt();

    bool success = false;
    if (version == "4.8") {
        if (password == pwd)
            success = true;
    }
    else if (version == "5.0") {
        // SHA256 lebih aman dari SHA1 untuk hashing password
        QString passHash = QCryptographicHash::hash(QString(uname + ":" + pwd).toUtf8(), QCryptographicHash::Sha256).toHex();
        if (password == passHash)
            success = true;
    }

    if (!success)
        return false;

    mUser.id = id;
    mUser.username = uname;
    mUser.fullname = fullname;
    mUser.group = group;

    if (mUser.group == GroupAdministrator)
        mUser.permission = AddOnAdministrators;
    else if (mUser.group == GroupUser)
        mUser.permission = AddOnUsers;
    else if (mUser.group == GroupGuest)
        mUser.permission = AddOnGuest;

    return true;
}

void Engine::logout()
{
    mUser = User();
    emit loggedOut();
}

void Engine::quit()
{
    mRun = false;
    emit quitted();
    qApp->quit();
}

bool Engine::isRunning()
{
    return mRun;
}

void Engine::setKantor(const QString &kantor)
{
    mKantor = data()->kantor(kantor);
}

void Engine::setKantor(const Kantor &kantor)
{
    mKantor = kantor;
}

bool Engine::load()
{
    static_cast<CData *>(data())->load();

    QString kodeKantor = databaseSettings()->value(IDS_SERVER_KANTOR_KODE).toString();
    if (kodeKantor.isEmpty()) {
        do {
            QMessageBox::information(nullptr, "Information", "Detail Kantor belum diatur. Silahkan atur terlebih dahulu.");
            SettingsDialog dialog;
            if (dialog.exec() == QDialog::Rejected)
                return false;
        } while (mKantor.kode.isEmpty());
    }
    else
        setKantor(kodeKantor);

    return true;
}

DatabasePtr Engine::database()
{
    QString connectionName = common()->randomString(16);

    QSqlDatabase db = QSqlDatabase::addDatabase(mDatabase.driverName(), connectionName);
    db.setHostName(mDatabase.hostName());
    db.setDatabaseName(mDatabase.databaseName());
    db.setUserName(mDatabase.userName());
    db.setPassword(mDatabase.password());
    db.setConnectOptions(mDatabase.connectOptions());
    db.open();

    return DatabasePtr(new CDatabase(db));
}

QString Engine::version()
{
    return IDD_VERSION;
}

QString Engine::workingDirectory()
{
    return qApp->applicationDirPath();
}

CoreData *Engine::data()
{
    if (mUser.username.isEmpty())
        return nullptr;

    static CData cdata;
    return &cdata;
}

Common *Engine::common()
{
    static CCommon common;
    return &common;
}

QSettings *Engine::settings()
{
    return &mSettings;
}

Settings *Engine::databaseSettings()
{
    if (mUser.username.isEmpty())
        return nullptr;

    static DatabaseSettings dbSettings(this);
    return &dbSettings;
}

Settings *Engine::databaseInfo()
{
    if (mUser.username.isEmpty())
        return nullptr;

    static DatabaseInfo dbInfo(this);
    return &dbInfo;
}

AddOnPtr Engine::addOn(const QString &name)
{
    if (mAddOnMap.contains(name))
        return mAddOnMap[name];

    return AddOnPtr();
}

QStringList Engine::availableAddOns()
{
    return mAddOnMap.keys();
}

void Engine::runSync(Runnable *runnable)
{
    runnable->setAutoDelete(true);
    QPointer<Runnable> ptr(runnable);
    QThreadPool::globalInstance()->start(runnable);

    while (!ptr.isNull()) {
        qApp->processEvents();
        QThread::msleep(10);
    }
}

void Engine::runAsync(Runnable *runnable)
{
    runnable->setAutoDelete(true);
    QThreadPool::globalInstance()->start(runnable);
}
