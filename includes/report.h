#ifndef REPORT_H
#define REPORT_H

#include <sqlite3.h>

void report_remaining_cds(sqlite3* db);
void report_sales_by_cd_and_period(sqlite3* db);
void report_most_popular_cd(sqlite3* db);
void report_most_popular_performer(sqlite3* db);
void report_by_author(sqlite3* db);
void generate_period_report(sqlite3* db, const char* start_date, const char* end_date);
int period_report_callback(void* data, int argc, char** argv, char** col_names);
void report_sales_by_cd_period(sqlite3* db);
int report_sales_callback(void* data, int argc, char** argv, char** col_names);

#endif
