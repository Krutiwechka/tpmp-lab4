#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "auth.h"
#include "db.h"

int get_role_callback(void* data, int argc, char** argv, char** col_names) {
    char** role = (char**)data;
    if (argc > 0 && argv[0]) {
        *role = strdup(argv[0]);
    }
    return 0;
}

int authenticate(sqlite3* db) {
    char username[50], password[50];
    printf("Username: ");
    scanf("%49s", username);
    printf("Password: ");
    scanf("%49s", password);

    char sql[256];
    sprintf(sql, "SELECT role FROM users WHERE username='%s' AND password='%s';", username, password);
    
    char* role = NULL;
    sqlite3_exec(db, sql, get_role_callback, &role, NULL);
    
    if (role == NULL) {
        printf("Invalid credentials.\n");
        return -1;
    }
    
    printf("Welcome, %s!\n", username);
    if (strcmp(role, "manager") == 0) {
        free(role);
        return 1;
    }
    free(role);
    return 0;
}
