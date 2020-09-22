#include "cdata.h"

#include <QObject>
#include <QDebug>

#include <runnable.h>

#include "engine.h"

void CRunnable::run() {
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    if (nameList.isEmpty() || nameList.contains("kantor")) {
        (*data).kantorMap.clear();
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
    }

    if (nameList.isEmpty() || nameList.contains("seksi")) {
        (*data).seksiMap.clear();
        db->exec("SELECT * FROM `seksi`");
        while (db->next()) {
            int id = db->value(0).toInt();
            QString kantor = db->value(1).toString();
            int type = db->value(2).toInt();
            QString nama = db->value(3).toString();
            QString kode = db->value(4).toString();
            QString telp = db->value(5).toString();

            Seksi &seksi = (*data).seksiMap[kantor + ":" + QString::number(id)];
            seksi.id = id;
            seksi.type = type;
            seksi.kantor = (*data).kantorMap[kantor];
            seksi.nama = nama;
            seksi.kode = kode;
            seksi.telp = telp;
        }
    }

    if (nameList.isEmpty() || nameList.contains("pegawai")) {
        (*data).pegawaiHash.clear();
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

            const Seksi &seksi = (*data).seksiMap[kantor + ":" + QString::number(seksiId)];

            Pegawai &p = (*data).pegawaiHash[kantor + ":" + QString::number(tahun) + ":" + nip];
            p.nip = nip;
            p.nip2 = nip2;
            p.kantor = (*data).kantorMap[kantor];
            p.nama = nama;
            p.pangkat = pangkat;
            p.seksi = seksi;
            p.jabatan = jabatan;
            p.tahun = tahun;
            p.plh = plh;
        }
    }

    if (nameList.isEmpty() || nameList.contains("wp")) {
        (*data).wajibPajakHash.clear();
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
    QMapIterator<QString, Kantor> iterator(mData.kantorMap);
    while (iterator.hasNext()) {
        iterator.next();
        if (iterator.value().type != Kpp)
            continue;

        if (iterator.value().kanwil == kanwil)
            list << iterator.value();
    }

    return list;
};

Seksi CData::seksi(const QString &kodeKantor, int id) const
{
    return mData.seksiMap[kodeKantor + ":" + QString::number(id)];
}

SeksiList CData::seksiList(const QString &kodeKantor)
{
    SeksiList list;
    QMapIterator<QString, Seksi> iterator(mData.seksiMap);
    while (iterator.hasNext()) {
        iterator.next();

        if (iterator.value().kantor.kode != kodeKantor)
            continue;

        list << iterator.value();
    }

    return list;
}

Pegawai CData::pegawai(const QString &kodeKantor, int tahun, const QString &nip) const
{
    QString pegawaiKey = kodeKantor + ":" + QString::number(tahun) + ":" + nip;
    if (!mData.pegawaiHash.contains(pegawaiKey))
        return Pegawai();

    return mData.pegawaiHash[kodeKantor + ":" + QString::number(tahun) + ":" + nip];
}


PegawaiList CData::pegawaiList(const QString &kodeKantor, int tahun)
{

    PegawaiList list;
    QHashIterator<QString, Pegawai> iterator(mData.pegawaiHash);
    while (iterator.hasNext()) {
        iterator.next();
        const Pegawai &pegawai = iterator.value();
        if (pegawai.kantor.kode != kodeKantor || pegawai.tahun != tahun)
            continue;

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


void CData::load(const QStringList &nameList)
{
    Engine *engine = Engine::instance();

    ObjectPtr loadingDialog = engine->addOn("dialog_loading")->newObject();
    if (!loadingDialog.isNull()) {
        loadingDialog->exec("setMessage", "Loading Data");
        loadingDialog->exec("show");
    }

    CRunnable *runnable = new CRunnable(&mData, nameList);
    engine->runSync(runnable);
}

User CData::user(int id)
{
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();
    db->prepare("SELECT `username`, `fullname`, `group` FROM `users` WHERE `id`=:id");
    db->bindValue(":id", id);
    db->exec();

    if (db->next())
        return User();

    User user;
    user.username = db->value(0).toString();
    user.fullname = db->value(2).toString();
    user.group = db->value(3).toInt();

    return user;
}

User CData::user(const QString &nip)
{
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();
    db->prepare("SELECT `id`, `username`, `fullname`, `group` FROM `users` WHERE `username`=:nip");
    db->bindValue(":nip", nip);
    db->exec();

    if (db->next())
        return User();

    User user;
    user.id = db->value(0).toInt();
    user.username = db->value(1).toString();
    user.fullname = db->value(2).toString();
    user.group = db->value(3).toInt();

    return user;
}

QList<User> CData::userList()
{
    QList<User> temp;

    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();
    db->exec("SELECT `id`, `username`, `fullname`, `group` FROM `users` ORDER BY `group`");
    while (db->next()) {
        User user;
        user.id = db->value(0).toInt();
        user.username = db->value(1).toString();
        user.fullname = db->value(2).toString();
        user.group = db->value(3).toInt();

        temp << user;
    }

    return temp;
}
bool CData::addUser(const QString &username, const QString &password, const QString &fullname, int group)
{
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    QString addSql = QString("INSERT INTO `users`(`username`, `password`, `fullname`, `group`) values('%1', '%2', '%3', '%4')").arg(username, password, fullname).arg(group);
    db->exec(addSql);
    if (db->numRowsAffected() == 0)
        return false;

    return true;
}

bool CData::deleteUser(int id)
{
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    QString deleteSql = QString("DELETE FROM `users` WHERE `id`='%1'").arg(id);
    db->exec(deleteSql);
    if (db->numRowsAffected() == 0)
        return false;

    return true;
}

bool CData::deleteUser(const QString &username)
{
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    QString deleteSql = QString("DELETE FROM `users` WHERE `username`='%1'").arg(username);
    db->exec(deleteSql);
    if (db->numRowsAffected() == 0)
        return false;

    return true;
}

bool CData::updateUser(int id, const QString &username, const QString &password, const QString &fullname, int group)
{
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    QString updateSql = QString("UPDATE `users` SET `username`='%2', `password`='%3', `fullname`='%4', `group`='%5' WHERE `id`='%1'").arg(id).arg(username, password, fullname).arg(group);
    db->exec(updateSql);
    if (db->numRowsAffected() == 0)
        return false;

    return true;
}

bool CData::addSeksi(const QString &kodeKantor, int type, const QString &nama, const QString &kode, const QString &telp)
{
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    int maxId(0);
    db->exec(QString("SELECT MAX(`id`) FROM `seksi` WHERE `kantor`='%1'").arg(kodeKantor));
    if (db->next())
        maxId = db->value(0).toInt();
    maxId++;

    QString addString = QString("INSERT INTO `seksi`(`id`, `kantor`, `tipe`, `nama`, `kode`, `telp`) VALUES('%1', '%2', '%3', '%4', '%5', '%6')")
            .arg(maxId).arg(kodeKantor).arg(type).arg(nama).arg(kode).arg(telp);

    db->exec(addString);
    if (db->numRowsAffected() == 0)
        return false;

    load(QStringList() << "seksi");
    return true;
}

bool CData::updateSeksi(int id, const QString &kodeKantor, int type, const QString &nama, const QString &kode, const QString &telp)
{
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    QString updateString = QString("UPDATE `seksi` SET `tipe`='%3', `nama`='%4', `kode`='%5', `telp`='%6' WHERE `id`='%1' AND `kantor`='%2'")
            .arg(id).arg(kodeKantor).arg(type).arg(nama).arg(kode).arg(telp);

    db->exec(updateString);
    if (db->numRowsAffected() == 0)
        return false;

    load(QStringList() << "seksi");
    return false;
}

bool CData::removeSeksi(int id, const QString &kodeKantor)
{
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    QString delString = QString("DELETE FROM `seksi` WHERE `id`='%1' AND `kantor`='%2'").arg(id).arg(kodeKantor);
    db->exec(delString);
    if (db->numRowsAffected() == 0)
        return false;

    load(QStringList() << "seksi");
    return true;
}

bool CData::addPegawai(const QString &kantor, const QString &nip, const QString &nip2, const QString &nama, int seksi, int pangkat, int jabatan, const QString &plh, int tahun)
{
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    QString checkSql = QString("SELECT * FROM `pegawai` WHERE `nip`='%1' AND `tahun`='%2' AND `kantor`='%3'").arg(nip).arg(tahun).arg(kantor);
    db->exec(checkSql);
    if (db->next())
        return false;

    db->prepare("INSERT INTO `pegawai` values(:col0, :col1, :col2, :col3, :col4, :col5, :col6, :col7, :col8)");
    db->bindValue(":col0", kantor);
    db->bindValue(":col1", nip);
    db->bindValue(":col2", nip2);
    db->bindValue(":col3", nama.trimmed());
    db->bindValue(":col4", pangkat);
    db->bindValue(":col5", seksi);
    db->bindValue(":col6", jabatan);
    db->bindValue(":col7", tahun);
    db->bindValue(":col8", plh.isEmpty()? QVariant() : plh);
    db->exec();

    if (db->numRowsAffected() == 0)
        return false;

    load(QStringList() << "pegawai");
    return true;
}

bool CData::updatePegawai(const QString &kantor, const QString &nip, const QString &nip2, const QString &nama, int seksi, int pangkat, int jabatan, const QString &plh, int tahun)
{
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    QString updateSql = QString("UPDATE `pegawai` SET `nip2`='%2', `nama`='%3', `seksi`='%4', `pangkat`='%5', `jabatan`='%6', `plh`='%7' WHERE `nip`='%1' AND `tahun`='%8' AND `kantor`='%9'")
            .arg(nip, nip2, nama.trimmed()).arg(seksi).arg(pangkat).arg(jabatan).arg(plh).arg(tahun).arg(kantor);

    db->exec(updateSql);
    if (db->numRowsAffected() == 0)
        return false;

    load(QStringList() << "pegawai");
    return true;
}

bool CData::deletePegawai(const QString &kantor, const QString &nip, int tahun )
{
    Engine *engine = Engine::instance();
    DatabasePtr db = engine->database();

    QString deleteSql = QString("DELETE FROM `pegawai` WHERE `nip`='%1' AND `tahun`='%2' AND `kantor`='%3'")
            .arg(nip).arg(tahun).arg(kantor);

    db->exec(deleteSql);
    if (db->numRowsAffected() == 0)
        return false;

    load(QStringList() << "pegawai");
    return true;
}
