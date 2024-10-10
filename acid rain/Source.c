#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define WIDTH 20
#define HEIGHT 10

typedef struct // 비 구조체
{
    int x;
    int y;
    int active;
} Rain;

typedef struct // 플레이어 구조체
{
    int x;
} Player;

Rain rain[WIDTH];
Player player;
int score = 0; // 점수 변수 초기화

void gotoxy(int x, int y) // 커서 이동 함수
{
    COORD pos = { 2 * x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int PressKey(int key) // 키가 눌려졌는지
{
    return ((GetAsyncKeyState(key) & 0x8000) != 0);
}

void Reset() // 초기화 함수
{
    for (int i = 0; i < WIDTH; i++)
    {
        rain[i].active = 0;
    }
    player.x = WIDTH / 2;
    score = 0; // 점수 초기화
}

void PrintMap() // 맵 출력 함수
{
    system("cls"); // 화면 클리어

    // 점수 출력
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 글자 색 설정
    gotoxy(0, HEIGHT + 2);
    printf("점수: %d\n", score); // 현재 점수 출력


    for (int i = 0; i < WIDTH; i++)
    {
        if (rain[i].active) // 비가 활성화 됐다면
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            gotoxy(rain[i].x, HEIGHT - rain[i].y);
            printf("l"); // 비
        }
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    gotoxy(player.x, HEIGHT);
    gotoxy(player.x, HEIGHT);
    printf("♥"); // 플레이어

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    gotoxy(0, HEIGHT + 1);
    for (int i = 0; i < WIDTH; i++)
        printf("■"); // 바닥

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // 흰색
}

void MovePlayer() // 플레이어 움직이는 함수
{
    if (PressKey(VK_LEFT)) // 왼쪽 방향키가 눌러지면
        player.x -= 1; // 왼쪽으로 한칸
    if (PressKey(VK_RIGHT)) // 오른쪽 방향키가 눌러지면
        player.x += 1; // 오른쪽으로 한칸

    if (player.x < 0) // 플레이어x가 0보다 작으면
        player.x = 0; // 0으로 설정
    if (player.x > WIDTH - 1) // 플레이어x가 WIDTH(20) - 1 = 19면
        player.x = WIDTH - 1; // 19로 설정
}

void CreateRain() // 비 생성 함수
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (!rain[i].active) // 비가 활성화 되지 않으면 (NOT)
        {
            rain[i].x = rand() % WIDTH;
            rain[i].y = HEIGHT - 1;

            rain[i].active = 1; // 비를 활성화

            return;
        }
    }
}

void MoveRain() // 비 움직이는 함수
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (rain[i].active) // 비가 활성화 되면
        {
            rain[i].y--; // 비의 y 감소
        }
    }
}

void DeleteRain() // 비 제거 함수
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (rain[i].active && rain[i].y < 0) // 비가 활성화 되고 비y가 0보다 작으면
        {
            rain[i].active = 0; // 비 비활성화
        }
    }
}

int PlayerTouchRain() // 플레이어가 비에 닿으면
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (rain[i].active && (rain[i].y == 0 && rain[i].x == player.x)) // 비가 활성화 되고, 비y가 0이랑 같고, 비x가 플레이어x랑 같으면
        {
            gotoxy(0, HEIGHT + 2);
            printf("\n           ----- 탈락 -----\n");
            printf("    SPACE바를 눌러서 종료하세요...");
            return 1; // 탈락
        }
    }
    return 0; // 탈락 안함
}

void main()
{
    Reset(); // 초기화 함수

    do // 반복문
    {
        system("mode con: cols=40 lines=15");

        srand((int)malloc(NULL));

        PrintMap(); // 맵 출력 함수
        MovePlayer(); // 플레이어 움직이는 함수
        CreateRain(); // 비 생성 함수
        MoveRain(); // 비 움직이는 함수
        DeleteRain(); // 비 제거 함수 

        score++; //점수 증가
        Sleep(15);
    } while (!(PlayerTouchRain())); // 비가 닿이지 않으면 반복

    while (!PressKey(VK_SPACE)); // 스페이스바가 눌리면 종료
}