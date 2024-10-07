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
}Rain;

typedef struct // �÷��̾� ����ü
{
	int x;
}Player;

Rain rain[WIDTH];
Player player;

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
	int i;
	for (i = 0; i < WIDTH; i++)
	{
		rain[i].active = 0;
		player.x = WIDTH / 2;
	}
}

void PrintMap() // �� ��� �Լ�
{
	system("cls"); // ȭ�� Ŭ����

	int i;
	for (i = 0; i < WIDTH; i++)
	{
		if (rain[i].active) // �� Ȱ��ȭ �ƴٸ�
		{
			gotoxy(rain[i].x, HEIGHT - rain[i].y);
			printf("l"); // ��
		}
	}
	gotoxy(player.x, HEIGHT);
	printf("��"); // �÷��̾�

	gotoxy(0, HEIGHT + 1);
	for (i = 0; i < WIDTH; i++)
		printf("��"); // �ٴ�
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
	int i;

	for (i = 0; i < WIDTH; i++)
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
	int i;
	for (i = 0; i < WIDTH; i++)
	{
		if (rain[i].active) // �� Ȱ��ȭ �Ǹ�
		{
			rain[i].y--; // ���� y ����
		}
	}
}

void DeleteRain() // �� ���� �Լ�
{
	int i;
	for (i = 0; i < WIDTH; i++)
	{
		if (rain[i].active && rain[i].y < 0) // �� Ȱ��ȭ �ǰ� ��y�� 0���� ������
		{
			rain[i].active = 0; // �� ��Ȱ��ȭ
		}
	}
}

int PlayerTouchRain() // �÷��̾ �� ������
{
	int i;
	for (i = 0; i < WIDTH; i++)
	{
		if (rain[i].active && (rain[i].y == 0 && rain[i].x == player.x)) // �� Ȱ��ȭ �ǰ�, ��y�� 0�̶� ����, ��x�� �÷��̾�x�� ������
		{
			printf("\n           ----- Ż�� -----\n");
			return 1;
		}
	}
	return 0; // Ż��
}

void main()
{
	char key; // �Է� Ű ����

	Reset(); // �ʱ�ȭ �Լ�

	do // �ݺ���
	{
		srand((int)malloc(NULL));

		PrintMap(); // �� ��� �Լ�
		MovePlayer(); // �÷��̾� �����̴� �Լ�
		CreateRain(); // �� ���� �Լ�
		MoveRain(); // �� �����̴� �Լ�
		DeleteRain(); // �� ���� �Լ�
		Sleep(60); // �ӵ� ����
	} while (!(PlayerTouchRain())); // �� ������ ������ �ݺ� (NOT)
}