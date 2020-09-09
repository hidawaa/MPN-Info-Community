#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>

#include <coreengine.h>
#include <interface.h>

#define IDS_GENERAL_LAST_VERSION                                "lastVersion"
#define IDS_GENERAL_LAST_USER                                   "lastUser"
#define IDS_GENERAL_DOWNLOAD_DIR                                "downloadDir"
#define IDS_GENERAL_DOWNLOAD_TIMEOUT                            "downloadTimeout"
#define IDS_GENERAL_DEFAULT_PAPER_SIZE                          "paperSize"
#define IDS_GENERAL_DEBUG                                       "debug"

#define IDS_NOTAHITUNG_USE_DATA_PELAPORAN                       "NotaHitung/useDataPelaporan"
#define IDS_NOTAHITUNG_SKIP_PRINT_PREVIEW                       "NotaHitung/skipPrintPreview"

#define IDS_UPDATE_ONLINE                                       "Update/online"
#define IDS_UPDATE_CHANNEL                                      "Update/channel"
#define IDS_UPDATE_HOST                                         "Update/host"

#define IDS_DATABASE_TYPE                                       "Database/type"
#define IDS_DATABASE_DRIVER                                     "Database/driver"
#define IDS_DATABASE_HOST                                       "Database/host"
#define IDS_DATABASE_PORT                                       "Database/port"
#define IDS_DATABASE_NAME                                       "Database/name"
#define IDS_DATABASE_OPTIONS                                    "Database/options"
#define IDS_DATABASE_ENCRYPTED                                  "Database/encrypt"
#define IDS_DATABASE_AUTH                                       "Database/auth"

#define IDS_APPPORTAL_AUTO_LOGIN                                "AppPortal/autoLogin"
#define IDS_APPPORTAL_REMEMBER                                  "AppPortal/remember"
#define IDS_APPPORTAL_HOST                                      "AppPortal/host"
#define IDS_APPPORTAL_AUTH                                      "AppPortal/auth"

#define IDS_SIKKA_AUTO_LOGIN                                    "Sikka/autoLogin"
#define IDS_SIKKA_REMEMBER                                      "Sikka/remember"
#define IDS_SIKKA_AUTH                                          "Sikka/auth"

#define IDS_SIDJP_AUTO_LOGIN                                    "SIDJP/autoLogin"
#define IDS_SIDJP_REMEMBER                                      "SIDJP/remember"
#define IDS_SIDJP_HOST                                          "SIDJP/host"
#define IDS_SIDJP_AUTH                                          "SIDJP/auth"

#define IDS_APPROWEB_AUTO_LOGIN                                 "Approweb/autoLogin"
#define IDS_APPROWEB_REMEMBER                                   "Approweb/remember"
#define IDS_APPROWEB_AUTH                                       "Approweb/auth"

#define IDS_NETWORK_USE_PROXY                                   "Network/useProxy"
#define IDS_NETWORK_PROXY_HOST                                  "Network/proxyHost"
#define IDS_NETWORK_PROXY_PORT                                  "Network/proxyPort"
#define IDS_NETWORK_PROXY_AUTH                                  "Network/proxyAuth"

#define IDS_INTERNET_USE_PROXY                                  "Internet/useProxy"
#define IDS_INTERNET_PROXY_HOST                                 "Internet/proxyHost"
#define IDS_INTERNET_PROXY_PORT                                 "Internet/proxyPort"
#define IDS_INTERNET_PROXY_AUTH                                 "Internet/proxyAuth"

#define IDS_DOWNLOADER_DATABASE_LOKAL_DRIVER                    "Downloader/dbLokalDriver"
#define IDS_DOWNLOADER_IMPORT_DATABASE_LOKAL                    "Downloader/dbLokalImport"
#define IDS_DOWNLOADER_IMPORT_DATABASE_LOKAL_DATE_TYPE          "Downloader/dbLokalTipe"
#define IDS_DOWNLOADER_IMPORT_DATABASE_LOKAL_JENIS_PAJAK        "Downloader/dbLokalJenisPajak"
#define IDS_DOWNLOADER_BULAN                                    "Downloader/bulan"

#define IDS_URL_SIDJP                                           "Url/sidjp"
#define IDS_URL_APPPORTAL                                       "Url/appportal"
#define IDS_URL_SIKKA                                           "Url/sikka"

#define IDS_DATABASE_OCI_AUTH                                   "OCI/auth"
#define IDS_DATABASE_ODBC_AUTH                                  "ODBC/auth"

#define IDS_SERVER_KANTOR_KODE                                  "kantor.kode"
#define IDS_SERVER_KANTOR_WPJ                                   "kantor.wpj"
#define IDS_SERVER_KANTOR_KP                                    "kantor.kp"
#define IDS_SERVER_KANTOR_ALAMAT                                "kantor.alamat"
#define IDS_SERVER_KANTOR_TELEPON                               "kantor.telepon"
#define IDS_SERVER_KANTOR_KOTA                                  "kantor.kota"
#define IDS_SERVER_DOWNLAOD_SPLIT                               "download.split"
#define IDS_SERVER_DOWNLOAD_LOGIN_AS_GUEST                      "download.loginAsGuest"
#define IDS_SERVER_DOWNLOAD_AUTO_IMPORT                         "download.autoImport"
#define IDS_SERVER_DOWNLAOD_LAST_IMPORT_DATABASE_LOKAL          "download.lastImportDbLok"
#define IDS_SERVER_MAPS_DEFAULT_POSITION                        "maps.defaultPosition"
#define IDS_SERVER_MAPS_DEFAULT_ZOOM                            "maps.defaultZoom"
#define IDS_SERVER_SHARED_DATA_FOLDER_PATH                      "shareddata.path"
#define IDS_SERVER_SHARED_DATA_AS_TEMPLATE                      "shareddata.asTemplate"

class Engine : public CoreEngine
{
    Q_OBJECT
public:
    ~Engine();

    static Engine *instance();

    bool connectDatabase();
    bool load();

    void loadAddons();

    void processAddOnsAtStart();
    void processAddOnsBeforeLogin();
    void processAddOnsAfterLogin();

    bool login(const QString &username, const QString &password);
    void logout();
    void quit();

    bool isRunning();

    void setKantor(const QString &kantor);
    void setKantor(const Kantor &kantor);

    QString workingDirectory();

    const User &user() { return mUser; }
    const Kantor &kantor() { return mKantor; }

    CoreData *data();
    Common *common();
    DatabasePtr database();

    QSettings *settings();
    Settings *databaseSettings();
    Settings *databaseInfo();

    AddOnPtr addOn(const QString &name);
    QStringList availableAddOns();

    void setWindow(Window *window) { mWindow = window; }
    Window *window() { return mWindow; }

    void runSync(Runnable *runnable);

private:
    Engine(QObject *parent = nullptr);
    QSettings mSettings;
    QString mDatabaseConnectionName;
    QSqlDatabase mDatabase;
    User mUser;
    Kantor mKantor;
    Window *mWindow;
    AddOnHash mAddOnMap;
    bool mRun { true };
};

#endif // ENGINE_H
