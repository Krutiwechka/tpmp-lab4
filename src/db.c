#include <stdio.h>
#include <sqlite3.h>
#include "db.h"

sqlite3* db_connect(const char* filename) {
    sqlite3* db;
    int rc = sqlite3_open(filename, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    return db;
}

void db_disconnect(sqlite3* db) {
    if (db) sqlite3_close(db);
}

int db_execute(sqlite3* db, const char* sql) {
    char* err_msg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }
    return 0;
}

int db_callback(void* data, int argc, char** argv, char** col_names) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", col_names[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
