#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>

#include <coreengine.h>
#include <interface.h>

class Engine : public CoreEngine
{
    Q_OBJECT
public:
    ~Engine();

    static Engine *instance();

    bool connectDatabase();
    void updateDatabase();

    void loadData();
    void loadPlugins();

    bool login(const QString &username, const QString &password);

    QString encrypt(const QString &value);
    QString decrypt(const QString &value);

    QSettings *settings();
    QSqlDatabase database();

    User user() { return mUser; }

    AddOnPtr addOn(const QString &name);
    QStringList availableAddOns();

    void setWindow(Window *window) { mWindow = window; }
    Window *window() { return mWindow; }

    Pegawai pegawai(const QString &nip) const;
    const PegawaiHash &pegawaiHash();

    WajibPajak wajibPajak(const QString &npwp) const;
    QList<WajibPajak> wajibPajak(const QString &npwp, const QString &kpp = QString(), const QString &cabang = QString());
    const WajibPajakHash &wajibPajakHash();

private:
    static const PegawaiHash emptyPegawaiHash;
    static const WajibPajakHash emptyWajibPajakHash;

    Engine(QObject *parent = nullptr);
    QSettings mSettings;
    QString mDatabaseConnection;
    QSqlDatabase mDatabase;
    User mUser;
    PegawaiHash mPegawaiHash;
    WajibPajakHash mWajibPajakHash;
    AddOnHash mAddOnMap;
    Window *mWindow;
};

#endif // ENGINE_H
