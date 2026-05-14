#include <stdlib.h>
#include <CUnit/Basic.h>
#include <sqlite3.h>
#include "db.h"

static sqlite3* db;

static int sum_callback(void* data, int argc, char** argv, char** col_names) {
    int* sum = (int*)data;
    if (argc > 0 && argv[0]) {
        *sum = atoi(argv[0]);
    } else {
        *sum = 0;
    }
    return 0;
}

int init_suite_sale(void) {
    sqlite3_open(":memory:", &db);
    db_execute(db, "CREATE TABLE compact_discs (id INTEGER PRIMARY KEY, price REAL);");
    db_execute(db, "CREATE TABLE arrivals (id INTEGER PRIMARY KEY, cd_id INTEGER, quantity INTEGER);");
    db_execute(db, "CREATE TABLE sales (id INTEGER PRIMARY KEY, cd_id INTEGER, quantity INTEGER);");
    
    db_execute(db, "INSERT INTO compact_discs (id, price) VALUES (1, 10.00);");
    db_execute(db, "INSERT INTO arrivals (cd_id, quantity) VALUES (1, 100);");
    return 0;
}

int clean_suite_sale(void) {
    sqlite3_close(db);
    return 0;
}

void test_check_stock_sufficient(void) {
    int remaining = 0;
    sqlite3_exec(db, "SELECT COALESCE((SELECT SUM(quantity) FROM arrivals WHERE cd_id=1),0) - "
                     "COALESCE((SELECT SUM(quantity) FROM sales WHERE cd_id=1),0);", 
                     sum_callback, &remaining, NULL);
    CU_ASSERT(remaining >= 50);
}

void test_add_arrival_increases_stock(void) {
    int old_total = 0;
    sqlite3_exec(db, "SELECT SUM(quantity) FROM arrivals WHERE cd_id=1;", sum_callback, &old_total, NULL);
    
    db_execute(db, "INSERT INTO arrivals (cd_id, quantity) VALUES (1, 30);");
    
    int new_total = 0;
    sqlite3_exec(db, "SELECT SUM(quantity) FROM arrivals WHERE cd_id=1;", sum_callback, &new_total, NULL);
    CU_ASSERT(new_total == old_total + 30);
}

void test_add_sale_decreases_stock(void) {
    int old_total = 0;
    sqlite3_exec(db, "SELECT SUM(quantity) FROM sales WHERE cd_id=1;", sum_callback, &old_total, NULL);
    
    db_execute(db, "INSERT INTO sales (cd_id, quantity) VALUES (1, 10);");
    
    int new_total = 0;
    sqlite3_exec(db, "SELECT SUM(quantity) FROM sales WHERE cd_id=1;", sum_callback, &new_total, NULL);
    CU_ASSERT(new_total == old_total + 10);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Sales Operations Tests", init_suite_sale, clean_suite_sale);
    CU_add_test(suite, "Check stock sufficient", test_check_stock_sufficient);
    CU_add_test(suite, "Add arrival increases stock", test_add_arrival_increases_stock);
    CU_add_test(suite, "Add sale decreases stock", test_add_sale_decreases_stock);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
