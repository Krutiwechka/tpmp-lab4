#include <stdlib.h>
#include <CUnit/Basic.h>
#include <sqlite3.h>
#include "db.h"

static sqlite3* db;

static int value_callback(void* data, int argc, char** argv, char** col_names) {
    int* val = (int*)data;
    if (argc > 0 && argv[0]) {
        *val = atoi(argv[0]);
    }
    return 0;
}

int init_suite_report(void) {
    sqlite3_open(":memory:", &db);
    db_execute(db, "CREATE TABLE compact_discs (id INTEGER PRIMARY KEY, code TEXT, price REAL);");
    db_execute(db, "CREATE TABLE arrivals (cd_id INTEGER, quantity INTEGER);");
    db_execute(db, "CREATE TABLE sales (cd_id INTEGER, quantity INTEGER);");
    db_execute(db, "CREATE TABLE musical_pieces (id INTEGER PRIMARY KEY, author TEXT, performer TEXT, cd_id INTEGER);");
    
    db_execute(db, "INSERT INTO compact_discs VALUES (1, 'CD001', 15.99);");
    db_execute(db, "INSERT INTO compact_discs VALUES (2, 'CD002', 12.99);");
    db_execute(db, "INSERT INTO arrivals VALUES (1, 100);");
    db_execute(db, "INSERT INTO arrivals VALUES (2, 50);");
    db_execute(db, "INSERT INTO sales VALUES (1, 30);");
    db_execute(db, "INSERT INTO sales VALUES (2, 10);");
    db_execute(db, "INSERT INTO musical_pieces VALUES (1, 'Mercury', 'Queen', 1);");
    db_execute(db, "INSERT INTO musical_pieces VALUES (2, 'McCartney', 'Beatles', 2);");
    return 0;
}

int clean_suite_report(void) {
    sqlite3_close(db);
    return 0;
}

void test_remaining_cds(void) {
    int remaining = 0;
    const char* sql = "SELECT COALESCE((SELECT SUM(quantity) FROM arrivals WHERE cd_id=1),0) - "
                      "COALESCE((SELECT SUM(quantity) FROM sales WHERE cd_id=1),0);";
    sqlite3_exec(db, sql, value_callback, &remaining, NULL);
    CU_ASSERT(remaining == 70);
}

void test_most_popular_cd(void) {
    int max_sold = 0;
    const char* sql = "SELECT MAX(total) FROM (SELECT SUM(quantity) as total FROM sales GROUP BY cd_id);";
    sqlite3_exec(db, sql, value_callback, &max_sold, NULL);
    CU_ASSERT(max_sold == 30);
}

void test_report_by_author(void) {
    int total_sold = 0;
    const char* sql = "SELECT SUM(s.quantity) FROM musical_pieces mp "
                      "JOIN sales s ON mp.cd_id = s.cd_id "
                      "WHERE mp.author='Mercury';";
    sqlite3_exec(db, sql, value_callback, &total_sold, NULL);
    CU_ASSERT(total_sold == 30);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Report Tests", init_suite_report, clean_suite_report);
    CU_add_test(suite, "Remaining CDs calculation", test_remaining_cds);
    CU_add_test(suite, "Most popular CD", test_most_popular_cd);
    CU_add_test(suite, "Report by author", test_report_by_author);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
