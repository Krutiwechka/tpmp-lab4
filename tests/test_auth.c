#include <CUnit/Basic.h>
#include <sqlite3.h>
#include <string.h>
#include "db.h"

static sqlite3* db;

// Простой callback для проверки существования пользователя
static int check_user_exists(void* data, int argc, char** argv, char** col_names) {
    int* exists = (int*)data;
    if (argc > 0 && argv[0]) {
        *exists = 1;
    }
    return 0;
}

int init_suite_auth(void) {
    sqlite3_open(":memory:", &db);
    // Создаём таблицу users для тестов
    db_execute(db, "CREATE TABLE users (id INTEGER PRIMARY KEY, username TEXT, password TEXT, role TEXT);");
    db_execute(db, "INSERT INTO users (username, password, role) VALUES ('manager', '1234', 'manager');");
    db_execute(db, "INSERT INTO users (username, password, role) VALUES ('customer', '5678', 'customer');");
    return 0;
}

int clean_suite_auth(void) {
    sqlite3_close(db);
    return 0;
}

void test_valid_manager_login(void) {
    int exists = 0;
    char sql[256];
    sprintf(sql, "SELECT role FROM users WHERE username='manager' AND password='1234';");
    sqlite3_exec(db, sql, check_user_exists, &exists, 0);
    CU_ASSERT(exists == 1);
}

void test_valid_customer_login(void) {
    int exists = 0;
    char sql[256];
    sprintf(sql, "SELECT role FROM users WHERE username='customer' AND password='5678';");
    sqlite3_exec(db, sql, check_user_exists, &exists, 0);
    CU_ASSERT(exists == 1);
}

void test_invalid_login(void) {
    int exists = 0;
    char sql[256];
    sprintf(sql, "SELECT role FROM users WHERE username='fake' AND password='xxx';");
    sqlite3_exec(db, sql, check_user_exists, &exists, 0);
    CU_ASSERT(exists == 0);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Authentication Tests", init_suite_auth, clean_suite_auth);
    CU_add_test(suite, "Valid manager login", test_valid_manager_login);
    CU_add_test(suite, "Valid customer login", test_valid_customer_login);
    CU_add_test(suite, "Invalid login", test_invalid_login);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
