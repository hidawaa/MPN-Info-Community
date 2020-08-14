#include "engine.h"

#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QPluginLoader>
#include <QDebug>

#include "simplecrypt.h"
#include "databasesettingdialog.h"
#include "interface.h"

const quint64 cryptoNumber(0x032ab4a5acb3a033);

const PegawaiHash Engine::emptyPegawaiHash;
const WajibPajakHash Engine::emptyWajibPajakHash;

QString generateRandomId(int length)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for(int i=0; i<length; ++i)
    {
        int index = int(rand() % possibleCharacters.length());
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

Engine::Engine(QObject *parent) :
    CoreEngine(parent),
    mSettings("settings.ini", QSettings::IniFormat)
{
}

Engine::~Engine() {

}

Engine *Engine::instance()
{
    static Engine engine;
    return &engine;
}

QString Engine::encrypt(const QString &value)
{
    SimpleCrypt crypto(cryptoNumber);
    return crypto.encryptToString(value);
}

QString Engine::decrypt(const QString &value)
{
    SimpleCrypt crypto(cryptoNumber);
    return crypto.decryptToString(value);
}

bool Engine::connectDatabase()
{
    {
        DatabaseSettingDialog dialog;
        do {
            QSqlDatabase::removeDatabase(mDatabaseConnection);

            int driver =  mSettings.value("Database/type").toInt();
            mDatabaseConnection = generateRandomId(16);

            QSqlDatabase db;
            QString driverName;
            if (driver == DatabaseMysql) {
                driverName = "QMYSQL";
                QString hostName =  mSettings.value("Database/host").toString();
                QStringList authList = decrypt(mSettings.value("Database/auth").toString()).split(";");
                QString databaseName = authList[0];
                QString username = authList.size() > 1? authList[1] : QString();
                QString password = authList.size() > 2? authList[2] : QString();

                db = QSqlDatabase::addDatabase(driverName, mDatabaseConnection);
                db.setHostName(hostName);
                db.setDatabaseName(databaseName);
                db.setUserName(username);
                db.setPassword(password);
                db.open();
            }
            else {
                QString databaseName = mSettings.value("Database/data").toString();
                if (databaseName.isEmpty())
                    databaseName = "data.db";

                bool encrypt = mSettings.value("Database/encrypt").toBool();
                driverName = encrypt? "QSQLCIPHER" : "QSQLITE";

                db = QSqlDatabase::addDatabase(driverName, mDatabaseConnection);
                db.setDatabaseName(databaseName);
                db.open();
            }

            if (db.isOpen()) {
                mDatabase = db;
                break;
            }

            if (dialog.exec() == QDialog::Rejected)
                break;

            mSettings.setValue("Database/type", dialog.driver());
            if (dialog.driver() == DatabaseMysql) {
                QString auth = QString("%1;%2;%3").arg(dialog.database(), dialog.username(), dialog.password());

                mSettings.setValue("Database/host", dialog.hostname());
                mSettings.setValue("Database/auth", encrypt(auth));
            }
            else {
                bool encrypt = false;
                QFile dbFile("data.db");
                if (dbFile.exists())
                {
                    int result = QMessageBox::question(nullptr, "Encrypt Database", "Database sudah tersedia. Apakah database tersebut terencrypt?", QMessageBox::Yes | QMessageBox::No);
                    if (result == QMessageBox::Yes)
                        encrypt = true;
                }
                else
                {
                    int result = QMessageBox::question(nullptr, "Encrypt Database", "Apakah Anda ingin mengencrypt database?", QMessageBox::Yes | QMessageBox::No);
                    if (result == QMessageBox::Yes)
                        encrypt = true;
                }

                if (encrypt)
                    mSettings.setValue("Database/encrypt", true);
                else
                    mSettings.remove("Database/encrypt");

                mSettings.setValue("Database/data", "data.db");
                mSettings.remove("Database/host");
                mSettings.remove("Database/auth");
            }
        } while (true);
    }

    return mDatabase.isOpen();
}

void Engine::updateDatabase()
{

}

void Engine::loadData()
{

}

void Engine::loadPlugins()
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

bool Engine::login(const QString &username, const QString &password)
{
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

QSettings *Engine::settings()
{
    return &mSettings;
}

QSqlDatabase Engine::database()
{
    if (mUser.username.isEmpty())
        return QSqlDatabase();

    return mDatabase;
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

Pegawai Engine::pegawai(const QString &nip) const
{
    if (mUser.username.isEmpty())
        return mPegawaiHash[nip];

    return Pegawai();
}

const PegawaiHash &Engine::pegawaiHash()
{
    if (mUser.username.isEmpty())
        return emptyPegawaiHash;

    return mPegawaiHash;
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

