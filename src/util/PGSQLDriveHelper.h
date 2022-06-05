#ifndef PGSQLDRIVEHELPER_H
#define PGSQLDRIVEHELPER_H
#include <libpq-fe.h>

//从外部引入
extern void PQfinish(PGconn *conn);
extern void PQclear(PGresult *res);

class PGSQLDriveHelper
{
public:
    PGSQLDriveHelper(){}
    int pg_test();
};

#endif // PGSQLDRIVEHELPER_H
