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
}Rain;

typedef struct // 플레이어 구조체
{
	int x;
}Player;

Rain rain[WIDTH];
Player player;

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
	int i;
	for (i = 0; i < WIDTH; i++)
	{
		rain[i].active = 0;
		player.x = WIDTH / 2;
	}
}

void PrintMap() // 맵 출력 함수
{
	system("cls"); // 화면 클리어

	int i;
	for (i = 0; i < WIDTH; i++)
	{
		if (rain[i].active) // 비가 활성화 됐다면
		{
			gotoxy(rain[i].x, HEIGHT - rain[i].y);
			printf("l"); // 비
		}
	}
	gotoxy(player.x, HEIGHT);
	printf("♥"); // 플레이어

	gotoxy(0, HEIGHT + 1);
	for (i = 0; i < WIDTH; i++)
		printf("■"); // 바닥
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
	int i;

	for (i = 0; i < WIDTH; i++)
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
	int i;
	for (i = 0; i < WIDTH; i++)
	{
		if (rain[i].active) // 비가 활성화 되면
		{
			rain[i].y--; // 비의 y 감소
		}
	}
}

void DeleteRain() // 비 제거 함수
{
	int i;
	for (i = 0; i < WIDTH; i++)
	{
		if (rain[i].active && rain[i].y < 0) // 비가 활성화 되고 비y가 0보다 작으면
		{
			rain[i].active = 0; // 비 비활성화
		}
	}
}

int PlayerTouchRain() // 플레이어가 비에 닿으면
{
	int i;
	for (i = 0; i < WIDTH; i++)
	{
		if (rain[i].active && (rain[i].y == 0 && rain[i].x == player.x)) // 비가 활성화 되고, 비y가 0이랑 같고, 비x가 플레이어x랑 같으면
		{
			printf("\n           ----- 탈락 -----\n");
			return 1;
		}
	}
	return 0; // 탈락
}

void main()
{
	char key; // 입력 키 변수

	Reset(); // 초기화 함수

	do // 반복문
	{
		srand((int)malloc(NULL));

		PrintMap(); // 맵 출력 함수
		MovePlayer(); // 플레이어 움직이는 함수
		CreateRain(); // 비 생성 함수
		MoveRain(); // 비 움직이는 함수
		DeleteRain(); // 비 제거 함수
		Sleep(60); // 속도 제한
	} while (!(PlayerTouchRain())); // 비가 닿이지 않으면 반복 (NOT)
}