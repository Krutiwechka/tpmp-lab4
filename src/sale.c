#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "sale.h"
#include "db.h"
#include "utils.h"

int check_stock(sqlite3* db, int cd_id, int quantity) {
    int arrived = 0, sold = 0;
    
    char sql_arrived[256];
    sprintf(sql_arrived, "SELECT COALESCE(SUM(quantity),0) FROM arrivals WHERE cd_id=%d;", cd_id);
    sqlite3_exec(db, sql_arrived, stock_callback, &arrived, NULL);
    
    char sql_sold[256];
    sprintf(sql_sold, "SELECT COALESCE(SUM(quantity),0) FROM sales WHERE cd_id=%d;", cd_id);
    sqlite3_exec(db, sql_sold, stock_callback, &sold, NULL);
    
    int remaining = arrived - sold;
    if (quantity <= remaining) return 1;
    return 0;
}

int stock_callback(void* data, int argc, char** argv, char** col_names) {
    int* value = (int*)data;
    if (argc > 0 && argv[0]) {
        *value = atoi(argv[0]);
    }
    return 0;
}

void add_arrival(sqlite3* db) {
    int cd_id = get_int_input("CD id: ");
    int quantity = get_int_input("Quantity: ");
    char date[11];
    printf("Date (YYYY-MM-DD): "); 
    scanf("%10s", date);

    char sql[512];
    sprintf(sql, "INSERT INTO arrivals (operation_date, operation_code, cd_id, quantity) VALUES ('%s', 'ARRIVAL', %d, %d);",
            date, cd_id, quantity);
    db_execute(db, sql);
    printf("Arrival recorded.\n");
}

void add_sale(sqlite3* db) {
    int cd_id = get_int_input("CD id: ");
    int quantity = get_int_input("Quantity: ");
    char date[11];
    printf("Date (YYYY-MM-DD): "); 
    scanf("%10s", date);

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
