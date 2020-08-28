#ifndef COMMON_H
#define COMMON_H

#include <QString>

class Common
{
public:
    virtual ~Common() {}

    virtual QString encrypt(const QString &value) = 0;
    virtual QString decrypt(const QString &value) = 0;
    virtual QString generateRandomId(int length) = 0;
    virtual QString intToNamaJabatan(int value) = 0;
    virtual QString intToJenisSeksi(int value) = 0;
    virtual QString intToNamaBulan(int value) = 0;
};

#endif // COMMON_H
