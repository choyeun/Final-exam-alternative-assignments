#pragma warning(disable:4996)  // 보안 경고 비활성화
#include <stdio.h>  // 표준 입출력 함수 사용
#include <string.h> // 문자열 처리 함수 사용
#include <stdlib.h> // 표준 라이브러리 함수 사용
#include <windows.h> // Windows API 함수 사용
#include <conio.h>  // 콘솔 입출력 함수 사용
#include <direct.h> // 디렉토리 처리 함수 사용
#include <io.h>     // 파일 입출력 함수 사용

// 최대 학생 수 정의
#define MAX_STUDENTS 100

// 학생 구조체 정의
typedef struct {
    char sid[10];   // 학번
    char name[10];  // 이름
    int score1;     // 국어 점수
    int score2;     // 영어 점수
    int score3;     // 수학 점수
    int total;      // 총점
    int average;    // 평균
    char grade;     // 등급
} Student;

// 함수 선언
void gotoxy(int x, int y);
int InputData();
int ViewData();
int EditData();
int SaveReport0();
int SaveReport1();
int SaveReport2();
int SaveReport3();
int SaveCSV();
int SaveHTML();
void ClearTempFolder();
void CalculateTotalAverage(Student* student);
char Grade(int X);
int CompareByTotal(const void* a, const void* b);
int CompareByName(const void* a, const void* b);
int CompareByID(const void* a, const void* b);
void SearchAndModifyStudent();

// 전역 변수 선언
FILE* fp1, * fp2;
char fname1[50], fname2[50];
int N, CNT = 0;
Student students[MAX_STUDENTS];

// 콘솔 커서 위치 이동 함수
void gotoxy(int x, int y) {
    COORD Pos = { x - 1, y - 1 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 메인 함수
int main(void) {
    while (1) {
        gotoxy(2, 2);
        // 메인 메뉴 출력
        printf("\n\t 유수봉의 성적처리 시스템 ");
        printf("\n\t ========================= ");
        printf("\n\t << 작업 목록 >>-------------------------------- ");
        printf("\n\t   1. 새로운 성적 데이터 입력받아 파일에 저장하기 ");
        printf("\n\t   2. 파일에 저장된 성적 데이터 화면에 표시하기 ");
        printf("\n\t   3. 파일에 저장된 성적 데이터를 이용한 성적 집계표 파일 만들기 ");
        printf("\n\t   4. 성적집계표 성적순 출력 ");
        printf("\n\t   5. 성적집계표 이름순 출력 ");
        printf("\n\t   6. 성적집계표 학번순 출력 ");
        printf("\n\t   7. 성적데이터를 Excel 파일(CSV)로 출력 ");
        printf("\n\t   8. 성적데이터를 Web Page(HTML)로 출력 ");
        printf("\n\t   9. 개인별 성적 데이터 관리 ");
        printf("\n\t   0. 작업 끝 ");
        printf("\n\t ------------------------<< 번호를 선택하시오 >>");
        N = getch();  // 사용자 입력 받음

        // 사용자가 선택한 메뉴에 따라 함수 호출
        switch (N) {
        case  '0':  return 0;  // 프로그램 종료
        case  '1':  ClearTempFolder(); InputData(); break;  // 데이터 입력
        case  '2':  ViewData();  break;  // 데이터 조회
        case  '3':  SaveReport0();   break;  // 성적집계표 기본 출력
        case  '4':  SaveReport1();   break;  // 성적집계표 성적순 출력
        case  '5':  SaveReport2();   break;  // 성적집계표 이름순 출력
        case  '6':  SaveReport3();   break;  // 성적집계표 학번순 출력
        case  '7':  SaveCSV();   break;  // CSV 파일로 저장
        case  '8':  SaveHTML();  break;  // HTML 파일로 저장
        case  '9':  SearchAndModifyStudent();  break;  // 데이터 편집
        }
        printf("\n\t -------------------------------------<< 확인 >>");
        N = getch();  // 사용자 확인 대기
        system("cls");  // 화면 지움
    }
}

// temp 폴더의 내용물을 삭제하는 함수
void ClearTempFolder() {
    char path[50];
    sprintf(path, "c:/Temp/*");  // temp 폴더 내 모든 파일 경로

    struct _finddata_t fileinfo;
    intptr_t handle = _findfirst(path, &fileinfo);  // 첫 번째 파일 찾기

    if (handle != -1) {
        do {
            // 현재 파일이 "." 또는 ".."이 아닌 경우 삭제
            if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
                char filepath[100];
                sprintf(filepath, "c:/Temp/%s", fileinfo.name);  // 파일 전체 경로
                remove(filepath);  // 파일 삭제
            }
        } while (_findnext(handle, &fileinfo) == 0);  // 다음 파일 찾기
        _findclose(handle);  // 파일 탐색 종료
    }
}

// 성적 데이터를 입력받아 파일에 저장하는 함수
int InputData() {
    printf("\n\t 본 작업을 수행하면 기존 데이터를 모두 삭제됩니다.");
    printf("\n\t 그래도 작업을 계속 하시겠습니까? (Y/N) ->");
    N = getch();
    if ((N == 'Y') || (N == 'y')) {
        strcpy(fname1, "c:/Temp/성적데이터.txt");  // 저장할 파일 경로

        // 파일 쓰기 모드로 열기
        if ((fp1 = fopen(fname1, "w")) == NULL) {
            printf("\n(!) \"%s\" 을 열 수 없습니다.\n", fname1);
            return 0;
        }
        else {
            printf("\n(!) 새로운 성적 파일 \"%s\"을 만들었습니다.\n", fname1);
        }

        CNT = 0;  // 데이터 개수 초기화
        while (1) {
            system("cls");
            gotoxy(2, 5);
            printf("-. 학번 :                  ('.' 입력하면 입력을 종료합니다.) : ");
            while (1) {
                gotoxy(12, 5);
                scanf("%9s", students[CNT].sid);  // 학번 입력
                if (strlen(students[CNT].sid) > 0) break;
            }
            if (strcmp(students[CNT].sid, ".") == 0) { break; }  // '.' 입력 시 종료

            gotoxy(2, 6);    printf("-. 이름 : ");   scanf("%9s", students[CNT].name);  // 이름 입력
            gotoxy(2, 8);    printf("-. 국어 : ");   scanf("%d", &students[CNT].score1);  // 국어 점수 입력
            gotoxy(2, 9);    printf("-. 영어 : ");   scanf("%d", &students[CNT].score2);  // 영어 점수 입력
            gotoxy(2, 10);   printf("-. 수학 : ");   scanf("%d", &students[CNT].score3);  // 수학 점수 입력

            // 총점과 평균 계산
            CalculateTotalAverage(&students[CNT]);
            // 파일에 데이터 저장
            fprintf(fp1, "%10s %5s %3d %3d %3d\n", students[CNT].sid, students[CNT].name, students[CNT].score1, students[CNT].score2, students[CNT].score3);
            CNT++;  // 데이터 개수 증가
            fflush(stdin);  // 입력 버퍼 비우기
            printf("\n(!) 총 %d 건의 데이터를 저장하였습니다.(',' 입력끝)", CNT);
            N = getch();
            if (N == '.') break;  // '.' 입력 시 종료
        }
        printf("\n(!) 총 %d 건의 데이터를 저장하였습니다.", CNT);
        fclose(fp1);  // 파일 닫기
        printf("\n(!) 파일을 닫았습니다.");
    }
    return 0;
}

// 파일에 있는 성적 데이터를 화면으로 출력하는 함수
int ViewData() {
    strcpy(fname1, "c:/Temp/성적데이터.txt");  // 파일 경로

    // 파일 읽기 모드로 열기
    if ((fp1 = fopen(fname1, "r")) == NULL) {
        printf("\n(!) \"%s\" 을 열 수 없습니다.\n", fname1);
        return 0;
    }
    else {
        printf("\n(!) \"%s\" 을 열었습니다.\n", fname1);
    }

    printf("\n(!) %s 파일 내용은 다음과 같습니다.\n", fname1);
    printf("\n\t 성적데이터 파일 내용 확인");
    printf("\n\t ========= ====== ==== ==== ==== ");

    CNT = 0;  // 데이터 개수 초기화
    while (!feof(fp1)) {
        fscanf(fp1, "%9s %9s %d %d %d", students[CNT].sid, students[CNT].name, &students[CNT].score1, &students[CNT].score2, &students[CNT].score3);  // 파일에서 데이터 읽기
        if (feof(fp1)) { break; }  // 파일 끝에 도달하면 종료
        printf("\n\t %s %s %4d %4d %4d", students[CNT].sid, students[CNT].name, students[CNT].score1, students[CNT].score2, students[CNT].score3);  // 데이터 출력
        CNT++;
    }

    printf("\n\t ========= ====== ==== ==== ==== ");
    fclose(fp1);  // 파일 닫기
    printf("\n(!) 파일을 닫았습니다.");

    return 0;
}

// 학생의 총점과 평균을 계산하는 함수
void CalculateTotalAverage(Student* student) {
    student->total = student->score1 + student->score2 + student->score3;  // 총점 계산
    student->average = student->total / 3;  // 평균 계산
    student->grade = Grade(student->average);  // 등급 계산
}

// 점수에 따라 학점을 계산하는 함수
char Grade(int X) {
    char grade;
    if (X >= 90) grade = 'A';
    else if (X >= 80) grade = 'B';
    else if (X >= 70) grade = 'C';
    else if (X >= 60) grade = 'D';
    else grade = 'F';
    return grade;
}

// 총점에 따라 학생을 비교하는 함수 (내림차순 정렬)
int CompareByTotal(const void* a, const void* b) {
    return ((Student*)b)->total - ((Student*)a)->total;
}

// 이름에 따라 학생을 비교하는 함수 (오름차순 정렬)
int CompareByName(const void* a, const void* b) {
    return strcmp(((Student*)a)->name, ((Student*)b)->name);
}

// 학번에 따라 학생을 비교하는 함수 (오름차순 정렬)
int CompareByID(const void* a, const void* b) {
    return strcmp(((Student*)a)->sid, ((Student*)b)->sid);
}

// 개인별 성적 데이터를 관리하는 함수
void SearchAndModifyStudent() {
    char searchID[10];
    char searchName[10];
    int found = 0;
    int foundIndex[MAX_STUDENTS];
    int foundCount = 0;

    strcpy(fname1, "c:/Temp/성적데이터.txt");

    // 파일 읽기 모드로 열기
    if ((fp1 = fopen(fname1, "r")) == NULL) {
        printf("\n(!) \"%s\" 을 열 수 없습니다.\n", fname1);
        return;
    }
    else {
        printf("\n(!) \"%s\" 을 열었습니다.\n", fname1);
    }

    // 데이터 파일에서 읽기
    CNT = 0;
    while (!feof(fp1)) {
        fscanf(fp1, "%9s %9s %d %d %d", students[CNT].sid, students[CNT].name, &students[CNT].score1, &students[CNT].score2, &students[CNT].score3);
        if (feof(fp1)) { break; }
        CalculateTotalAverage(&students[CNT]);
        CNT++;
    }
    fclose(fp1);

    // 검색할 학번 또는 이름 입력 받기
    printf("\n 검색할 학번 또는 이름을 입력하세요: ");
    scanf("%s", searchID);

    // 학번 또는 이름으로 학생 데이터 검색
    for (int i = 0; i < CNT; i++) {
        if (strcmp(students[i].sid, searchID) == 0 || strcmp(students[i].name, searchID) == 0) {
            foundIndex[foundCount] = i;
            foundCount++;
        }
    }

    if (foundCount == 0) {
        printf("\n 학생을 찾을 수 없습니다.\n");
    }
    else if (foundCount == 1) {
        found = 1;
    }
    else {
        printf("\n 동명이인이 있습니다. 해당 학생을 선택하세요.\n");
        for (int i = 0; i < foundCount; i++) {
            printf(" %d. 학번: %s, 이름: %s, 국어: %d, 영어: %d, 수학: %d, 총점: %d, 평균: %d, 등급: %c\n",
                i + 1, students[foundIndex[i]].sid, students[foundIndex[i]].name, students[foundIndex[i]].score1, students[foundIndex[i]].score2, students[foundIndex[i]].score3, students[foundIndex[i]].total, students[foundIndex[i]].average, students[foundIndex[i]].grade);
        }
        int choice;
        printf(" 선택: ");
        scanf("%d", &choice);
        if (choice < 1 || choice > foundCount) {
            printf("\n 잘못된 선택입니다.\n");
            return;
        }
        foundIndex[0] = foundIndex[choice - 1];
        found = 1;
    }

    if (found) {
        int i = foundIndex[0];
        printf("\n 학생을 찾았습니다: \n");
        printf(" 학번: %s \n 이름: %s \n 국어: %d \n 영어: %d \n 수학: %d \n 총점: %d \n 평균: %d \n 등급: %c \n",
            students[i].sid, students[i].name, students[i].score1, students[i].score2, students[i].score3, students[i].total, students[i].average, students[i].grade);

        // 수정 또는 삭제 선택
        printf("\n 1. 수정  2. 삭제  0. 취소 >> ");
        N = getch();

        if (N == '1') {
            // 데이터 수정
            printf("\n 새로운 이름: "); scanf("%9s", students[i].name);
            printf(" 새로운 국어 점수: "); scanf("%d", &students[i].score1);
            printf(" 새로운 영어 점수: "); scanf("%d", &students[i].score2);
            printf(" 새로운 수학 점수: "); scanf("%d", &students[i].score3);
            CalculateTotalAverage(&students[i]);
            printf("\n 수정되었습니다.\n");
        }
        else if (N == '2') {
            // 데이터 삭제
            for (int j = i; j < CNT - 1; j++) {
                students[j] = students[j + 1];
            }
            CNT--;
            printf("\n 삭제되었습니다.\n");
        }
    }

    // 수정된 데이터를 파일에 저장
    if ((fp1 = fopen(fname1, "w")) == NULL) {
        printf("\n(!) \"%s\" 을 열 수 없습니다.\n", fname1);
        return;
    }
    for (int i = 0; i < CNT; i++) {
        fprintf(fp1, "%10s %5s %3d %3d %3d\n", students[i].sid, students[i].name, students[i].score1, students[i].score2, students[i].score3);
    }
    fclose(fp1);
    printf("\n 파일이 업데이트되었습니다.\n");
}

// 성적 집계표를 기본 순서대로 출력하는 함수
int SaveReport0() {
    strcpy(fname1, "c:/Temp/성적데이터.txt");  // 파일 경로

    // 파일 읽기 모드로 열기
    if ((fp1 = fopen(fname1, "r")) == NULL) {
        printf("(!) 입력파일 : \"%s\" 을 열 수 없습니다.\n", fname1);
        return 0;
    }
    else {
        printf("\n(!) 입력파일 : \"%s\" 을 열었습니다.\n", fname1);
    }

    strcpy(fname2, "c:/Temp/성적집계표-기본.txt");  // 출력 파일 경로

    // 파일 쓰기 모드로 열기
    if ((fp2 = fopen(fname2, "w")) == NULL) {
        printf("(!) 출력파일 : \"%s\" 을 만들 수 없습니다.\n", fname2);
        return 0;
    }
    else {
        printf("\n(!) 출력파일 : \"%s\" 을 만들었습니다.\n", fname2);
    }

    fprintf(fp2, "\n\t 성적집계표-기본");
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====");
    fprintf(fp2, "\n 학     번 이  름 국어 영어 수학 합계 평균 등급");
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====");

    CNT = 0;  // 데이터 개수 초기화
    while (!feof(fp1)) {
        fscanf(fp1, "%9s %9s %d %d %d", students[CNT].sid, students[CNT].name, &students[CNT].score1, &students[CNT].score2, &students[CNT].score3);  // 파일에서 데이터 읽기
        if (feof(fp1)) { break; }  // 파일 끝에 도달하면 종료
        CalculateTotalAverage(&students[CNT]);
        fprintf(fp2, "\n %s %s %4d %4d %4d %5d %4d   %c",
            students[CNT].sid, students[CNT].name, students[CNT].score1, students[CNT].score2, students[CNT].score3, students[CNT].total, students[CNT].average, students[CNT].grade);  // 데이터 파일에 쓰기
        CNT++;
    }
    printf("\n\t(!) 총 %d 건의 데이터를 처리하였습니다. \n", CNT);
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====\n ");

    fclose(fp2);    printf("\n(!) 출력파일을 닫았습니다.");  // 출력 파일 닫기
    fclose(fp1);    printf("\n(!) 입력파일을 닫았습니다.");  // 입력 파일 닫기
    system("notepad c:/Temp/성적집계표-기본.txt");  // 출력 파일 열기
    return 0;
}

// 성적집계표를 성적순으로 출력하는 함수
int SaveReport1() {
    strcpy(fname1, "c:/Temp/성적데이터.txt");  // 파일 경로

    // 파일 읽기 모드로 열기
    if ((fp1 = fopen(fname1, "r")) == NULL) {
        printf("(!) 입력파일 : \"%s\" 을 열 수 없습니다.\n", fname1);
        return 0;
    }
    else {
        printf("\n(!) 입력파일 : \"%s\" 을 열었습니다.\n", fname1);
    }

    CNT = 0;  // 데이터 개수 초기화
    while (!feof(fp1)) {
        fscanf(fp1, "%9s %9s %d %d %d", students[CNT].sid, students[CNT].name, &students[CNT].score1, &students[CNT].score2, &students[CNT].score3);  // 파일에서 데이터 읽기
        if (feof(fp1)) { break; }  // 파일 끝에 도달하면 종료
        CalculateTotalAverage(&students[CNT]);  // 총점과 평균 계산
        CNT++;
    }
    fclose(fp1);

    // 성적순으로 정렬
    qsort(students, CNT, sizeof(Student), CompareByTotal);

    strcpy(fname2, "c:/Temp/성적집계표-성적순.txt");  // 출력 파일 경로

    // 파일 쓰기 모드로 열기
    if ((fp2 = fopen(fname2, "w")) == NULL) {
        printf("(!) 출력파일 : \"%s\" 을 만들 수 없습니다.\n", fname2);
        return 0;
    }
    else {
        printf("\n(!) 출력파일 : \"%s\" 을 만들었습니다.\n", fname2);
    }

    fprintf(fp2, "\n\t 성적집계표-성적순");
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====");
    fprintf(fp2, "\n 학     번 이  름 국어 영어 수학 합계 평균 등급");
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====");

    for (int i = 0; i < CNT; i++) {
        fprintf(fp2, "\n %s %s %4d %4d %4d %5d %4d   %c",
            students[i].sid, students[i].name, students[i].score1, students[i].score2, students[i].score3, students[i].total, students[i].average, students[i].grade);  // 데이터 파일에 쓰기
    }

    printf("\n\t(!) 총 %d 건의 데이터를 처리하였습니다. \n", CNT);
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====\n ");

    fclose(fp2);    printf("\n(!) 출력파일을 닫았습니다.");  // 출력 파일 닫기
    system("notepad c:/Temp/성적집계표-성적순.txt");  // 출력 파일 열기
    return 0;
}

// 성적집계표를 이름순으로 출력하는 함수
int SaveReport2() {
    strcpy(fname1, "c:/Temp/성적데이터.txt");  // 파일 경로

    // 파일 읽기 모드로 열기
    if ((fp1 = fopen(fname1, "r")) == NULL) {
        printf("(!) 입력파일 : \"%s\" 을 열 수 없습니다.\n", fname1);
        return 0;
    }
    else {
        printf("\n(!) 입력파일 : \"%s\" 을 열었습니다.\n", fname1);
    }

    CNT = 0;  // 데이터 개수 초기화
    while (!feof(fp1)) {
        fscanf(fp1, "%9s %9s %d %d %d", students[CNT].sid, students[CNT].name, &students[CNT].score1, &students[CNT].score2, &students[CNT].score3);  // 파일에서 데이터 읽기
        if (feof(fp1)) { break; }  // 파일 끝에 도달하면 종료
        CalculateTotalAverage(&students[CNT]);  // 총점과 평균 계산
        CNT++;
    }
    fclose(fp1);

    // 이름순으로 정렬
    qsort(students, CNT, sizeof(Student), CompareByName);

    strcpy(fname2, "c:/Temp/성적집계표-이름순.txt");  // 출력 파일 경로

    // 파일 쓰기 모드로 열기
    if ((fp2 = fopen(fname2, "w")) == NULL) {
        printf("(!) 출력파일 : \"%s\" 을 만들 수 없습니다.\n", fname2);
        return 0;
    }
    else {
        printf("\n(!) 출력파일 : \"%s\" 을 만들었습니다.\n", fname2);
    }

    fprintf(fp2, "\n\t 성적집계표-이름순");
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====");
    fprintf(fp2, "\n 학     번 이  름 국어 영어 수학 합계 평균 등급");
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====");

    for (int i = 0; i < CNT; i++) {
        fprintf(fp2, "\n %s %s %4d %4d %4d %5d %4d   %c",
            students[i].sid, students[i].name, students[i].score1, students[i].score2, students[i].score3, students[i].total, students[i].average, students[i].grade);  // 데이터 파일에 쓰기
    }

    printf("\n\t(!) 총 %d 건의 데이터를 처리하였습니다. \n", CNT);
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====\n ");

    fclose(fp2);    printf("\n(!) 출력파일을 닫았습니다.");  // 출력 파일 닫기
    system("notepad c:/Temp/성적집계표-이름순.txt");  // 출력 파일 열기
    return 0;
}

// 성적집계표를 학번순으로 출력하는 함수
int SaveReport3() {
    strcpy(fname1, "c:/Temp/성적데이터.txt");  // 파일 경로

    // 파일 읽기 모드로 열기
    if ((fp1 = fopen(fname1, "r")) == NULL) {
        printf("(!) 입력파일 : \"%s\" 을 열 수 없습니다.\n", fname1);
        return 0;
    }
    else {
        printf("\n(!) 입력파일 : \"%s\" 을 열었습니다.\n", fname1);
    }

    CNT = 0;  // 데이터 개수 초기화
    while (!feof(fp1)) {
        fscanf(fp1, "%9s %9s %d %d %d", students[CNT].sid, students[CNT].name, &students[CNT].score1, &students[CNT].score2, &students[CNT].score3);  // 파일에서 데이터 읽기
        if (feof(fp1)) { break; }  // 파일 끝에 도달하면 종료
        CalculateTotalAverage(&students[CNT]);  // 총점과 평균 계산
        CNT++;
    }
    fclose(fp1);

    // 학번순으로 정렬
    qsort(students, CNT, sizeof(Student), CompareByID);

    strcpy(fname2, "c:/Temp/성적집계표-학번순.txt");  // 출력 파일 경로

    // 파일 쓰기 모드로 열기
    if ((fp2 = fopen(fname2, "w")) == NULL) {
        printf("(!) 출력파일 : \"%s\" 을 만들 수 없습니다.\n", fname2);
        return 0;
    }
    else {
        printf("\n(!) 출력파일 : \"%s\" 을 만들었습니다.\n", fname2);
    }

    fprintf(fp2, "\n\t 성적집계표-학번순");
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====");
    fprintf(fp2, "\n 학     번 이  름 국어 영어 수학 합계 평균 등급");
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====");

    for (int i = 0; i < CNT; i++) {
        fprintf(fp2, "\n %s %s %4d %4d %4d %5d %4d   %c",
            students[i].sid, students[i].name, students[i].score1, students[i].score2, students[i].score3, students[i].total, students[i].average, students[i].grade);  // 데이터 파일에 쓰기
    }

    printf("\n\t(!) 총 %d 건의 데이터를 처리하였습니다. \n", CNT);
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====\n ");

    fclose(fp2);    printf("\n(!) 출력파일을 닫았습니다.");  // 출력 파일 닫기
    system("notepad c:/Temp/성적집계표-학번순.txt");  // 출력 파일 열기
    return 0;
}

// 파일에 있는 성적 데이터를 Excel 파일로 출력하는 함수
int SaveCSV() {
    strcpy(fname1, "c:/Temp/성적데이터.txt");  // 파일 경로

    // 파일 읽기 모드로 열기
    if ((fp1 = fopen(fname1, "r")) == NULL) {
        printf("\n(!) 입력파일 : \"%s\" 을 열 수 없습니다.\n", fname1);
        return 0;
    }
    else {
        printf("\n(!) 입력파일 : \"%s\" 을 열었습니다.\n", fname1);
    }

    strcpy(fname2, "c:/Temp/성적집계표.csv");  // 출력 파일 경로

    // 파일 쓰기 모드로 열기
    if ((fp2 = fopen(fname2, "w")) == NULL) {
        printf("\n(!) 출력파일 : \"%s\" 을 만들 수 없습니다.\n", fname2);
        return 0;
    }
    else {
        printf("\n(!) 출력파일 : \"%s\" 을 만들었습니다.\n", fname2);
    }

    fprintf(fp2, "유수봉의 성적표 만들기(CSV) \n");
    fprintf(fp2, "==========,=====,=====,=====,=====,=====,=====,===== \n");
    fprintf(fp2, "학번,이름,국어,영어,수학,합계,평균,등급 \n");
    fprintf(fp2, "==========,=====,=====,=====,=====,=====,=====,===== \n");

    CNT = 0;  // 데이터 개수 초기화
    while (!feof(fp1)) {
        fscanf(fp1, "%9s %9s %d %d %d", students[CNT].sid, students[CNT].name, &students[CNT].score1, &students[CNT].score2, &students[CNT].score3);  // 파일에서 데이터 읽기
        if (feof(fp1)) { break; }

        CalculateTotalAverage(&students[CNT]);  // 총점과 평균 계산

        // CSV 파일에 데이터 쓰기
        fprintf(fp2, "%s, %s, %d, %d, %d, %d, %d, %c \n",
            students[CNT].sid, students[CNT].name, students[CNT].score1, students[CNT].score2, students[CNT].score3, students[CNT].total, students[CNT].average, students[CNT].grade);
        CNT++;
    }
    printf("\n\t(!) 총 %d 건의 데이터를 처리하였습니다. \n", CNT);
    fprintf(fp2, "==========,=====,=====,=====,=====,=====,=====,===== \n");

    fclose(fp1);    printf("\n(!) 입력파일을 닫았습니다.");  // 입력 파일 닫기
    fclose(fp2);    printf("\n(!) 출력파일을 닫았습니다.");  // 출력 파일 닫기
    system("start excel \"c:/Temp/성적집계표.csv\"");  // Excel로 CSV 파일 열기

    return 0;
}

// 파일에 있는 성적 데이터를 HTML 파일로 출력하는 함수
int SaveHTML() {
    strcpy(fname1, "c:/Temp/성적데이터.txt");  // 파일 경로

    // 파일 읽기 모드로 열기
    if ((fp1 = fopen(fname1, "r")) == NULL) {
        printf("(!) 입력파일 : \"%s\" 을 열 수 없습니다.\n", fname1);
        return 0;
    }
    else {
        printf("(!) 입력파일 : \"%s\" 을 열었습니다.\n", fname1);
    }

    strcpy(fname2, "c:/Temp/성적집계표.html");  // 출력 파일 경로

    // 파일 쓰기 모드로 열기
    if ((fp2 = fopen(fname2, "w")) == NULL) {
        printf("(!) 출력파일 : \"%s\" 을 만들 수 없습니다.\n", fname2);
        return 0;
    }
    else {
        printf("(!) 출력파일 : \"%s\" 을 만들었습니다.\n", fname2);
    }

    // HTML 파일 작성 시작
    fprintf(fp2, "<!DOCTYPE html><HTML lang=\"ko\"><HEAD> \n"); // lang="ko" 추가
    fprintf(fp2, "<meta charset=\"euc-kr\"> \n");  // euc-kr 인코딩 설정 추가
    fprintf(fp2, "<TITLE>유수봉의 HTML 성적표</TITLE> \n");
    fprintf(fp2, "<STYLE> \n");
    fprintf(fp2, "table {border-collapse: collapse; text-align: center; width: 500px; } \n");
    fprintf(fp2, "table tr:hover {background-color: lightgreen; height:40px;} \n");
    fprintf(fp2, "</STYLE></HEAD><BODY> \n");
    fprintf(fp2, "<DIV align=center><H2>유수봉의 성적표 만들기(HTML)</H2></DIV> \n");
    fprintf(fp2, "<TABLE border=1 align=center> \n");
    fprintf(fp2, "<THEAD> \n");
    fprintf(fp2, "<TR><TH>학번</TH><TH>이름</TH><TH>국어</TH><TH>영어</TH><TH>수학</TH><TH>합계</TH><TH>평균</TH><TH>등급</TH></TR> \n");
    fprintf(fp2, "</THEAD><TBODY> \n");

    CNT = 0;  // 데이터 개수 초기화
    while (!feof(fp1)) {
        fscanf(fp1, "%9s %9s %d %d %d", students[CNT].sid, students[CNT].name, &students[CNT].score1, &students[CNT].score2, &students[CNT].score3);  // 파일에서 데이터 읽기
        if (feof(fp1)) { break; }
        CalculateTotalAverage(&students[CNT]);  // 총점과 평균 계산

        // HTML 테이블에 데이터 쓰기
        fprintf(fp2, "<TR><TD>%s</TD><TD>%s</TD><TD>%d</TD><TD>%d</TD><TD>%d</TD><TD>%d</TD><TD>%d</TD><TD>%c</TD></TR> \n",
            students[CNT].sid, students[CNT].name, students[CNT].score1, students[CNT].score2, students[CNT].score3, students[CNT].total, students[CNT].average, students[CNT].grade);
        CNT++;
    }
    printf("\t(!) 총 %d 건의 데이터를 처리하였습니다. \n", CNT);
    fprintf(fp2, "</TBODY> \n");
    fprintf(fp2, "</TABLE> \n");
    fprintf(fp2, "</BODY> \n </HTML> \n");

    fclose(fp1);    printf("(!) 입력파일을 닫았습니다.\n");  // 입력 파일 닫기
    fclose(fp2);    printf("(!) 출력파일을 닫았습니다.\n");  // 출력 파일 닫기
    system("start \"\" \"c:/Temp/성적집계표.html\"");  // 기본 브라우저로 HTML 파일 열기
    return 0;
}
