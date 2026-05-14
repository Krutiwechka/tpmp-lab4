#include <CUnit/Basic.h>
#include <sqlite3.h>
#include <stdlib.h>
#include "db.h"

static sqlite3* db;

// Правильный callback для sqlite3_exec
static int count_callback(void* data, int argc, char** argv, char** col_names) {
    int* count = (int*)data;
    if (argc > 0 && argv[0]) {
        *count = atoi(argv[0]);
    }
    return 0;
}

static int price_callback(void* data, int argc, char** argv, char** col_names) {
    double* price = (double*)data;
    if (argc > 0 && argv[0]) {
        *price = atof(argv[0]);
    }
    return 0;
}

int init_suite_cd(void) {
    sqlite3_open(":memory:", &db);
    db_execute(db, "CREATE TABLE compact_discs (id INTEGER PRIMARY KEY, code TEXT, manufacture_date TEXT, manufacturer TEXT, price REAL);");
    return 0;
}

int clean_suite_cd(void) {
    sqlite3_close(db);
    return 0;
}

void test_add_cd(void) {
    db_execute(db, "INSERT INTO compact_discs (code, manufacture_date, manufacturer, price) VALUES ('CD999', '2025-01-01', 'TestLabel', 19.99);");
    
    int count = 0;
    sqlite3_exec(db, "SELECT COUNT(*) FROM compact_discs WHERE code='CD999';", count_callback, &count, NULL);
    CU_ASSERT(count == 1);
}

void test_update_cd_price(void) {
    db_execute(db, "INSERT INTO compact_discs (code, manufacturer, price) VALUES ('CD888', 'Test', 10.00);");
    db_execute(db, "UPDATE compact_discs SET price = 15.00 WHERE code='CD888';");
    
    double price = 0;
    sqlite3_exec(db, "SELECT price FROM compact_discs WHERE code='CD888';", price_callback, &price, NULL);
    CU_ASSERT(price == 15.00);
}

void test_delete_cd(void) {
    db_execute(db, "INSERT INTO compact_discs (code, manufacturer, price) VALUES ('CD777', 'Test', 5.00);");
    db_execute(db, "DELETE FROM compact_discs WHERE code='CD777';");
    
    int count = 0;
    sqlite3_exec(db, "SELECT COUNT(*) FROM compact_discs WHERE code='CD777';", count_callback, &count, NULL);
    CU_ASSERT(count == 0);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("CD Operations Tests", init_suite_cd, clean_suite_cd);
    CU_add_test(suite, "Add CD", test_add_cd);
    CU_add_test(suite, "Update CD price", test_update_cd_price);
    CU_add_test(suite, "Delete CD", test_delete_cd);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
