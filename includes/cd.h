#ifndef CD_H
#define CD_H

#include <sqlite3.h>

void add_cd(sqlite3* db);
void update_cd(sqlite3* db);
void delete_cd(sqlite3* db);
void list_cds(sqlite3* db);

#endif
