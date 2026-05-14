#ifndef SALE_H
#define SALE_H

#include <sqlite3.h>

int check_stock(sqlite3* db, int cd_id, int quantity);
int stock_callback(void* data, int argc, char** argv, char** col_names);
void add_arrival(sqlite3* db);
void add_sale(sqlite3* db);

#endif
