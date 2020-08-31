#include "cdata.h"

#include <QObject>
#include <runnable.h>

#include "engine.h"

void CRunnable::run() {
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    (*data).kantorMap.clear();
    (*data).seksiMap.clear();
    (*data).pegawaiHash.clear();
    (*data).wajibPajakHash.clear();

    emit gotMessage("Load data Kantor");
    db->exec("SELECT * FROM `kantor`");
    while (db->next()) {
        QString kanwil = db->value(0).toString();
        QString kpp = db->value(1).toString();
        QString nama = db->value(2).toString();
        QString key = kpp;

        if (kpp == "000") {
            key = kanwil;
            kpp.clear();
        }

        Kantor & kantor = (*data).kantorMap[key];
        kantor.type = kpp.isEmpty()? Kanwil : Kpp;
        kantor.kode = key;
        kantor.kanwil = kanwil;
        kantor.nama = nama;
    }

    emit gotMessage("Load data Seksi");
    db->exec("SELECT * FROM `seksi`");
    while (db->next()) {
        int id = db->value(0).toInt();
        QString kantor = db->value(1).toString();
        int type = db->value(2).toInt();
        QString nama = db->value(3).toString();
        QString kode = db->value(4).toString();
        QString telp = db->value(5).toString();

        Seksi &seksi = (*data).seksiMap[id];
        seksi.id = id;
        seksi.type = type;
        seksi.kantor = (*data).kantorMap[kantor];
        seksi.nama = nama;
        seksi.kode = kode;
        seksi.telp = telp;
    }

    emit gotMessage("Load data Pegawai");
    db->exec("SELECT * FROM `pegawai`");
    while (db->next()) {
        QString nip = db->value(1).toString();
        QString nip2 = db->value(2).toString();
        QString kantor = db->value(0).toString();
        QString nama = db->value(3).toString();
        int pangkatType = db->value(4).toInt();
        int seksiId = db->value(5).toInt();
        int jabatanType = db->value(6).toInt();
        int tahun = db->value(7).toInt();
        QString plh = db->value(8).toString();

        Pangkat pangkat;
        pangkat.type = pangkatType;
        pangkat.nama = engine->common()->namaPangkat(pangkatType);

        Jabatan jabatan;
        jabatan.type = jabatanType;
        jabatan.nama = engine->common()->namaJabatan(jabatanType);

        const Seksi &seksi = (*data).seksiMap[seksiId];

        Pegawai &p = (*data).pegawaiHash[nip + ":" + QString::number(tahun)];
        p.nip = nip;
        p.nip2 = nip2;
        p.kantor = seksi.kantor;
        p.nama = nama;
        p.pangkat = pangkat;
        p.seksi = seksi;
        p.jabatan = jabatan;
        p.plh = plh;
    }

    emit gotMessage("Load data WajibPajak");
    db->exec("SELECT * FROM `wp`");
    while (db->next()) {
        QString admin = db->value(0).toString();
        QString npwp = db->value(1).toString();
        QString kpp = db->value(2).toString();
        QString cabang = db->value(3).toString();
        QString nama = db->value(4).toString();
        QString alamat = db->value(5).toString();
        QString kelurahan = db->value(6).toString();
        QString kecamatan = db->value(7).toString();
        QString kota = db->value(8).toString();
        QString propinsi = db->value(9).toString();
        QString jenis = db->value(10).toString();
        QString bhkm = db->value(11).toString();
        QString status = db->value(12).toString();
        QString klu = db->value(13).toString();
        QDate tgl_daftar = db->value(14).toDate();
        QDate tgl_pkp = db->value(15).toDate();
        QDate tgl_pkp_cabut = db->value(16).toDate();
        QString nik = db->value(17).toString();
        QString telp = db->value(18).toString();
        QString ar = db->value(19).toString();
        QString eks = db->value(20).toString();
        QString js = db->value(21).toString();

        WajibPajak &wp = (*data).wajibPajakHash[npwp + kpp + cabang];
        wp.admin = admin;
        wp.npwp = npwp;
        wp.kpp = kpp;
        wp.cabang = cabang;
        wp.nama = nama;
        wp.alamat = alamat;
        wp.kelurahan = kelurahan;
        wp.kecamatan = kecamatan;
        wp.kota = kota;
        wp.propinsi = propinsi;
        wp.jenis = jenis;
        wp.bhkm = bhkm;
        wp.status = status;
        wp.klu = klu;
        wp.tgl_daftar = tgl_daftar;
        wp.tgl_pkp = tgl_pkp;
        wp.tgl_pkp_cabut = tgl_pkp_cabut;
        wp.nik = nik;
        wp.telp = telp;
        wp.ar = ar;
        wp.eks = eks;
        wp.js = js;
        wp.valid = true;
    }
}

void CData::load()
{
    Engine *engine = Engine::instance();

    ObjectPtr loadingDialog = engine->addOn("dialog_loading")->newObject();
    loadingDialog->exec("setMessage", "Loading Data");
    loadingDialog->exec("show");

    CRunnable *runnable = new CRunnable(&mData);
    QObject::connect(runnable, &CRunnable::gotMessage, [loadingDialog](const QString &message) {
        loadingDialog->exec("setMessage", message);
    });

    engine->runSync(runnable);
}

Kantor CData::kantor(const QString &kode) const
{
    return mData.kantorMap[kode];
}

const KantorMap &CData::kantorMap()
{
    return mData.kantorMap;
}

KantorList CData::kanwilList()
{
    KantorList list;

    QMapIterator<QString, Kantor> iterator(mData.kantorMap);
    while (iterator.hasNext()) {
        iterator.next();

        if (iterator.value().type == Kanwil)
            list << iterator.value();
    }

    return list;
}

KantorList CData::kppList()
{
    KantorList list;
    QMapIterator<QString, Kantor> iterator(mData.kantorMap);
    while (iterator.hasNext()) {
        iterator.next();

        if (iterator.value().type == Kpp)
            list << iterator.value();
    }

    return list;
}

KantorList CData::kppList(const QString &kanwil)
{
    KantorList list;
    foreach (const Kantor &kantor, kppList()) {
        if (kantor.type == Kanwil)
            continue;

        if (kantor.kanwil == kanwil)
            list << kantor;
    }

    return list;
};

Seksi CData::seksi(int id) const
{
    return mData.seksiMap[id];
}

const SeksiMap &CData::seksiMap()
{
    return mData.seksiMap;
}

SeksiList CData::seksiList()
{
    SeksiList list;
    QMapIterator<int, Seksi> iterator(mData.seksiMap);
    while (iterator.hasNext()) {
        iterator.next();

        list << iterator.value();
    }

    return list;
}

Pegawai CData::pegawai(const QString &nip, int tahun) const
{
    if (tahun == 0)
        tahun = QDate::currentDate().year();

    return mData.pegawaiHash[nip + ":" + QString::number(tahun)];
}

const PegawaiHash &CData::pegawaiHash()
{
    return mData.pegawaiHash;
}

PegawaiList CData::pegawaiList()
{
    PegawaiList list;
    QHashIterator<QString, Pegawai> iterator(mData.pegawaiHash);
    while (iterator.hasNext()) {
        iterator.next();

        list << iterator.value();
    }

    return list;
}

WajibPajak CData::wajibPajak(const QString &npwp) const
{
    if (mData.wajibPajakHash.contains(npwp))
        return mData.wajibPajakHash[npwp];

    return WajibPajak();
}

WajibPajakList CData::wajibPajak(const QString &npwp, const QString &kpp, const QString &cabang)
{
    WajibPajakList wpList;

    QString npwpfull = npwp + kpp + cabang;
    foreach (const QString &key, mData.wajibPajakHash.keys()) {
        QString _npwp = key.left(9);
        QString _kpp = key.mid(9, 3);
        QString _cabang = key.right(3);

        bool npwpMatch = npwp.isEmpty()? true : (_npwp.left(npwp.size()) == npwp);
        bool kppMatch = kpp.isEmpty()? true : (_kpp.left(kpp.size()) == kpp);
        bool cabangMatch = cabang.isEmpty()? true : (_cabang.left(cabang.size()) == cabang);

        if (npwpMatch && kppMatch && cabangMatch)
            wpList << mData.wajibPajakHash[key];
    }

    return wpList;
}

const WajibPajakHash &CData::wajibPajakHash()
{
    return mData.wajibPajakHash;
}
