#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "create.h"
#include "insert.h"
#include "select.h"
#include "delete.h"
#include "update.h"

void parse_sql_command(const char *command);

int main() {
    char command[256];

    printf("Simple SQL Database\n");
    printf("Enter your SQL command (type EXIT to quit):\n");

    while (1) {
        printf("> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; // Remove newline character

        if (strcmp(command, "EXIT") == 0) {
            printf("Exiting...\n");
            break;
        }

        parse_sql_command(command);
    }

    return 0;
}

void parse_sql_command(const char *command) {
    if (strncmp(command, "CREATE TABLE", 12) == 0) {
        create_table(command);
    } else if (strncmp(command, "INSERT INTO", 11) == 0) {
        insert_into(command);
    } else if (strncmp(command, "SELECT * FROM", 13) == 0) {
        select_all(command);
    } else if (strncmp(command, "DELETE FROM", 11) == 0) {
        delete_from(command);
    } else if (strncmp(command, "UPDATE", 6) == 0) {
        update_data(command);
    } else {
        printf("[ERROR] Unknown command: %s\n", command);
    }
}
