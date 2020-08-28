#ifndef CCOMMON_H
#define CCOMMON_H

#include <common.h>

class CCommon : public Common
{
public:
    ~CCommon() {}

    QString encrypt(const QString &value);
    QString decrypt(const QString &value);
    QString generateRandomId(int length);
    QString intToNamaJabatan(int value);
    QString intToJenisSeksi(int value);
    QString intToNamaBulan(int value);
};

#endif // CCOMMON_H
