#ifndef SALE_H
#define SALE_H

#include <sqlite3.h>

void add_arrival(sqlite3* db);
void add_sale(sqlite3* db);
int check_stock(sqlite3* db, int cd_id, int quantity);

#endif
