#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>

#include <coreengine.h>
#include <interface.h>

#include "databasesettings.h"

class Window;
class Engine : public CoreEngine
{
    Q_OBJECT
public:
    ~Engine();

    static Engine *instance();

    bool connectDatabase();

    void loadData();
    void loadAddons();

    void processAddOnsAtStart();
    void processAddOnsBeforeLogin();
    void processAddOnsAfterLogin();

    bool login(const QString &username, const QString &password);

    Common *common();
    DatabasePtr database();

    QSettings *settings();
    DatabaseSettings *databaseSettings();

    AddOnPtr addOn(const QString &name);
    QStringList availableAddOns();

    void setWindow(Window *window) { mWindow = window; }
    Window *window() { return mWindow; }

    void run(void (* foo)(void *), void *data = nullptr);

    User user() { return mUser; }

    Kantor kantor(const QString &kode) const;
    const KantorMap &kantorMap();

    KantorList kanwilList();
    KantorList kppList();
    KantorList kppList(const QString &kanwil);

    Seksi seksi(int id) const;
    const SeksiMap &seksiMap();
    SeksiList seksiList();

    Pegawai pegawai(const QString &nip) const;
    const PegawaiHash &pegawaiHash();
    PegawaiList pegawaiList();

    WajibPajak wajibPajak(const QString &npwp) const;
    QList<WajibPajak> wajibPajak(const QString &npwp, const QString &kpp = QString(), const QString &cabang = QString());
    const WajibPajakHash &wajibPajakHash();

private:
    Engine(QObject *parent = nullptr);
    QSettings mSettings;
    QString mDatabaseConnectionName;
    QSqlDatabase mDatabase;
    User mUser;
    Kantor mKantor;
    KantorMap mKantorMap;
    SeksiMap mSeksiMap;
    PegawaiHash mPegawaiHash;
    WajibPajakHash mWajibPajakHash;
    AddOnHash mAddOnMap;
    Window *mWindow;
};

#endif // ENGINE_H
