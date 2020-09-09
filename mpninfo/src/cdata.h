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
public:
    CRunnable(CDataPrivate *_data, const QStringList &_nameList) : data(_data), nameList(_nameList) {}

    void run();

private:
    CDataPrivate *data;
    QStringList nameList;
};

class CData : public CoreData
{
public:
    CData() {}

    Kantor kantor(const QString &kode) const;
    const KantorMap &kantorMap();

    KantorList kanwilList();
    KantorList kppList();
    KantorList kppList(const QString &kanwil);

    Seksi seksi(const QString &kodeKantor, int id) const;
    SeksiList seksiList(const QString &kodeKantor);

    Pegawai pegawai(const QString &kodeKantor, int tahun, const QString &nip) const;
    PegawaiList pegawaiList(const QString &kodeKantor, int tahun);

    WajibPajak wajibPajak(const QString &npwp) const;
    QList<WajibPajak> wajibPajak(const QString &npwp, const QString &kpp = QString(), const QString &cabang = QString());
    const WajibPajakHash &wajibPajakHash();

    void load(const QStringList &nameList = QStringList());

    User user(int id);
    User user(const QString &nip);
    QList<User> userList();

    bool addUser(const QString &username, const QString &password, const QString &fullname, int group);
    bool deleteUser(int id);
    bool deleteUser(const QString &username);
    bool updateUser(int id, const QString &username, const QString &password, const QString &fullname, int group);

    bool addSeksi(const QString &kodeKantor, int type, const QString &nama, const QString &kode, const QString &telp);
    bool updateSeksi(int id, const QString &kodeKantor, int type, const QString &nama, const QString &kode, const QString &telp);
    bool removeSeksi(int id, const QString &kodeKantor);

    bool addPegawai(const QString &kantor, const QString &nip, const QString &nip2, const QString &nama, int seksi, int pangkat, int jabatan, const QString &plh, int tahun);
    bool updatePegawai(const QString &kantor, const QString &nip, const QString &nip2, const QString &nama, int seksi, int pangkat, int jabatan, const QString &plh, int tahun);
    bool deletePegawai(const QString &kantor, const QString &nip, int tahun );

private:
    CDataPrivate mData;
};

#endif // CDATA_H
