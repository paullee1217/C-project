#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void select_all(const char *command) {
    char table_name[50];

    // 명령어 디버깅 출력
    printf("DEBUG: Command received: %s\n", command);

    // 1. "SELECT" 확인
    const char *select_keyword = "SELECT * FROM ";
    if (strncmp(command, select_keyword, strlen(select_keyword)) != 0) {
        printf("[ERROR] Invalid SELECT command.\n");
        return;
    }

    // 2. 테이블 이름 추출
    const char *table_start = command + strlen(select_keyword);
    sscanf(table_start, "%49[^;]", table_name); // 세미콜론 제거

    // 파일 열기
    FILE *file = fopen(table_name, "r");
    if (file == NULL) {
        printf("[ERROR] Table '%s' does not exist.\n", table_name);
        return;
    }

    char buffer[256];
    char columns[256];
    char *column_headers[10];
    int column_widths[10] = {0}; // 각 열의 최대 너비
    int num_columns = 0;

    // 3. 파일 첫 줄에서 컬럼 정의 읽기
    if (fgets(columns, sizeof(columns), file) == NULL) {
        printf("[ERROR] Table '%s' is empty or corrupted.\n", table_name);
        fclose(file);
        return;
    }

    // 줄바꿈 문자 제거
    columns[strcspn(columns, "\n")] = '\0';

    // 4. 컬럼 정의 파싱 및 열 너비 초기화
    char *token = strtok(columns, ",");
    while (token != NULL) {
        column_headers[num_columns] = strdup(token); // 동적 메모리 할당
        column_widths[num_columns] = strlen(token) + 2; // 초기 너비 설정 (여백 포함)
        num_columns++;
        token = strtok(NULL, ",");
    }

    // 5. 데이터 행의 최대 길이 계산
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // 줄바꿈 제거
        token = strtok(buffer, ",");
        for (int i = 0; i < num_columns && token != NULL; i++) {
            int data_length = strlen(token) + 2; // 데이터 길이 + 여백
            if (data_length > column_widths[i]) {
                column_widths[i] = data_length; // 열 너비 갱신
            }
            token = strtok(NULL, ",");
        }
    }

    // 파일 포인터를 처음으로 되돌림 (데이터 재출력 준비)
    fseek(file, 0, SEEK_SET);
    fgets(columns, sizeof(columns), file); // 컬럼 헤더 스킵

    // 6. 테이블 출력 상단
    printf("\n+");
    for (int i = 0; i < num_columns; i++) {
        for (int j = 0; j < column_widths[i]; j++) printf("-");
        printf("+");
    }
    printf("\n|");

    // 컬럼 이름 출력
    for (int i = 0; i < num_columns; i++) {
        printf(" %-*s |", column_widths[i] - 1, column_headers[i]);
    }
    printf("\n+");
    for (int i = 0; i < num_columns; i++) {
        for (int j = 0; j < column_widths[i]; j++) printf("-");
        printf("+");
    }
    printf("\n");

    // 7. 테이블 내용 출력
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // 줄바꿈 제거

        printf("|");
        token = strtok(buffer, ",");
        for (int i = 0; i < num_columns && token != NULL; i++) {
            printf(" %-*s |", column_widths[i] - 1, token);
            token = strtok(NULL, ",");
        }
        printf("\n");
    }

    // 8. 테이블 출력 하단
    printf("+");
    for (int i = 0; i < num_columns; i++) {
        for (int j = 0; j < column_widths[i]; j++) printf("-");
        printf("+");
    }
    printf("\n");

    // 동적 메모리 해제
    for (int i = 0; i < num_columns; i++) {
        free(column_headers[i]);
    }

    fclose(file);
}
