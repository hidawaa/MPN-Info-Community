#ifndef CDATA_H
#define CDATA_H

#include <coredata.h>
#include <runnable.h>

struct CDataPrivate {
    KantorMap kantorMap;
    SeksiMap seksiMap;
    PegawaiHash pegawaiHash;
    WajibPajakHash wajibPajakHash;
};

class CRunnable : public Runnable
{
    Q_OBJECT
public:
    CRunnable(CDataPrivate *_data) : data(_data) {}

    void run();

signals:
    void gotMessage(const QString &message);

private:
    CDataPrivate *data;
};

class CData : public CoreData
{
public:
    CData() {}

    void load();

    Kantor kantor(const QString &kode) const;
    const KantorMap &kantorMap();

    KantorList kanwilList();
    KantorList kppList();
    KantorList kppList(const QString &kanwil);

    Seksi seksi(int id) const;
    const SeksiMap &seksiMap();
    SeksiList seksiList();

    Pegawai pegawai(const QString &nip, int tahun = 0) const;
    const PegawaiHash &pegawaiHash();
    PegawaiList pegawaiList();

    WajibPajak wajibPajak(const QString &npwp) const;
    QList<WajibPajak> wajibPajak(const QString &npwp, const QString &kpp = QString(), const QString &cabang = QString());
    const WajibPajakHash &wajibPajakHash();

private:
    CDataPrivate mData;
};

#endif // CDATA_H
