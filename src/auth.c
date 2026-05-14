#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "auth.h"

int authenticate(sqlite3* db) {
    char username[50], password[50];
    printf("Username: ");
    scanf("%49s", username);
    printf("Password: ");
    scanf("%49s", password);

    if (strcmp(username, "manager") == 0 && strcmp(password, "1234") == 0) {
        printf("Welcome, Manager!\n");
        return 1; 
    } else if (strcmp(username, "customer") == 0 && strcmp(password, "5678") == 0) {
        printf("Welcome, Customer!\n");
        return 0; 
    } else {
        printf("Invalid credentials.\n");
        return -1;
    }
}
