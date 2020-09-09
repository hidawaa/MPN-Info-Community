#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QDate>

#include "enums.h"

class Common
{
public:
    virtual ~Common() {}

    virtual QDate date(const QString &value) = 0;

    virtual QString encrypt(const QString &value) = 0;
    virtual QString decrypt(const QString &value) = 0;
    virtual QString randomString(int length, Qt::CaseSensitivity sensitivity = Qt::CaseSensitive) = 0;
    virtual QStringList split(const QString &string, const QChar separator, bool *ok = nullptr) = 0;

    virtual QString formattedNpwp(const QString &npwp) = 0;
    virtual QString formattedNpwp(const QString &npwp, const QString &kpp, const QString &cabang) = 0;
    virtual QString formattedKetetapan(const QString &noSk) = 0;
    virtual QString formattedTanggal(int tahun, int bulan, int tanggal) = 0;

    virtual const QStringList &namaBulanList() = 0;
    virtual const QStringList &namaJabatanList() = 0;
    virtual const QStringList &namaPangkatList() = 0;
    virtual const QStringList &namaGolonganList() = 0;
    virtual const QStringList &namaJenisSeksiList() = 0;
    virtual const QStringList &namaSektorList() = 0;

    virtual QString namaBulan(int value) = 0;
    virtual QString namaBulanEn(int value) = 0;
    virtual QString namaBulanId(int value) = 0;
    virtual QString namaUserGroup(int value) = 0;
    virtual QString namaJabatan(int value) = 0;
    virtual QString namaPangkat(int value) = 0;
    virtual QString namaGolongan(int value) = 0;
    virtual QString namaJenisSeksi(int value) = 0;
    virtual QString namaSektor(int value) = 0;
    virtual QString namaSektor(const QString &value) = 0;
    virtual QString namaKdmapGroup(int value) = 0;

    virtual int bulanTerlambat(const QDate &value, const QDate &jt, int masa, int tahun, int jenis) = 0;
    virtual double dendaLapor(const QString &jenis, int masa, int tahun) = 0;
};

#endif // COMMON_H
