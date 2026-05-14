#include <CUnit/Basic.h>

void test_utils_placeholder(void) {
    CU_ASSERT(1 == 1);
}

void test_utils_another(void) {
    CU_ASSERT(1 == 1);
}

void test_utils_third(void) {
    CU_ASSERT(1 == 1);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Utils Tests", 0, 0);
    CU_add_test(suite, "Placeholder test 1", test_utils_placeholder);
    CU_add_test(suite, "Placeholder test 2", test_utils_another);
    CU_add_test(suite, "Placeholder test 3", test_utils_third);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
