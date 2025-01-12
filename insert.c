#include <stdio.h>
#include <string.h>
#include "insert.h"

void insert_into(const char *command) {
    char table_name[50];
    char values[100];

    sscanf(command, "INSERT INTO %s VALUES (%[^)])", table_name, values);

    FILE *file = fopen(table_name, "r+");
    if (file == NULL) {
        printf("[ERROR] Table '%s' does not exist.\n", table_name);
        return;
    }

    fseek(file, 0, SEEK_END);
    fprintf(file, "%s\n", values);
    fclose(file);

    printf("[SUCCESS] Data inserted into table '%s': %s\n", table_name, values);
}
