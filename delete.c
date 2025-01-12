#include <stdio.h>
#include <string.h>
#include "delete.h"

void delete_from(const char *command) {
    char table_name[50];
    int target_id;

    sscanf(command, "DELETE FROM %s WHERE id=%d", table_name, &target_id);

    FILE *file = fopen(table_name, "r");
    if (file == NULL) {
        printf("[ERROR] Table '%s' does not exist.\n", table_name);
        return;
    }

    FILE *temp = fopen("temp_table.txt", "w");
    if (temp == NULL) {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

    char buffer[256];
    int found = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        int current_id;
        sscanf(buffer, "%d,", &current_id);

        if (current_id != target_id) {
            fputs(buffer, temp);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp);

    remove(table_name);
    rename("temp_table.txt", table_name);

    if (found) {
        printf("[SUCCESS] Data with ID %d deleted from '%s'.\n", target_id, table_name);
    } else {
        printf("[ERROR] ID %d not found in '%s'.\n", target_id, table_name);
    }
}
