#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>

#define U 1
#define D 2
#define L 3
#define R 4			//�ߵ�״̬��U���� ��D���£�L : �� R����
#define WID 25		//��ͼ��С
#define HIG 25		//��ͼ��С

typedef struct SNAKE //�����һ���ڵ�
{
	int x, y;
	struct SNAKE *next;
}snake;

//ȫ�ֱ���//
int score = 0, add = 10;//�ܵ÷���ÿ�γ�ʳ��÷֡�
int status, sleeptime = 310;//ÿ�����е�ʱ����
snake *head, *food;//��ͷָ�룬ʳ��ָ��
snake *q;//�������ж��ߵ�ʱ���õ���ָ��
int endgamestatus = 0; //��Ϸ�����������1��ײ��ǽ��2��ҧ���Լ���3�������˳���Ϸ��

//����ȫ������//
void setPos(int x, int y);//���ù��λ��
void creatMap();//������ͼ
void initSnake();//��ʼ������
int isBiteSelf(snake *s);//�ж��Ƿ�ҧ�����Լ�
void createFood();//�������ʳ��
void cannotCrossWall();//���ܴ�ǽ
void snakemove();//��ǰ��,��U,��D,��L,��R
void pause();//��ͣ
void gameCircle();//������Ϸ
void welcomeToGame();//��ʼ����
void endGame();//������Ϸ
void gameStart();//��Ϸ��ʼ��
int main()
{
	gameStart();
	gameCircle();
	endGame();
	return 0;
}
void setPos(int x, int y)//���ù��λ��
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

void creatMap()//������ͼ
{
	int i;
	for (i = 0; i <= WID; i++)//��ӡ���±߿�
	{
		setPos(2 * i, 0);
		printf("��");
		setPos(2 * i, HIG);
		printf("��");
	}
	for (i = 1; i < HIG; i++)//��ӡ���ұ߿�
	{
		setPos(0, i);
		printf("��");
		setPos(2 * WID, i);
		printf("��");
	}
}

void initSnake()//��ʼ������
{
	snake *tail;
	int i;
	head = (snake*)malloc(sizeof(snake));//����ͷ��ʼ��β�巨����x,y�趨��ʼ��λ��
	head->x = 12 * 2;
	head->y = 12;
	head->next = NULL;
	q = head;
	for (i = 1; i <= 1; i++)
	{
		tail = (snake*)malloc(sizeof(snake));
		tail->x = 2 * 12 + 2 * i;
		tail->y = 12;
		q->next = tail;
		q = q->next;
	}
	tail->next = NULL;
	q = head;
	while (q != NULL)//��ͷ��Ϊ���������
	{
		setPos(q->x, q->y);
		printf("��");
		q = q->next;
	}
}

int isBiteSelf(snake *s)//�ж��Ƿ�ҧ�����Լ�
{
	q = head;
	while (q != NULL)
	{
		if (q->x == s->x && q->y == s->y)
		{
			return 1;
		}
		q = q->next;
	}
	return 0;
}

void createFood()//�������ʳ��
{
	snake *food_1;
	srand((unsigned)time(NULL));
	food_1 = (snake*)malloc(sizeof(snake));
	food_1->x = 2 * (rand() % (WID - 1) + 1);
	food_1->y = rand() % (HIG - 1) + 1;
	q = head;
	while (q->next != NULL)
	{
		if (q->x == food_1->x && q->y == food_1->y) //�ж������Ƿ���ʳ���غ�
		{
			food_1->x = 2 * (rand() % (WID - 1) + 1);
			food_1->y = rand() % (HIG - 1) + 1;
			q = head;
		}
		else
			q = q->next;
	}
	setPos(food_1->x, food_1->y);
	food = food_1;
	printf("��");
}

void cannotCrossWall()//���ܴ�ǽ
{
	if (q->x == 0 || q->x == WID * 2 || q->y == 0 || q->y == HIG)
	{
		endgamestatus = 1;
		endGame();
	}
}

void snakemove()//��ǰ��,��U,��D,��L,��R
{
	snake * nexthead;
	nexthead = (snake*)malloc(sizeof(snake));
	if (status == U)
	{
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
	}
	else if (status == D)
	{
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
	}
	else if (status == L)
	{
		nexthead->x = head->x - 2;
		nexthead->y = head->y;
	}
	else if (status == R)
	{
		nexthead->x = head->x + 2;
		nexthead->y = head->y;
	}
	q = nexthead;
	cannotCrossWall();		//�ж��Ƿ�ײǽ
	if (isBiteSelf(nexthead) == 1)       //�ж��Ƿ��ҧ���Լ�
	{
		endgamestatus = 2;
		endGame();
	}
	if (nexthead->x == food->x && nexthead->y == food->y)//�����һ����ʳ��//
	{
		nexthead->next = head;
		head = nexthead;
		q = head;
		while (q != NULL)
		{
			setPos(q->x, q->y);
			printf("��");
			q = q->next;
		}
		score = score + add;
		createFood();
		if (sleeptime > 20)
		{
			sleeptime -= 20;
			add += 5;
		}
	}
	else                                               //���û��ʳ��//
	{
		nexthead->next = head;
		head = nexthead;
		q = head;
		while (q->next->next != NULL)
		{
			setPos(q->x, q->y);
			printf("��");
			q = q->next;
		}
		setPos(q->next->x, q->next->y);
		printf("  ");
		free(q->next);
		q->next = NULL;
	}
}

void pause()//��ͣ
{
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE))
		{
			break;
		}
	}
}

void gameCircle()//������Ϸ        
{

	setPos(64, 14);
	printf("���ܴ�ǽ������ҧ���Լ�\n");
	setPos(64, 15);
	printf("�á�.��.��.���ֱ�����ߵ��ƶ�.");
	setPos(64, 16);
	printf("ESC ���˳���Ϸ��space����ͣ��Ϸ.");
	status = L;
	while (1)
	{
		setPos(64, 10);
		printf("�÷֣�%d  ", score);
		setPos(64, 11);
		printf("ÿ��ʳ��÷֣�%d��", add);
		if (GetAsyncKeyState(VK_UP) && status != D)
		{
			status = U;
		}
		else if (GetAsyncKeyState(VK_DOWN) && status != U)
		{
			status = D;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != R)
		{
			status = L;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && status != L)
		{
			status = R;
		}
		else if (GetAsyncKeyState(VK_SPACE))
		{
			pause();
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
		{
			endgamestatus = 3;
			break;
		}
		Sleep(sleeptime);
		snakemove();
	}
}

void welcomeToGame()//��ʼ����
{
	setPos(40, 12);

	system("title C���Գ�����Ϸ̰֮����");
	printf("��ӭ����̰ʳ����Ϸ��");
	setPos(40, 25);
	printf("              ���ߣ�Ѧ��\n");
	system("pause");
	system("cls");
	setPos(25, 12);
	printf("�á�.��.��.���ֱ�����ߵ��ƶ�\n");
	setPos(25, 13);
	printf("����Խ�����ٶȾ�Խ�졣\n");
	system("pause");
	system("cls");
}

void endGame()//������Ϸ
{
	system("cls");
	setPos(24, 12);
	if (endgamestatus == 1)
	{
		printf("�Բ�����ײ��ǽ�ˡ���Ϸ����.");
	}
	else if (endgamestatus == 2)
	{
		printf("�Բ�����ҧ���Լ��ˡ���Ϸ����.");
	}
	else if (endgamestatus == 3)
	{
		printf("�����Ѿ���������Ϸ��");
	}
	setPos(24, 13);
	printf("���ĵ÷���%d\n", score);
	system("pause");
	exit(0);
}

void gameStart()//��Ϸ��ʼ��
{
	system("mode con cols=100 lines=30");//����̨���100���߶�30
	welcomeToGame();
	creatMap();
	initSnake();
	createFood();
}