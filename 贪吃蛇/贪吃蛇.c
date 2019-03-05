#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>

#define U 1
#define D 2
#define L 3
#define R 4			//蛇的状态，U：上 ；D：下；L : 左 R：右
#define WID 25		//地图大小
#define HIG 25		//地图大小

typedef struct SNAKE //蛇身的一个节点
{
	int x, y;
	struct SNAKE *next;
}snake;

//全局变量//
int score = 0, add = 10;//总得分与每次吃食物得分。
int status, sleeptime = 310;//每次运行的时间间隔
snake *head, *food;//蛇头指针，食物指针
snake *q;//遍历和判断蛇的时候用到的指针
int endgamestatus = 0; //游戏结束的情况，1：撞到墙；2：咬到自己；3：主动退出游戏。

//声明全部函数//
void setPos(int x, int y);//设置光标位置
void creatMap();//创建地图
void initSnake();//初始化蛇身
int isBiteSelf(snake *s);//判断是否咬到了自己
void createFood();//随机出现食物
void cannotCrossWall();//不能穿墙
void snakemove();//蛇前进,上U,下D,左L,右R
void pause();//暂停
void gameCircle();//控制游戏
void welcomeToGame();//开始界面
void endGame();//结束游戏
void gameStart();//游戏初始化
int main()
{
	gameStart();
	gameCircle();
	endGame();
	return 0;
}
void setPos(int x, int y)//设置光标位置
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

void creatMap()//创建地图
{
	int i;
	for (i = 0; i <= WID; i++)//打印上下边框
	{
		setPos(2 * i, 0);
		printf("□");
		setPos(2 * i, HIG);
		printf("□");
	}
	for (i = 1; i < HIG; i++)//打印左右边框
	{
		setPos(0, i);
		printf("□");
		setPos(2 * WID, i);
		printf("□");
	}
}

void initSnake()//初始化蛇身
{
	snake *tail;
	int i;
	head = (snake*)malloc(sizeof(snake));//从蛇头开始，尾插法，以x,y设定开始的位置
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
	while (q != NULL)//从头到为，输出蛇身
	{
		setPos(q->x, q->y);
		printf("■");
		q = q->next;
	}
}

int isBiteSelf(snake *s)//判断是否咬到了自己
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

void createFood()//随机出现食物
{
	snake *food_1;
	srand((unsigned)time(NULL));
	food_1 = (snake*)malloc(sizeof(snake));
	food_1->x = 2 * (rand() % (WID - 1) + 1);
	food_1->y = rand() % (HIG - 1) + 1;
	q = head;
	while (q->next != NULL)
	{
		if (q->x == food_1->x && q->y == food_1->y) //判断蛇身是否与食物重合
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
	printf("★");
}

void cannotCrossWall()//不能穿墙
{
	if (q->x == 0 || q->x == WID * 2 || q->y == 0 || q->y == HIG)
	{
		endgamestatus = 1;
		endGame();
	}
}

void snakemove()//蛇前进,上U,下D,左L,右R
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
	cannotCrossWall();		//判断是否撞墙
	if (isBiteSelf(nexthead) == 1)       //判断是否会咬到自己
	{
		endgamestatus = 2;
		endGame();
	}
	if (nexthead->x == food->x && nexthead->y == food->y)//如果下一个有食物//
	{
		nexthead->next = head;
		head = nexthead;
		q = head;
		while (q != NULL)
		{
			setPos(q->x, q->y);
			printf("■");
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
	else                                               //如果没有食物//
	{
		nexthead->next = head;
		head = nexthead;
		q = head;
		while (q->next->next != NULL)
		{
			setPos(q->x, q->y);
			printf("■");
			q = q->next;
		}
		setPos(q->next->x, q->next->y);
		printf("  ");
		free(q->next);
		q->next = NULL;
	}
}

void pause()//暂停
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

void gameCircle()//控制游戏        
{

	setPos(64, 14);
	printf("不能穿墙，不能咬到自己\n");
	setPos(64, 15);
	printf("用↑.↓.←.→分别控制蛇的移动.");
	setPos(64, 16);
	printf("ESC ：退出游戏，space：暂停游戏.");
	status = L;
	while (1)
	{
		setPos(64, 10);
		printf("得分：%d  ", score);
		setPos(64, 11);
		printf("每个食物得分：%d分", add);
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

void welcomeToGame()//开始界面
{
	setPos(40, 12);

	system("title C语言程序游戏之贪吃蛇");
	printf("欢迎来到贪食蛇游戏！");
	setPos(40, 25);
	printf("              作者：薛奇\n");
	system("pause");
	system("cls");
	setPos(25, 12);
	printf("用↑.↓.←.→分别控制蛇的移动\n");
	setPos(25, 13);
	printf("蛇身越长，速度就越快。\n");
	system("pause");
	system("cls");
}

void endGame()//结束游戏
{
	system("cls");
	setPos(24, 12);
	if (endgamestatus == 1)
	{
		printf("对不起，您撞到墙了。游戏结束.");
	}
	else if (endgamestatus == 2)
	{
		printf("对不起，您咬到自己了。游戏结束.");
	}
	else if (endgamestatus == 3)
	{
		printf("您的已经结束了游戏。");
	}
	setPos(24, 13);
	printf("您的得分是%d\n", score);
	system("pause");
	exit(0);
}

void gameStart()//游戏初始化
{
	system("mode con cols=100 lines=30");//控制台宽度100，高度30
	welcomeToGame();
	creatMap();
	initSnake();
	createFood();
}