#include "engine.h"

#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QPluginLoader>
#include <QSqlQuery>
#include <QDebug>

#include <interface.h>
#include <window.h>

#include "databaseconnectiondialog.h"
#include "ccommon.h"
#include "cdatabase.h"

const char *chiperPassword = "MpnInfoDatabase";

const KantorMap emptyKantorMap;
const SeksiMap emptySeksiMap;
const PegawaiHash emptyPegawaiHash;
const WajibPajakHash emptyWajibPajakHash;

Engine::Engine(QObject *parent) :
    CoreEngine(parent),
    mSettings("settings.ini", QSettings::IniFormat)
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

            int driver =  mSettings.value("Database/type").toInt();
            mDatabaseConnectionName = common()->generateRandomId(16);

            QSqlDatabase db;
            if (driver == DatabaseMysql) {
                QString driverName = "QMYSQL";
                QString hostName =  mSettings.value("Database/host").toString();
                QStringList authList = common()->decrypt(mSettings.value("Database/auth").toString()).split(";");
                QString databaseName = authList[0];
                QString username = authList.size() > 1? authList[1] : QString();
                QString password = authList.size() > 2? authList[2] : QString();
                QString options =  mSettings.value("Database/options").toString();

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
                QString databaseName = mSettings.value("Database/name").toString();
                if (databaseName.isEmpty())
                    databaseName = "test.db";

                bool encrypt = mSettings.value("Database/encrypt").toBool();
                QString driverName = encrypt? "QSQLCIPHER" : "QSQLITE";
                QString options =  mSettings.value("Database/options").toString();

                db = QSqlDatabase::addDatabase(driverName, mDatabaseConnectionName);
                db.setDatabaseName(databaseName);
                db.setConnectOptions(options);
                db.open();
            }
            else if (driver == DatabaseOther) {
                QString driverName =  mSettings.value("Database/driver").toString();
                QString hostName =  mSettings.value("Database/host").toString();
                QStringList authList = common()->decrypt(mSettings.value("Database/auth").toString()).split(";");
                QString databaseName = authList[0];
                QString username = authList.size() > 1? authList[1] : QString();
                QString password = authList.size() > 2? authList[2] : QString();
                QString options =  mSettings.value("Database/options").toString();

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

            mSettings.setValue("Database/type", dialog.driver());
            if (dialog.driver() == DatabaseMysql) {
                QString auth = QString("%1;%2;%3").arg(dialog.database(), dialog.username(), dialog.password());

                mSettings.setValue("Database/host", dialog.hostname());
                mSettings.setValue("Database/auth", common()->encrypt(auth));
                mSettings.setValue("Database/options", dialog.options());

                mSettings.remove("Database/driver");
            }
            else if (dialog.driver() == DatabaseSqlite) {
                bool encrypt = false;
                if (dialog.driverName() == "QSQLCIPHER")
                    encrypt = true;

                if (encrypt)
                    mSettings.setValue("Database/encrypt", true);
                else
                    mSettings.remove("Database/encrypt");

                mSettings.setValue("Database/name", dialog.database());
                mSettings.setValue("Database/options", dialog.options());

                mSettings.remove("Database/driver");
                mSettings.remove("Database/host");
                mSettings.remove("Database/auth");
            }
            else {
                QString auth = QString("%1;%2;%3").arg(dialog.database(), dialog.username(), dialog.password());

                mSettings.setValue("Database/driver", dialog.driverName());
                mSettings.setValue("Database/host", dialog.hostname());
                mSettings.setValue("Database/auth", common()->encrypt(auth));
                mSettings.setValue("Database/options", dialog.options());
            }
        } while (true);
    }

    return mDatabase.isOpen();
}

void Engine::loadData()
{

}

void Engine::loadAddons()
{
    QDir pluginsDir("addons");
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

bool Engine::login(const QString &username, const QString &password)
{
    if (username.isEmpty())
        return false;

    bool success = true;
    if (success) {
        mUser.username = username;
        mUser.group = GroupAdministrator;

        if (mUser.group == GroupAdministrator)
            mUser.permission = AddOnAdministrators;
        else if (mUser.group == GroupUser)
            mUser.permission = AddOnUsers;
        else if (mUser.group == GroupGuest)
            mUser.permission = AddOnGuest;
    }
    return success;
}

DatabasePtr Engine::database()
{
    QString connectionName = common()->generateRandomId(16);

    QSqlDatabase db = QSqlDatabase::addDatabase(mDatabase.driverName(), connectionName);
    db.setHostName(mDatabase.hostName());
    db.setDatabaseName(mDatabase.databaseName());
    db.setUserName(mDatabase.userName());
    db.setPassword(mDatabase.password());
    db.setConnectOptions(mDatabase.connectOptions());
    db.open();

    return DatabasePtr(new CDatabase(db));
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

DatabaseSettings *Engine::databaseSettings()
{
    if (mUser.username.isEmpty())
        return nullptr;

    static DatabaseSettings dbSettings(this);
    return &dbSettings;
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

void Engine::run(void (*cb)(void *), void *data)
{
    cb(data);
}

Kantor Engine::kantor(const QString &kode) const
{
    if (mUser.username.isEmpty())
        return Kantor();

    return mKantorMap[kode];
}

const KantorMap &Engine::kantorMap()
{
    if (mUser.username.isEmpty())
        return emptyKantorMap;

    return mKantorMap;
}

KantorList Engine::kanwilList()
{
    KantorList list;
    if (mUser.username.isEmpty())
        return list;

    QMapIterator<QString, Kantor> iterator(mKantorMap);
    while (iterator.hasNext()) {
        iterator.next();

        if (iterator.value().kpp.isEmpty())
            list << iterator.value();
    }

    return list;
}

KantorList Engine::kppList()
{
    KantorList list;
    if (mUser.username.isEmpty())
        return list;

    QMapIterator<QString, Kantor> iterator(mKantorMap);
    while (iterator.hasNext()) {
        iterator.next();

        if (!iterator.value().kpp.isEmpty())
            list << iterator.value();
    }

    return list;
}

KantorList Engine::kppList(const QString &kanwil)
{
    KantorList list;
    if (mUser.username.isEmpty())
        return list;

    foreach (const Kantor &kantor, kppList()) {
        if (kantor.kanwil == kanwil)
            list << kantor;
    }

    return list;
};

Seksi Engine::seksi(int id) const
{
    if (mUser.username.isEmpty())
        return Seksi();

    return mSeksiMap[id];
}

const SeksiMap &Engine::seksiMap()
{
    if (mUser.username.isEmpty())
        return emptySeksiMap;

    return mSeksiMap;
}

SeksiList Engine::seksiList()
{
    SeksiList list;
    if (mUser.username.isEmpty())
        return list;

    QMapIterator<int, Seksi> iterator(mSeksiMap);
    while (iterator.hasNext()) {
        iterator.next();

        list << iterator.value();
    }

    return list;
}

Pegawai Engine::pegawai(const QString &nip) const
{
    if (mUser.username.isEmpty())
        return Pegawai();

    return mPegawaiHash[nip];
}

const PegawaiHash &Engine::pegawaiHash()
{
    if (mUser.username.isEmpty())
        return emptyPegawaiHash;

    return mPegawaiHash;
}

PegawaiList Engine::pegawaiList()
{
    PegawaiList list;
    if (mUser.username.isEmpty())
        return list;

    QHashIterator<QString, Pegawai> iterator(mPegawaiHash);
    while (iterator.hasNext()) {
        iterator.next();

        list << iterator.value();
    }

    return list;
}

WajibPajak Engine::wajibPajak(const QString &npwp) const
{
    if (mUser.username.isEmpty())
        return WajibPajak();

    if (mWajibPajakHash.contains(npwp))
        return mWajibPajakHash[npwp];

    return WajibPajak();
}

QList<WajibPajak> Engine::wajibPajak(const QString &npwp, const QString &kpp, const QString &cabang)
{
    QList<WajibPajak> wpList;
    if (mUser.username.isEmpty())
        return wpList;

    QString npwpfull = npwp + kpp + cabang;
    foreach (const QString &key, emptyWajibPajakHash.keys()) {
        if (key.left(npwpfull.size()) == npwpfull)
            wpList << mWajibPajakHash[key];
    }

    return wpList;
}

const WajibPajakHash &Engine::wajibPajakHash()
{
    if (mUser.username.isEmpty())
        return emptyWajibPajakHash;

    return mWajibPajakHash;
}

