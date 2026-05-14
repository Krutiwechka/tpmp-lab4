#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "report.h"
#include "db.h"

void report_remaining_cds(sqlite3* db) {
    const char* sql = 
        "SELECT cd.code, "
        "COALESCE(SUM(arr.quantity), 0) - COALESCE(SUM(sal.quantity), 0) AS remaining "
        "FROM compact_discs cd "
        "LEFT JOIN arrivals arr ON cd.id = arr.cd_id "
        "LEFT JOIN sales sal ON cd.id = sal.cd_id "
        "GROUP BY cd.id ORDER BY remaining DESC;";
    sqlite3_exec(db, sql, db_callback, 0, 0);
}

void report_sales_by_cd_and_period(sqlite3* db) {
    int cd_id;
    char start_date[11], end_date[11];
    printf("Enter CD id: ");
    scanf("%d", &cd_id);
    printf("Start date (YYYY-MM-DD): ");
    scanf("%10s", start_date);
    printf("End date (YYYY-MM-DD): ");
    scanf("%10s", end_date);

    char sql[512];
    sprintf(sql, 
        "SELECT SUM(quantity) AS total_quantity, SUM(quantity * cd.price) AS total_value "
        "FROM sales s JOIN compact_discs cd ON s.cd_id = cd.id "
        "WHERE s.cd_id = %d AND s.operation_date BETWEEN '%s' AND '%s';",
        cd_id, start_date, end_date);
    sqlite3_exec(db, sql, db_callback, 0, 0);
}

void report_most_popular_cd(sqlite3* db) {
    const char* sql = 
        "SELECT cd.id, cd.code, cd.manufacture_date, cd.manufacturer, cd.price, "
        "SUM(s.quantity) AS total_sold "
        "FROM sales s JOIN compact_discs cd ON s.cd_id = cd.id "
        "GROUP BY cd.id ORDER BY total_sold DESC LIMIT 1;";
    sqlite3_exec(db, sql, db_callback, 0, 0);
}

void report_most_popular_performer(sqlite3* db) {
    const char* sql = 
        "SELECT mp.performer, SUM(s.quantity) AS total_sold "
        "FROM musical_pieces mp "
        "JOIN sales s ON mp.cd_id = s.cd_id "
        "GROUP BY mp.performer ORDER BY total_sold DESC LIMIT 1;";
    sqlite3_exec(db, sql, db_callback, 0, 0);
}

void report_by_author(sqlite3* db) {
    const char* sql = 
        "SELECT mp.author, "
        "SUM(s.quantity) AS total_sold, "
        "SUM(s.quantity * cd.price) AS total_revenue "
        "FROM musical_pieces mp "
        "JOIN compact_discs cd ON mp.cd_id = cd.id "
        "JOIN sales s ON cd.id = s.cd_id "
        "GROUP BY mp.author;";
    sqlite3_exec(db, sql, db_callback, 0, 0);
}

void generate_period_report(sqlite3* db, const char* start_date, const char* end_date) {
    db_execute(db, "DELETE FROM period_report;");
    
    char sql[1024];
    sprintf(sql,
        "INSERT INTO period_report (cd_id, cd_code, total_arrived, total_sold, report_start_date, report_end_date) "
        "SELECT cd.id, cd.code, "
        "       COALESCE((SELECT SUM(quantity) FROM arrivals WHERE cd_id=cd.id AND operation_date BETWEEN '%s' AND '%s'), 0), "
        "       COALESCE((SELECT SUM(quantity) FROM sales WHERE cd_id=cd.id AND operation_date BETWEEN '%s' AND '%s'), 0), "
        "       '%s', '%s' "
        "FROM compact_discs cd;",
        start_date, end_date, start_date, end_date, start_date, end_date);
    
    db_execute(db, sql);
    printf("Period report generated for %s to %s\n", start_date, end_date);
    
    sqlite3_exec(db, "SELECT cd_id, cd_code, total_arrived, total_sold FROM period_report;", period_report_callback, 0, 0);
}

int period_report_callback(void* data, int argc, char** argv, char** col_names) {
    printf("CD %s: arrived=%s, sold=%s\n", argv[1], argv[2], argv[3]);
    return 0;
}

void report_sales_by_cd_period(sqlite3* db) {
    int cd_id;
    char start_date[11], end_date[11];
    printf("Enter CD id: ");
    scanf("%d", &cd_id);
    printf("Start date (YYYY-MM-DD): ");
    scanf("%10s", start_date);
    printf("End date (YYYY-MM-DD): ");
    scanf("%10s", end_date);
    
    char sql[512];
    sprintf(sql,
        "SELECT SUM(s.quantity) AS total_quantity, SUM(s.quantity * cd.price) AS total_value "
        "FROM sales s JOIN compact_discs cd ON s.cd_id = cd.id "
        "WHERE s.cd_id = %d AND s.operation_date BETWEEN '%s' AND '%s';",
        cd_id, start_date, end_date);
    
    sqlite3_exec(db, sql, report_sales_callback, 0, 0);
}

int report_sales_callback(void* data, int argc, char** argv, char** col_names) {
    printf("Quantity sold: %s, Total value: %s\n", argv[0], argv[1]);
    return 0;
}
