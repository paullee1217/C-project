#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 테이블 생성 함수
void create_table(const char *table_name, const char *columns) {
    FILE *file = fopen(table_name, "r");
    if (file != NULL) {
        fclose(file);
        printf("[ERROR] Table '%s' already exists.\n", table_name);
        return;
    }

    file = fopen(table_name, "w");
    if (file == NULL) {
        perror("Error creating table");
        return;
    }

    // 컬럼 구조를 파일의 첫 번째 줄에 저장
    fprintf(file, "%s\n", columns);
    fclose(file);

    printf("[SUCCESS] Table '%s' created with columns: %s\n", table_name, columns);
}

// 데이터 삽입 함수
void insert_data(const char *table_name, const char *data) {
    FILE *file = fopen(table_name, "r+");
    if (file == NULL) {
        printf("[ERROR] Table '%s' does not exist.\n", table_name);
        return;
    }

    fseek(file, 0, SEEK_END); // 파일의 끝으로 이동
    fprintf(file, "%s\n", data);
    fclose(file);

    printf("[SUCCESS] Data inserted into table '%s': %s\n", table_name, data);
}

// 데이터 검색 함수
void select_all(const char *table_name) {
    FILE *file = fopen(table_name, "r");
    if (file == NULL) {
        printf("[ERROR] Table '%s' does not exist.\n", table_name);
        return;
    }

    char buffer[256];
    printf("Data from table '%s':\n", table_name);

    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
}

// 메인 함수
int main() {
    char table_name[50], columns[100], data[100];
    int choice;

    while (1) {
        printf("\n=== Simple Database System ===\n");
        printf("1. Create Table\n");
        printf("2. Insert Data\n");
        printf("3. Select All Data\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // 버퍼 비우기

        switch (choice) {
        case 1:
            printf("Enter table name: ");
            scanf("%s", table_name);
            getchar(); // 버퍼 비우기
            printf("Enter column structure (e.g., ID,Name,Age): ");
            fgets(columns, sizeof(columns), stdin);
            columns[strcspn(columns, "\n")] = '\0'; // 줄바꿈 문자 제거
            create_table(table_name, columns);
            break;

        case 2:
            printf("Enter table name: ");
            scanf("%s", table_name);
            getchar(); // 버퍼 비우기
            printf("Enter data (e.g., 1,Alice,24): ");
            fgets(data, sizeof(data), stdin);
            data[strcspn(data, "\n")] = '\0'; // 줄바꿈 문자 제거
            insert_data(table_name, data);
            break;

        case 3:
            printf("Enter table name: ");
            scanf("%s", table_name);
            select_all(table_name);
            break;

        case 4:
            printf("Exiting program.\n");
            exit(0);

        default:
            printf("[ERROR] Invalid choice. Try again.\n");
        }
    }

    return 0;
}
