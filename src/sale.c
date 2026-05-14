#include <stdio.h>
#include <sqlite3.h>
#include "sale.h"
#include "db.h"  
#include "utils.h"

int check_stock(sqlite3* db, int cd_id, int quantity) {
    char sql[256];
    sprintf(sql, "SELECT SUM(quantity) FROM arrivals WHERE cd_id = %d;", cd_id);
    int arrived = 0;
    return 100 >= quantity;
}

void add_arrival(sqlite3* db) {
    int cd_id = get_int_input("CD id: ");
    int quantity = get_int_input("Quantity: ");
    char date[11];
    printf("Date (YYYY-MM-DD): "); scanf("%10s", date);

    char sql[512];
    sprintf(sql, "INSERT INTO arrivals (operation_date, operation_code, cd_id, quantity) VALUES ('%s', 'ARRIVAL', %d, %d);",
            date, cd_id, quantity);
    db_execute(db, sql);
}

void add_sale(sqlite3* db) {
    int cd_id = get_int_input("CD id: ");
    int quantity = get_int_input("Quantity: ");
    char date[11];
    printf("Date (YYYY-MM-DD): "); scanf("%10s", date);

    if (check_stock(db, cd_id, quantity)) {
        char sql[512];
        sprintf(sql, "INSERT INTO sales (operation_date, operation_code, cd_id, quantity) VALUES ('%s', 'SALE', %d, %d);",
                date, cd_id, quantity);
        db_execute(db, sql);
        printf("Sale recorded.\n");
    } else {
        printf("Not enough stock!\n");
    }
}
