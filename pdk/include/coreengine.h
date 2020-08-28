#ifndef COREENGINE_H
#define COREENGINE_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QVariantMap>

#include "database.h"
#include "common.h"
#include "window.h"
#include "interface.h"

enum DatabaseTypes {
    DatabaseUnknown = 0,
    DatabaseMysql,
    DatabaseSqlite,
    DatabaseOther
};

enum GroupTypes {
    GroupUnknown = -1,
    GroupAdministrator,
    GroupUser,
    GroupGuest
};

enum JabatanTypes {
    JabatanUnknown = -1,
    JabatanKepalaKantor,
    JabatanKepalaSeksi,
    JabatanFungsionalPemeriksa,
    JabatanOperatorConsole,
    JabatanAccountRepresentativePelayanan,
    JabatanAccountRepresentativePengawasan,
    JabatanPelaksana
};

enum SeksiTypes {
    SeksiUnknown = -1,
    SeksiKepalaKantor,
    SeksiSubbagianUmum,
    SeksiPengolahanDataDanInformasi,
    SeksiPelayanan,
    SeksiPenagihan,
    SeksiPengawasanDanKonsultasiPelayanan,
    SeksiPengawasanDanKonsultasiPengawasan,
    SeksiEkstensifikasiPerpajakan,
    SeksiPemeriksaanDanKepatuhanInternal
};

struct User {
    QString username;
    int group { -1 };
    int permission { 0 };
};

struct Kantor {
    QString kanwil;
    QString kpp;
    QString nama;
};

typedef QMap<QString, Kantor> KantorMap;
typedef QList<Kantor> KantorList;

struct Jabatan {
    int id { -1 };
    QString nama;
};

struct Seksi {
    int id { -1 };
    int type { -1 };
    QString nama;
};

typedef QMap<int, Seksi> SeksiMap;
typedef QList<Seksi> SeksiList;

struct Pegawai {
    QString nip;
    QString nama;
    Seksi seksi;
    Jabatan jabatan;
};

typedef QHash<QString, Pegawai> PegawaiHash;
typedef QList<Pegawai> PegawaiList;

struct WajibPajak {
    QString npwp;
    QString kpp;
    QString cabang;
    QString nama;
    QString alamat;
    QString kelurahan;
    QString kecamatan;
    QString kota;
    QString propinsi;
    QString jenis;
    QString bhkm;
    QString status;
    QString klu;
    QString nik;
    QString telp;
    QString tgl_daftar;
    QString tgl_pkp;
    QString tgl_pkp_cabut;
    QString ar;
    QString eks;
    QString js;
};

typedef QHash<QString, WajibPajak> WajibPajakHash;

class Settings
{
public:
    virtual ~Settings() {}

    virtual void setValue(const QString &key, const QVariant &value) = 0;
    virtual QVariant value(const QString &key) = 0;
};

class CoreEngine : public QObject
{
public:
    CoreEngine(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~CoreEngine() {}

    virtual DatabasePtr database() = 0;

    virtual QSettings *settings() = 0;
    virtual Settings *databaseSettings() = 0;

    virtual AddOnPtr addOn(const QString &name) = 0;
    virtual QStringList availableAddOns() = 0;

    virtual Common *common() = 0;
    virtual Window *window() = 0;

    virtual void run(void (* foo)(void *), void *data = nullptr) = 0;

    virtual Kantor kantor(const QString &kode) const = 0;
    virtual const KantorMap &kantorMap() = 0;

    virtual KantorList kanwilList() = 0;
    virtual KantorList kppList() = 0;
    virtual KantorList kppList(const QString &kanwil) = 0;

    virtual Seksi seksi(int id) const = 0;
    virtual const SeksiMap &seksiMap() = 0;
    virtual SeksiList seksiList() = 0;

    virtual Pegawai pegawai(const QString &nip) const = 0;
    virtual const PegawaiHash &pegawaiHash() = 0;
    virtual PegawaiList pegawaiList() = 0;

    virtual WajibPajak wajibPajak(const QString &npwp) const = 0;
    virtual QList<WajibPajak> wajibPajak(const QString &npwp, const QString &kpp = QString(), const QString &cabang = QString()) = 0;
    virtual const WajibPajakHash &wajibPajakHash() = 0;
};

#endif // COREENGINE_H
