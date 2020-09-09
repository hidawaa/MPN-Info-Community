#ifndef CDATABASECOMPATIBILITY_H
#define CDATABASECOMPATIBILITY_H

#include "cdatabase.h"

class CDatabaseCompatibility : public DatabaseCompatilbility
{
public:
    CDatabaseCompatibility(CDatabase *_db);

    void truncateTable(const QString &table);
    void beginTransaction();
    void endTransaction();

private:
    CDatabase *db;
};

#endif // CDATABASECOMPATIBILITY_H
