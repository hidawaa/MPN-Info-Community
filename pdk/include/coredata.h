#ifndef COREDATA_H
#define COREDATA_H

#include <QString>
#include <QDate>
#include <QMap>
#include <QHash>

struct User {
    int id { 0 };
    QString username;
    QString fullname;
    int group { -1 };
    int permission { 0 };
};

struct Kantor {
    int type { -1 };
    QString kode;
    QString kanwil;
    QString nama;
};

typedef QMap<QString, Kantor> KantorMap;
typedef QList<Kantor> KantorList;

struct Jabatan {
    int type { -1 };
    QString nama;
};

struct Pangkat {
    int type { -1 };
    QString nama;
};

struct Seksi {
    int id { -1 };
    int type { -1 };
    Kantor kantor;
    QString nama;
    QString kode;
    QString telp;
};

typedef QMap<QString, Seksi> SeksiMap;
typedef QList<Seksi> SeksiList;

struct Pegawai {
    QString nip;
    QString nip2;
    QString nama;
    Kantor kantor;
    Seksi seksi;
    Jabatan jabatan;
    Pangkat pangkat;
    int tahun { -1 };
    QString plh;
};

typedef QHash<QString, Pegawai> PegawaiHash;
typedef QList<Pegawai> PegawaiList;

struct WajibPajak {
    QString admin;
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
    QDate tgl_daftar;
    QDate tgl_pkp;
    QDate tgl_pkp_cabut;
    QString ar;
    QString eks;
    QString js;
    bool valid { false };
};

typedef QHash<QString, WajibPajak> WajibPajakHash;
typedef QList<WajibPajak> WajibPajakList;

class CoreData
{
public:
    virtual ~CoreData() {}

    virtual Kantor kantor(const QString &kode) const = 0;
    virtual const KantorMap &kantorMap() = 0;

    virtual KantorList kanwilList() = 0;
    virtual KantorList kppList() = 0;
    virtual KantorList kppList(const QString &kanwil) = 0;

    virtual Seksi seksi(const QString &kodeKantor, int id) const = 0;
    virtual SeksiList seksiList(const QString &kodeKantor) = 0;

    virtual Pegawai pegawai(const QString &kodeKantor, int tahun, const QString &nip) const = 0;
    virtual PegawaiList pegawaiList(const QString &kodeKantor, int tahun) = 0;

    virtual WajibPajak wajibPajak(const QString &npwp) const = 0;
    virtual QList<WajibPajak> wajibPajak(const QString &npwp, const QString &kpp = QString(), const QString &cabang = QString()) = 0;
    virtual const WajibPajakHash &wajibPajakHash() = 0;
};

#endif // COREDATA_H
