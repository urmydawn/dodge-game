#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define WIDTH 20
#define HEIGHT 10

typedef struct // �� ����ü
{
    int x;
    int y;
    int active;
} Rain;

typedef struct // �÷��̾� ����ü
{
    int x;
} Player;

Rain rain[WIDTH];
Player player;
int score = 0; // ���� ���� �ʱ�ȭ

void gotoxy(int x, int y) // Ŀ�� �̵� �Լ�
{
    COORD pos = { 2 * x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int PressKey(int key) // Ű�� ����������
{
    return ((GetAsyncKeyState(key) & 0x8000) != 0);
}

void Reset() // �ʱ�ȭ �Լ�
{
    for (int i = 0; i < WIDTH; i++)
    {
        rain[i].active = 0;
    }
    player.x = WIDTH / 2;
    score = 0; // ���� �ʱ�ȭ
}

void PrintMap() // �� ��� �Լ�
{
    system("cls"); // ȭ�� Ŭ����

    // ���� ���
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // ���� �� ����
    gotoxy(0, HEIGHT + 2);
    printf("����: %d\n", score); // ���� ���� ���


    for (int i = 0; i < WIDTH; i++)
    {
        if (rain[i].active) // �� Ȱ��ȭ �ƴٸ�
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            gotoxy(rain[i].x, HEIGHT - rain[i].y);
            printf("l"); // ��
        }
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    gotoxy(player.x, HEIGHT);
    gotoxy(player.x, HEIGHT);
    printf("��"); // �÷��̾�

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    gotoxy(0, HEIGHT + 1);
    for (int i = 0; i < WIDTH; i++)
        printf("��"); // �ٴ�

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // ���
}

void MovePlayer() // �÷��̾� �����̴� �Լ�
{
    if (PressKey(VK_LEFT)) // ���� ����Ű�� ��������
        player.x -= 1; // �������� ��ĭ
    if (PressKey(VK_RIGHT)) // ������ ����Ű�� ��������
        player.x += 1; // ���������� ��ĭ

    if (player.x < 0) // �÷��̾�x�� 0���� ������
        player.x = 0; // 0���� ����
    if (player.x > WIDTH - 1) // �÷��̾�x�� WIDTH(20) - 1 = 19��
        player.x = WIDTH - 1; // 19�� ����
}

void CreateRain() // �� ���� �Լ�
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (!rain[i].active) // �� Ȱ��ȭ ���� ������ (NOT)
        {
            rain[i].x = rand() % WIDTH;
            rain[i].y = HEIGHT - 1;

            rain[i].active = 1; // �� Ȱ��ȭ

            return;
        }
    }
}

void MoveRain() // �� �����̴� �Լ�
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (rain[i].active) // �� Ȱ��ȭ �Ǹ�
        {
            rain[i].y--; // ���� y ����
        }
    }
}

void DeleteRain() // �� ���� �Լ�
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (rain[i].active && rain[i].y < 0) // �� Ȱ��ȭ �ǰ� ��y�� 0���� ������
        {
            rain[i].active = 0; // �� ��Ȱ��ȭ
        }
    }
}

int PlayerTouchRain() // �÷��̾ �� ������
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (rain[i].active && (rain[i].y == 0 && rain[i].x == player.x)) // �� Ȱ��ȭ �ǰ�, ��y�� 0�̶� ����, ��x�� �÷��̾�x�� ������
        {
            gotoxy(0, HEIGHT + 2);
            printf("\n           ----- Ż�� -----\n");
            printf("    SPACE�ٸ� ������ �����ϼ���...");
            return 1; // Ż��
        }
    }
    return 0; // Ż�� ����
}

void main()
{
    Reset(); // �ʱ�ȭ �Լ�

    do // �ݺ���
    {
        system("mode con: cols=40 lines=15");

        srand((int)malloc(NULL));

        PrintMap(); // �� ��� �Լ�
        MovePlayer(); // �÷��̾� �����̴� �Լ�
        CreateRain(); // �� ���� �Լ�
        MoveRain(); // �� �����̴� �Լ�
        DeleteRain(); // �� ���� �Լ� 

        score++; //���� ����
        Sleep(15);
    } while (!(PlayerTouchRain())); // �� ������ ������ �ݺ�

    while (!PressKey(VK_SPACE)); // �����̽��ٰ� ������ ����
}