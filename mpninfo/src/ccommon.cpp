#include "ccommon.h"

#include "simplecrypt.h"

const quint64 cryptoNumber(0x032ab4a5acb3a033);

QString CCommon::generateRandomId(int length)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for(int i=0; i<length; ++i)
    {
        int index = int(rand() % possibleCharacters.length());
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }

    return randomString;
}

QString CCommon::encrypt(const QString &value)
{
    SimpleCrypt crypto(cryptoNumber);
    return crypto.encryptToString(value);
}

QString CCommon::decrypt(const QString &value)
{
    SimpleCrypt crypto(cryptoNumber);
    return crypto.decryptToString(value);
}

QString CCommon::intToNamaJabatan(int)
{
    return QString();
}

QString CCommon::intToJenisSeksi(int)
{
    return QString();
}

QString CCommon::intToNamaBulan(int)
{
    return QString();
}
