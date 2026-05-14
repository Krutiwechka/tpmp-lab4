#ifndef REPORT_H
#define REPORT_H

#include <sqlite3.h>

void report_remaining_cds(sqlite3* db);
void report_sales_by_cd_and_period(sqlite3* db);
void report_most_popular_cd(sqlite3* db);
void report_most_popular_performer(sqlite3* db);
void report_by_author(sqlite3* db);

#endif
