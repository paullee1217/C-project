#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void update_data(const char *command) {
    char table_name[50];
    char updates[256];
    char condition[50];
    int target_id;

    printf("DEBUG: Command received: %s\n", command);

    // 명령어 파싱
    if (sscanf(command, "UPDATE %49s SET %255[^W] WHERE %49s", table_name, updates, condition) != 3) {
        printf("[ERROR] Invalid UPDATE command.\n");
        return;
    }

    // 조건에서 ID 추출
    if (sscanf(condition, "id=%d", &target_id) != 1) {
        printf("[ERROR] Invalid WHERE condition. Use 'id=VALUE'.\n");
        return;
    }

    FILE *file = fopen(table_name, "r");
    if (file == NULL) {
        printf("[ERROR] Table '%s' does not exist.\n", table_name);
        return;
    }

    FILE *temp_file = fopen("temp_table.txt", "w");
    if (temp_file == NULL) {
        perror("[ERROR] Could not create temporary file.");
        fclose(file);
        return;
    }

    char buffer[256];
    int found = 0;

    // 첫 줄은 컬럼 헤더
    if (fgets(buffer, sizeof(buffer), file)) {
        fprintf(temp_file, "%s", buffer); // 헤더 복사
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        int current_id;
        char name[50];
        int age;

        // 데이터 파싱
        if (sscanf(buffer, "%d,%49[^,],%d", &current_id, name, &age) != 3) {
            fprintf(temp_file, "%s", buffer); // 잘못된 데이터는 그대로 복사
            continue;
        }

        // ID가 일치하면 업데이트
        if (current_id == target_id) {
            char new_name[50] = "";
            int new_age = age; // 기본 값은 기존 값

            // 업데이트 데이터 파싱
            sscanf(updates, "name='%49[^']', age=%d", new_name, &new_age);

            // 새로운 데이터 작성 (문자열에 따옴표 포함)
            if (strlen(new_name) == 0) strcpy(new_name, name); // 기존 값 유지
            fprintf(temp_file, "%d,'%s',%d\n", current_id, new_name, new_age); // 따옴표 추가
            found = 1;
        } else {
            fprintf(temp_file, "%s", buffer); // 업데이트 대상이 아니면 그대로 복사
        }
    }

    fclose(file);
    fclose(temp_file);

    // 파일 교체
    if (found) {
        remove(table_name);
        rename("temp_table.txt", table_name);
        printf("[SUCCESS] Data with ID %d updated in '%s'.\n", target_id, table_name);
    } else {
        printf("[ERROR] ID %d not found in '%s'.\n", target_id, table_name);
        remove("temp_table.txt");
    }
}
