#ifndef COREENGINE_H
#define COREENGINE_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QVariantMap>

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

struct Jabatan {
    int id { -1 };
    QString nama;
};

struct Seksi {
    int id { -1 };
    Jabatan jabatan;
};

struct Pegawai {
    QString nip;
    QString nama;
    Seksi seksi;
    Jabatan jabatan;
};

typedef QHash<QString, Pegawai> PegawaiHash;

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

class Window;
class CoreEngine : public QObject
{
public:
    CoreEngine(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~CoreEngine() {}
    virtual QString encrypt(const QString &value) = 0;
    virtual QString decrypt(const QString &value) = 0;

    virtual QSettings *settings() = 0;
    virtual QSqlDatabase database() = 0;

    virtual AddOnPtr addOn(const QString &name) = 0;
    virtual QStringList availableAddOns() = 0;
    virtual Window *window() = 0;

    virtual Pegawai pegawai(const QString &nip) const = 0;
    virtual const PegawaiHash &pegawaiHash() = 0;

    virtual WajibPajak wajibPajak(const QString &npwp) const = 0;
    virtual QList<WajibPajak> wajibPajak(const QString &npwp, const QString &kpp = QString(), const QString &cabang = QString()) = 0;
    virtual const WajibPajakHash &wajibPajakHash() = 0;
};

#endif // COREENGINE_H
