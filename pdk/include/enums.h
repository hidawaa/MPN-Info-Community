#ifndef ENUMS_H
#define ENUMS_H

enum DatabaseTypes {
    DatabaseUnknown = 0,
    DatabaseMysql,
    DatabaseSqlite,
    DatabaseOther
};

enum GroupTypes {
    GroupUnknown = -1,
    GroupAdministrator,
    GroupUser,
    GroupGuest
};

enum KantorTypes {
    Kanwil = 0,
    Kpp
};

enum JabatanTypes {
    JabatanUnknown = -1,
    JabatanKepalaKantor,
    JabatanKepalaSeksi,
    JabatanFungsionalPemeriksa,
    JabatanOperatorConsole,
    JabatanAccountRepresentativePelayanan,
    JabatanAccountRepresentativePengawasan,
    JabatanPelaksana
};

enum SeksiTypes {
    SeksiUnknown = -1,
    SeksiKepalaKantor,
    SeksiSubbagianUmum,
    SeksiPengolahanDataDanInformasi,
    SeksiPelayanan,
    SeksiPenagihan,
    SeksiPengawasanDanKonsultasiPelayanan,
    SeksiPengawasanDanKonsultasiPengawasan,
    SeksiEkstensifikasiPerpajakan,
    SeksiPemeriksaanDanKepatuhanInternal
};

enum JatuhTempoTypes {
    JatuhTempoUnknown = -1,
    JatuhTempoPotPut,
    JatuhTempoPotPutTahunanOp,
    JatuhTempoPotPutTahunanBadan,
    JatuhTempoPPh,
    JatuhTempoPpn,
    JatuhTempoPphOp,
    JatuhTempoPphBadan
};

enum MpnSources {
    MpnSourceMpn = 1,
    MpnSourceSpm,
    MpnSourceManual = 4
};

enum MpnTypes {
    MpnIdr = 1,
    MpnUsd,
    MpnPbb
};

enum SpmkpSources {
    SpmkpSourceKppn = 1,
    SpmkpSourceSidjp,
    SpmkpSourceDashboard
};

enum SpmpppSources {
    SpmppSourceKppn = 1
};

enum PkPm {
    Pk = 1,
    Pm
};

#endif // ENUMS_H
