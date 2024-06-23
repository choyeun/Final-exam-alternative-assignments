#pragma warning(disable:4996)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <direct.h>
#include <io.h>

void gotoxy(int x, int y) {
    COORD Pos = { x - 1, y - 1 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

int InputData();
int ViewData();
int EditData();
int SaveReport0();
int SaveReport1();
int SaveReport2();
int SaveCSV();
int SaveHTML();
void ClearTempFolder();

FILE* fp1, * fp2;
char fname1[50], fname2[50];
char sid[10], name[10];
int N, CNT = 0, score1, score2, score3, total, average;

int main(void) {
    while (1) {
        gotoxy(2, 2);
        printf("\n\t 유수봉의 성적처리 시스템 ");
        printf("\n\t ========================= ");
        printf("\n\t << 작업 목록 >>-------------------------------- ");
        printf("\n\t   1. 새로운 성적 데이터 입력받아 파일에 저장하기 ");
        printf("\n\t   2. 파일에 저장된 성적 데이터 화면에 표시하기 ");
        printf("\n\t   3. 파일에 저장된 성적 데이터를 이용한 성적 집계표 파일 만들기 ");
        printf("\n\t   4. 성적집계표-1 출력 ");
        printf("\n\t   5. 성적집계표-2 출력 ");
        printf("\n\t   6. 개인별 성적 데이터 관리 ");
        printf("\n\t   8. 성적데이터를 Excel File(CSV) 로 출력 ");
        printf("\n\t   9. 성적데이터를 Web Page(HTML) 로 출력 ");
        printf("\n\t   0. 작업끝 ");
        printf("\n\t ------------------------<< 번호를 선택하시오 >>");
        N = getch();

        switch (N) {
        case  '0':  return 0;
        case  '1':  ClearTempFolder(); InputData(); break;
        case  '2':  ViewData();  break;
        case  '3':  SaveReport0();   break;
        case  '4':  SaveReport1();   break;
        case  '5':  SaveReport2();   break;
        case  '6':  EditData();  break;
        case  '8':  SaveCSV();   break;
        case  '9':  SaveHTML();  break;
        }
        printf("\n\t -------------------------------------<< 확인 >>");
        N = getch();
        system("cls");
    }
}

// temp 폴더의 내용물을 삭제
void ClearTempFolder() {
    char path[50];
    sprintf(path, "c:/Temp/*");

    struct _finddata_t fileinfo;
    intptr_t handle = _findfirst(path, &fileinfo);

    if (handle != -1) {
        do {
            if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
                char filepath[100];
                sprintf(filepath, "c:/Temp/%s", fileinfo.name);
                remove(filepath);
            }
        } while (_findnext(handle, &fileinfo) == 0);
        _findclose(handle);
    }
}

// 성적 데이터 입력 처리
int InputData() {
    printf("\n\t 본 작업을 수행하면 기존 데이터를 모두 삭제됩니다.");
    printf("\n\t 그래도 작업을 계속 하시겠습니까? (Y/N) ->");
    N = getch();
    if ((N == 'Y') || (N == 'y')) {
        strcpy(fname1, "c:/Temp/성적데이터.txt");

        if ((fp1 = fopen(fname1, "w")) == NULL) {
            printf("\n(!) \"%s\" 을 열 수 없습니다.\n", fname1);
            return 0;
        }
        else {
            printf("\n(!) 새로운 성적 파일 \"%s\"을 만들었습니다.\n", fname1);
        }

        CNT = 0;
        while (1) {
            system("cls");
            gotoxy(2, 5);
            printf("-. 학번 :                  ('.' 입력하면 입력을 종료합니다.) : ");
            while (1) {
                gotoxy(12, 5);
                scanf("%9s", sid);
                if (strlen(sid) > 0) break;
            }
            if (strcmp(sid, ".") == 0) { break; }

            gotoxy(2, 6);    printf("-. 이름 : ");   scanf("%9s", name);
            gotoxy(2, 8);    printf("-. 국어 : ");   scanf("%d", &score1);
            gotoxy(2, 9);    printf("-. 영어 : ");   scanf("%d", &score2);
            gotoxy(2, 10);   printf("-. 수학 : ");   scanf("%d", &score3);

            fprintf(fp1, "%10s %5s %3d %3d %3d\n", sid, name, score1, score2, score3);
            CNT++;
            fflush(stdin);
            printf("\n(!) 총 %d 건의 데이터를 저장하였습니다.(',' 입력끝)", CNT);
            N = getch();
            if (N == '.') break;
        }
        printf("\n(!) 총 %d 건의 데이터를 저장하였습니다.", CNT);
        fclose(fp1);
        printf("\n(!) 파일을 닫았습니다.");
    }
    return 0;
}

// 파일에 있는 성적 데이터를 화면으로 출력 처리
int ViewData() {
    strcpy(fname1, "c:/Temp/성적데이터.txt");

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

    while (!feof(fp1)) {
        fscanf(fp1, "%9s %9s %d %d %d", sid, name, &score1, &score2, &score3);
        if (feof(fp1)) { break; }
        printf("\n\t %s %s %4d %4d %4d", sid, name, score1, score2, score3);
    }

    printf("\n\t ========= ====== ==== ==== ==== ");
    fclose(fp1);
    printf("\n(!) 파일을 닫았습니다.");

    return 0;
}

char Grade(int X) {
    char grade;
    if (X >= 90) grade = 'A';
    else if (X >= 80) grade = 'B';
    else if (X >= 70) grade = 'C';
    else if (X >= 60) grade = 'D';
    else grade = 'F';
    return grade;
}

int EditData() {
    return 0;
}

int SaveReport1() {
    return 0;
}

int SaveReport2() {
    return 0;
}

// 파일에 있는 성적 데이터를 합계 평균 처리 후 파일로 출력
int SaveReport0() {
    strcpy(fname1, "c:/Temp/성적데이터.txt");

    if ((fp1 = fopen(fname1, "r")) == NULL) {
        printf("(!) 입력파일 : \"%s\" 을 열 수 없습니다.\n", fname1);
        return 0;
    }
    else {
        printf("\n(!) 입력파일 : \"%s\" 을 열었습니다.\n", fname1);
    }

    strcpy(fname2, "c:/Temp/성적집계표-1.txt");

    if ((fp2 = fopen(fname2, "w")) == NULL) {
        printf("(!) 출력파일 : \"%s\" 을 만들 수 없습니다.\n", fname2);
        return 0;
    }
    else {
        printf("\n(!) 출력파일 : \"%s\" 을 만들었습니다.\n", fname2);
    }

    fprintf(fp2, "\n\t 성적집계표-1");
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====");
    fprintf(fp2, "\n 학     번 이  름 국어 영어 수학 합계 평균 등급");
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====");

    CNT = 0;
    while (!feof(fp1)) {
        fscanf(fp1, "%9s %9s %d %d %d", sid, name, &score1, &score2, &score3);
        if (feof(fp1)) { break; }
        total = score1 + score2 + score3;   average = total / 3;
        fprintf(fp2, "\n %s %s %4d %4d %4d %5d %4d   %c",
            sid, name, score1, score2, score3, total, average, Grade(average));
        CNT++;
    }
    printf("\n\t(!) 총 %d 건의 데이터를 처리하였습니다. \n", CNT);
    fprintf(fp2, "\n ========= ====== ==== ==== ==== ==== ==== ====\n ");

    fclose(fp2);    printf("\n(!) 출력파일을 닫았습니다.");
    fclose(fp1);    printf("\n(!) 입력파일을 닫았습니다.");
    system("notepad c:/Temp/성적집계표-1.txt");
    return 0;
}

// 파일에 있는 성적 데이터를 Excel 파일로 출력
int SaveCSV() {
    strcpy(fname1, "c:/Temp/성적데이터.txt");

    if ((fp1 = fopen(fname1, "r")) == NULL) {
        printf("\n(!) 입력파일 : \"%s\" 을 열 수 없습니다.\n", fname1);
        return 0;
    }
    else {
        printf("\n(!) 입력파일 : \"%s\" 을 열었습니다.\n", fname1);
    }

    strcpy(fname2, "c:/Temp/성적집계표.csv");

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

    CNT = 0;
    while (!feof(fp1)) {
        fscanf(fp1, "%9s %9s %d %d %d", sid, name, &score1, &score2, &score3);
        if (feof(fp1)) { break; }

        total = score1 + score2 + score3;
        average = total / 3;

        fprintf(fp2, "%s, %s, %d, %d, %d, %d, %d, %c \n",
            sid, name, score1, score2, score3, total, average, Grade(average));
        CNT++;
    }
    printf("\n\t(!) 총 %d 건의 데이터를 처리하였습니다. \n", CNT);
    fprintf(fp2, "==========,=====,=====,=====,=====,=====,=====,===== \n");

    fclose(fp1);    printf("\n(!) 입력파일을 닫았습니다.");
    fclose(fp2);    printf("\n(!) 출력파일을 닫았습니다.");
    system("start excel \"c:/Temp/성적집계표.csv\"");  // Excel로 CSV 파일 열기

    return 0;
}

// 파일에 있는 성적 데이터를 HTML 파일로 출력
int SaveHTML() {
    strcpy(fname1, "c:/Temp/성적데이터.txt");

    if ((fp1 = fopen(fname1, "r")) == NULL) {
        printf("(!) 입력파일 : \"%s\" 을 열 수 없습니다.\n", fname1);
        return 0;
    }
    else {
        printf("(!) 입력파일 : \"%s\" 을 열었습니다.\n", fname1);
    }

    strcpy(fname2, "c:/Temp/성적집계표.html");

    if ((fp2 = fopen(fname2, "w")) == NULL) {
        printf("(!) 출력파일 : \"%s\" 을 만들 수 없습니다.\n", fname2);
        return 0;
    }
    else {
        printf("(!) 출력파일 : \"%s\" 을 만들었습니다.\n", fname2);
    }

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

    CNT = 0;
    while (!feof(fp1)) {
        fscanf(fp1, "%9s %9s %d %d %d", sid, name, &score1, &score2, &score3);
        if (feof(fp1)) { break; }
        total = score1 + score2 + score3;     average = total / 3;

        fprintf(fp2, "<TR><TD>%s</TD><TD>%s</TD><TD>%d</TD><TD>%d</TD><TD>%d</TD><TD>%d</TD><TD>%d</TD><TD>%c</TD></TR> \n",
            sid, name, score1, score2, score3, total, average, Grade(total));
        CNT++;
    }
    printf("\t(!) 총 %d 건의 데이터를 처리하였습니다. \n", CNT);
    fprintf(fp2, "</TBODY> \n");
    fprintf(fp2, "</TABLE> \n");
    fprintf(fp2, "</BODY> \n </HTML> \n");

    fclose(fp1);    printf("(!) 입력파일을 닫았습니다.\n");
    fclose(fp2);    printf("(!) 출력파일을 닫았습니다.\n");
    system("start \"\" \"c:/Temp/성적집계표.html\"");  // 기본 브라우저로 HTML 파일 열기
    return 0;
}
