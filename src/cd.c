#include <stdio.h>
#include <sqlite3.h>
#include "cd.h"
#include "db.h"
#include "utils.h"

void add_cd(sqlite3* db) {
    char code[30], date[11], manufacturer[100];
    double price;
    printf("Enter CD code: "); scanf("%29s", code);
    printf("Manufacture date (YYYY-MM-DD): "); scanf("%10s", date);
    printf("Manufacturer: "); scanf("%99s", manufacturer);
    printf("Price: "); scanf("%lf", &price);

    char sql[512];
    sprintf(sql, "INSERT INTO compact_discs (code, manufacture_date, manufacturer, price) VALUES ('%s', '%s', '%s', %f);",
            code, date, manufacturer, price);
    db_execute(db, sql);
    printf("CD added.\n");
}

void update_cd(sqlite3* db) {
    int id = get_int_input("Enter CD id to update: ");
    double price = get_int_input("Enter new price: ");
    char sql[256];
    sprintf(sql, "UPDATE compact_discs SET price = %f WHERE id = %d;", price, id);
    db_execute(db, sql);
}

void delete_cd(sqlite3* db) {
    int id = get_int_input("Enter CD id to delete: ");
    char sql[256];
    sprintf(sql, "DELETE FROM compact_discs WHERE id = %d;", id);
    db_execute(db, sql);
}

void list_cds(sqlite3* db) {
    sqlite3_exec(db, "SELECT * FROM compact_discs;", db_callback, 0, 0);
}
