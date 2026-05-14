#ifndef AUTH_H
#define AUTH_H

#include <sqlite3.h>

int authenticate(sqlite3* db);
int get_role_callback(void* data, int argc, char** argv, char** col_names);

#endif
