#include "ccommon.h"

#include <QRandomGenerator>
#include <QDebug>

#include "simplecrypt.h"

const QStringList _namaBulanList = QStringList() << ""
                                                 << "Januari"
                                                 << "Februari"
                                                 << "Maret"
                                                 << "April"
                                                 << "Mei"
                                                 << "Juni"
                                                 << "Juli"
                                                 << "Agustus"
                                                 << "September"
                                                 << "Oktober"
                                                 << "November"
                                                 << "Desember";

const QStringList _namaBulanEnList = QStringList() << ""
                                                   << "JAN"
                                                   << "FEB"
                                                   << "MAR"
                                                   << "APR"
                                                   << "MAY"
                                                   << "JUN"
                                                   << "JUL"
                                                   << "AUG"
                                                   << "SEP"
                                                   << "OCT"
                                                   << "NOV"
                                                   << "DEC";

const QStringList _namaBulanIdList = QStringList() << ""
                                                   << "JAN"
                                                   << "PEB"
                                                   << "MAR"
                                                   << "APR"
                                                   << "MEI"
                                                   << "JUN"
                                                   << "JUL"
                                                   << "AGUST"
                                                   << "SEP"
                                                   << "OKT"
                                                   << "NOP"
                                                   << "DES";

const QStringList _namaUserGroupList = QStringList() << "Administrator"
                                                     << "Users";

const QStringList _namaJabatanList = QStringList()  << "Kepala Kantor"
                                                    << "Kepala Seksi"
                                                    << "Fungsional Pemeriksa"
                                                    << "Operator Console"
                                                    << "Account Representative Pelayanan"
                                                    << "Account Representative Pengawasan"
                                                    << "Pelaksana";

const QStringList _namaPangkatList = QStringList() << "Juru Muda"
                                                   << "Juru Muda Tingkat"
                                                   << "Juru"
                                                   << "Juru Tingkat 1"
                                                   << "Pengatur Muda"
                                                   << "Pengatur Muda Tingkat 1"
                                                   << "Pengatur"
                                                   << "Pengatur Tingkat 1"
                                                   << "Penata Muda"
                                                   << "Penata Muda Tingkat 1"
                                                   << "Penata"
                                                   << "Penata Tingkat 1"
                                                   << "Pembina"
                                                   << "Pembina Tingkat 1"
                                                   << "Pembina Utama Muda"
                                                   << "Pembina Utama Madya"
                                                   << "Pembina Utama";

const QStringList _namaGolonganList = QStringList() << "Ia"
                                                    << "Ib"
                                                    << "Ic"
                                                    << "Id"
                                                    << "IIa"
                                                    << "IIb"
                                                    << "IIc"
                                                    << "IId"
                                                    << "IIIa"
                                                    << "IIIb"
                                                    << "IIIc"
                                                    << "IIId"
                                                    << "IVa"
                                                    << "IVb"
                                                    << "IVc"
                                                    << "IVd"
                                                    << "IVe";

const QStringList _namaJenisSeksiList = QStringList() << "Kepala Kantor"
                                                      << "SubBagian Umum dan Kepatuhan Internal"
                                                      << "Pengolahan Data dan Informasi"
                                                      << "Pelayanan"
                                                      << "Penagihan"
                                                      << "Pengawasan dan Konsultasi I"
                                                      << "Pengawasan dan Konsultasi II/III/IV"
                                                      << "Ekstensifikasi dan Penyuluhan"
                                                      << "Pemeriksaan";

const QStringList _namaSektorList = QStringList() << "Pertanian, Kehutanan, dan Perikanan"
                                                  << "Pertambangan dan Penggalian"
                                                  << "Industri Pengolahan"
                                                  << "Pengadaan Listrik, Gas, Uap/Air Panas dan Udara Dingin"
                                                  << "Pengadaan Air, Pengelolaan Sampah dan Daur Ulang, Pembuangan dan Pembersihan Limbah dan Sampah"
                                                  << "Konstruksi"
                                                  << "Perdagangan Besar dan Eceran; Reparasi dan Perawatan Mobil dan Sepeda Motor"
                                                  << "Transportasi dan Pergudangan"
                                                  << "Penyediaan Akomodasi dan Penyediaan Makan Minum"
                                                  << "Informasi dan Komunikasi"
                                                  << "Jasa Keuangan dan Asuransi"
                                                  << "Real Estate"
                                                  << "Jasa Profesional, Ilmiah dan Teknis"
                                                  << "Jasa Persewaan, Ketenagakerjaan, Agen Perjalanan dan Penunjang Usaha Lainnya"
                                                  << "Administrasi Pemerintahan dan Jaminan Sosial Wajib"
                                                  << "Jasa Pendidikan"
                                                  << "Jasa Kesehatan dan Kegiatan Sosial"
                                                  << "Kebudayaan, Hiburan dan Rekreasi"
                                                  << "Kegiatan Jasa Lainnya"
                                                  << "Jasa Perorangan yang Melayani Rumah Tangga, Kegiatan yang Menghasilkan Barang dan Jasa Oleh Rumah Tangga yang Digunakan Sendiri Untuk Memenuhi Kebutuhan"
                                                  << "Kegiatan Badan Internasional dan Badan Ekstra Internasional Lainnya";

const QStringList _namaKdmapGroupList = QStringList() << "PPh Migas"
                                                      << "PPh Non Migas"
                                                      << "PPN dan PPnBM"
                                                      << "PBB dan BPHTB"
                                                      << "Pendapatan atas PL dan PIB";

const quint64 cryptoNumber(0x032ab4a5acb3a033);

QString CCommon::osName()
{
#if defined(Q_OS_ANDROID)
    return QLatin1String("Android");
#elif defined(Q_OS_BLACKBERRY)
    return QLatin1String("Blackberry");
#elif defined(Q_OS_IOS)
    return QLatin1String("IOS");
#elif defined(Q_OS_MACOS)
    return QLatin1String("Mac");
#elif defined(Q_OS_TVOS)
    return QLatin1String("TvOS");
#elif defined(Q_OS_WATCHOS)
    return QLatin1String("WatchOS");
#elif defined(Q_OS_WINCE)
    return QLatin1String("WinCE");
#elif defined(Q_OS_WIN)
    return QLatin1String("Windows");
#elif defined(Q_OS_LINUX)
    return QLatin1String("Linux");
#elif defined(Q_OS_UNIX)
    return QLatin1String("Unix");
#else
    return QLatin1String("Unknown");
#endif
}

QDate CCommon::date(const QString &value)
{
    {
        QDate date = QDate::fromString(value, "dd/MM/yyyy");
        if (date.isValid())
            return date;
    }

    {
        QDate date = QDate::fromString(value, "dd-MM-yyyy");
        if (date.isValid())
            return date;
    }

    {
        int tanggal(0), bulan(0), tahun(0);
        QStringList daySplit = value.split('-');

        tanggal = daySplit.count() > 0? daySplit[0].toInt() : 0;
        if (daySplit.count() > 1) {
            for (int b=1; b<=12; b++)
                if (_namaBulanEnList[b] == daySplit[1].toUpper()) {
                    bulan = b;
                    break;
                }
        }

        if (daySplit.count() > 2) {
            tahun = daySplit[2].right(2).toInt();
            if (tahun >= 45)
                tahun += 1900;
            else
                tahun += 2000;
        }

        QDate date(tahun, bulan, tanggal);
        if (date.isValid())
            return date;
    }

    {
        int tanggal(0), bulan(0), tahun(0);
        QStringList daySplit = value.split('-');

        tanggal = daySplit.count() > 0? daySplit[0].toInt() : 0;
        if (daySplit.count() > 1) {
            for (int b=1; b<=12; b++)
                if (_namaBulanIdList[b] == daySplit[1].toUpper()) {
                    bulan = b;
                    break;
                }
        }

        if (daySplit.count() > 2) {
            tahun = daySplit[2].right(2).toInt();
            if (tahun > 20)
                tahun += 1900;
            else
                tahun += 2000;
        }

        QDate date(tahun, bulan, tanggal);
        if (date.isValid())
            return date;
    }

    {
        QDate date = QDate::fromString(value, "yyyyMMdd");
        if (date.isValid())
            return date;
    }

    return QDate();
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

QString CCommon::randomString(int length, Qt::CaseSensitivity sensitivity)
{
    const QString possibleCharactersSensitive("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const QString possibleCharactersInsensitive("abcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for(int i=0; i<length; ++i) {
        QChar nextChar;
        qint64 randomNumber = qint64(QRandomGenerator::global()->generate());
        if (sensitivity == Qt::CaseSensitive) {
            int index = randomNumber % possibleCharactersSensitive.length();
            nextChar = possibleCharactersSensitive.at(index);
        }
        else {
            int index = randomNumber % possibleCharactersInsensitive.length();
            nextChar = possibleCharactersInsensitive.at(index);
        }

        randomString.append(nextChar);
    }

    return randomString;
}

QStringList CCommon::split(const QString &string, const QChar separator, bool *ok)
{
    QStringList list;
    QString temp;

    bool isText = false;
    for (int i=0; i < string.length(); i++) {
        if (string[i] == '"') {
            if (i == 0 || (i-1 >= 0 && string[i-1] == separator))
            {
                if (!isText)
                {
                    isText = true;
                    continue;
                }
            }

            if (i == string.length()-1 || (i+1 < string.length() && string[i+1] == separator))
            {
                if (isText)
                {
                    isText = false;
                    continue;
                }
            }

            if (i+1 < string.length() && string[i+1] == '"')
            {
                temp.append(string[i++]);
                continue;
            }

            temp.append(string[i]);
            continue;
        }

        if (isText) {
            temp.append(string[i]);
        }
        else {
            if (string[i] == separator) {
                list << temp;
                temp.clear();
            }
            else {
                temp.append(string[i]);
            }
        }
    }

    if (ok) *ok = !isText;

    list << temp.trimmed();

    return list;
}

QString CCommon::formattedNpwp(const QString &npwp)
{
    if (npwp.isEmpty())
        return QString();

    return formattedNpwp(npwp.left(9), npwp.mid(9, 3), npwp.right(3));
}

QString CCommon::formattedNpwp(const QString &npwp, const QString &kpp, const QString &cabang) {
    return QString("%1.%2.%3.%4-%5.%6").arg(npwp.mid(0, 2))
            .arg(npwp.mid(2, 3))
            .arg(npwp.mid(5, 3))
            .arg(npwp.mid(8, 1))
            .arg(kpp)
            .arg(cabang);
}

QString CCommon::formattedKetetapan(const QString &noSk) {
    if (noSk == "000000000000000")
        return QString();

    if (noSk.isEmpty())
        return QString();

    return QString("%1/%2/%3/%4/%5").arg(noSk.mid(0, 5))
            .arg(noSk.mid(5, 3))
            .arg(noSk.mid(8, 2))
            .arg(noSk.mid(10, 3))
            .arg(noSk.mid(13, 2));
}

QString CCommon::formattedTanggal(int tahun, int bulan, int tanggal)
{
    return QDate(tahun, bulan, tanggal).toString("dd-MM-yyyy");
}

const QStringList &CCommon::namaBulanList()
{
    return _namaBulanList;
}

const QStringList &CCommon::namaJabatanList()
{
    return _namaJabatanList;
}

const QStringList &CCommon::namaPangkatList()
{
    return _namaPangkatList;
}

const QStringList &CCommon::namaGolonganList()
{
    return _namaGolonganList;
}

const QStringList &CCommon::namaJenisSeksiList()
{
    return _namaJenisSeksiList;
}

const QStringList &CCommon::namaSektorList()
{
    return _namaSektorList;
}

QString CCommon::namaBulan(int value)
{
    if (value < 0 || value >= _namaBulanList.size())
        return QString();

    return _namaBulanList[value];
}

QString CCommon::namaBulanEn(int value)
{
    if (value < 0 || value >= _namaBulanEnList.size())
        return QString();

    return _namaBulanEnList[value];
}

QString CCommon::namaBulanId(int value)
{
    if (value < 0 || value >= _namaBulanIdList.size())
        return QString();

    return _namaBulanIdList[value];
}

QString CCommon::namaUserGroup(int value)
{
    if (value < 0 || value >= _namaUserGroupList.size())
        return QString();

    return _namaUserGroupList[value];
}

QString CCommon::namaJabatan(int value)
{
    if (value < 0 || value >= _namaJabatanList.size())
        return QString();

    return _namaJabatanList[value];
}

QString CCommon::namaPangkat(int value)
{
    if (value < 0 || value >= _namaPangkatList.size())
        return QString();

    return _namaPangkatList[value];
}

QString CCommon::namaGolongan(int value)
{
    if (value < 0 || value >= _namaGolonganList.size())
        return QString();

    return _namaGolonganList[value];
}

QString CCommon::namaJenisSeksi(int value)
{
    if (value < 0 || value >= _namaJenisSeksiList.size())
        return QString();

    return _namaJenisSeksiList[value];
}

QString CCommon::namaSektor(int value)
{
    if (value < 0 || value >= _namaSektorList.size())
        return QString();

    return _namaSektorList[value];
}

QString CCommon::namaSektor(const QString &value)
{
    if (value < "A" || value > "U")
        return QString();

    char c = value.toStdString().c_str()[0];
    char a = 'A';

    return _namaSektorList[int(c - a)];
}

QString CCommon::namaKdmapGroup(int value)
{
    if (value < 0 || value >= _namaKdmapGroupList.size())
        return QString();

    return _namaKdmapGroupList[value];
}

int CCommon::bulanTerlambat(const QDate &value, const QDate &jt, int masa, int tahun, int jenis)
{
    if (value <= jt)
        return 0;

    QDate jtMasaPajak;
    int jtDay(1);

    switch (jenis) {
    case JatuhTempoPotPut:
        jtMasaPajak = QDate(tahun, masa, 10).addMonths(1);
        jtDay = jtMasaPajak.day();
        break;
    case JatuhTempoPPh:
        jtMasaPajak = QDate(tahun, masa, 15).addMonths(1);
        jtDay = jtMasaPajak.day();
        break;
    case JatuhTempoPpn: {
        if (QDate(tahun, masa, 1) < QDate(2010, 4, 1)) {
            jtMasaPajak = QDate(tahun, masa, 15).addMonths(1);
            jtDay = jtMasaPajak.day();
        }
        else {
            jtMasaPajak = QDate(tahun, masa, 1).addMonths(2).addDays(-1);
            jtDay = value.daysInMonth();
        }
    }
        break;
    case JatuhTempoPotPutTahunanBadan:
    case JatuhTempoPphBadan: {
        if (tahun < 2008) {
            jtMasaPajak = QDate(tahun +1, 3, 25);
            jtDay = jtMasaPajak.day();
        }
        else {
            jtMasaPajak = QDate(tahun +1, 4, 30);
            jtDay = value.daysInMonth();
        }
    }
        break;
    case JatuhTempoPotPutTahunanOp:
    case JatuhTempoPphOp: {
        if (tahun < 2008) {
            jtMasaPajak = QDate(tahun +1, 3, 25);
            jtDay = jtMasaPajak.day();
        }
        else {
            jtMasaPajak = QDate(tahun +1, 3, 31);
            jtDay = value.daysInMonth();
        }
    }
        break;
    default:
        break;
    }

    return 12 * (value.year() - jtMasaPajak.year()) + value.month() - jtMasaPajak.month() + (value.day() > jtDay ? 1 : 0);
}

double CCommon::dendaLapor(const QString &jenis, int masa, int tahun)
{
    if (tahun >= 2008) {
        if (masa == 0) {
            if (jenis == "411126")
                return 1000000;
            else
                return 100000;
        }
        else {
            if (jenis == "411211")
                return 500000;
            else
                return 100000;
        }
    }
    else if (tahun >= 2001) {
        if (masa == 0)
            return 100000;
        else
            return 50000;
    }
    else if (tahun >= 1995) {
        if (masa == 0)
            return 50000;
        else
            return 25000;
    }
    else
        return 10000;
}
