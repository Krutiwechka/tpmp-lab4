#include <stdio.h>
#include <sqlite3.h>
#include "db.h"
#include "auth.h"
#include "cd.h"
#include "sale.h"
#include "report.h"

int main() {
    sqlite3* db = db_connect("music_salon.db");
    if (!db) return 1;

    int role = authenticate(db);
    if (role == -1) {
        db_disconnect(db);
        return 1;
    }

    int choice;
    do {
        printf("\n=== MUSIC SALON ===\n");
        if (role == 1) { // менеджер
            printf("1. Add CD\n2. Update CD\n3. Delete CD\n4. Add arrival\n5. Add sale\n");
            printf("6. Report: remaining CDs\n7. Report: sales by CD/period\n");
            printf("8. Report: most popular CD\n9. Report: most popular performer\n");
            printf("10. Report: by author\n0. Exit\nChoice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1: add_cd(db); break;
                case 2: update_cd(db); break;
                case 3: delete_cd(db); break;
                case 4: add_arrival(db); break;
                case 5: add_sale(db); break;
                case 6: report_remaining_cds(db); break;
                case 7: /* report by period stub */ break;
                case 8: report_most_popular_cd(db); break;
                case 9: report_most_popular_performer(db); break;
                case 10: report_by_author(db); break;
            }
        } else { // покупатель
            printf("6. Report: remaining CDs\n8. Report: most popular CD\n9. Report: most popular performer\n0. Exit\nChoice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 6: report_remaining_cds(db); break;
                case 8: report_most_popular_cd(db); break;
                case 9: report_most_popular_performer(db); break;
            }
        }
    } while (choice != 0);

    db_disconnect(db);
    return 0;
}
