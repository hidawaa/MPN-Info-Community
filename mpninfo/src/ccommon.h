#ifndef CCOMMON_H
#define CCOMMON_H

#include <common.h>

class CCommon : public Common
{
public:
    ~CCommon() {}

    QString osName();

    QDate date(const QString &value);

    QString encrypt(const QString &value);
    QString decrypt(const QString &value);
    QString randomString(int length, Qt::CaseSensitivity sensitivity = Qt::CaseSensitive);
    QStringList split(const QString &string, const QChar separator, bool *ok = nullptr);

    QString formattedNpwp(const QString &npwp);
    QString formattedNpwp(const QString &npwp, const QString &kpp, const QString &cabang);
    QString formattedKetetapan(const QString &noSk);
    QString formattedTanggal(int tahun, int bulan, int tanggal);

    const QStringList &namaBulanList();
    const QStringList &namaJabatanList();
    const QStringList &namaPangkatList();
    const QStringList &namaGolonganList();
    const QStringList &namaJenisSeksiList();
    const QStringList &namaSektorList();

    QString namaBulan(int value);
    QString namaBulanEn(int value);
    QString namaBulanId(int value);
    QString namaUserGroup(int value);
    QString namaJabatan(int value);
    QString namaPangkat(int value);
    QString namaGolongan(int value);
    QString namaJenisSeksi(int value);
    QString namaSektor(int value);
    QString namaSektor(const QString &value);
    QString namaKdmapGroup(int value);

    int bulanTerlambat(const QDate &value, const QDate &jt, int masa, int tahun, int jenis);
    double dendaLapor(const QString &jenis, int masa, int tahun);

};

#endif // CCOMMON_H
