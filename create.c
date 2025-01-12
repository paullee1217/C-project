#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void create_table(const char *command) {
    char table_name[50]; // 테이블 이름 저장
    char columns[256];   // 컬럼 정의 저장

    // 명령어 디버깅 출력
    printf("DEBUG: Command received: %s\n", command);

    // 명령어 파싱: "CREATE TABLE <table_name> (<columns>)"
    if (sscanf(command, "CREATE TABLE %49s (%255[^)])", table_name, columns) != 2) {
        printf("[ERROR] Invalid CREATE TABLE command.\n");
        return;
    }

    // 괄호와 세미콜론 제거
    table_name[strcspn(table_name, "(")] = '\0'; // 테이블 이름에서 괄호 제거

    // 파일 존재 여부 확인
    FILE *file = fopen(table_name, "r");
    if (file != NULL) {
        fclose(file);
        printf("[ERROR] Table '%s' already exists.\n", table_name);
        return;
    }

    // 테이블 파일 생성
    file = fopen(table_name, "w");
    if (file == NULL) {
        perror("[ERROR] Could not create table");
        return;
    }

    // 컬럼 정의 저장
    fprintf(file, "%s\n", columns);
    fclose(file);

    printf("[SUCCESS] Table '%s' created with columns: %s\n", table_name, columns);
}
