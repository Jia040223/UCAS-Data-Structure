#include "head_file.h"
#include <conio.h>
#include <windows.h>

#define BUFFERSIZE 1024
#define BACKGROUND_COLOR 0xDCF8FF
extern unsigned int month_day[13];


int Welcome() {
	int Mode = 0;
	initgraph(640, 480);//新建窗口
	cleardevice();

	CITY_GRAPH* pG = new CITY_GRAPH;
	Init_CITY_GRAPH(pG);

	setfillcolor(0xDCF8FF);
	fillrectangle(0, 0, 640, 480);

	IMAGE background;
	loadimage(&background, L"./image/bg.png");
	putimage(0, 0, &background);//绘制图像到屏幕，图片左上角坐标为(0,0)

	settextstyle(35, 0, _T("隶书"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	TCHAR intruduction[50] = L"数据结构大作业展示";
	TCHAR intruduction1[50] = L"作者：贾城昊 李金明";
	outtextxy(20, 20, intruduction);
	outtextxy(70, 70, intruduction1);

	settextstyle(40, 0, _T("黑体"));
	settextcolor(BLACK);
	TCHAR intruduction2[50] = L"GKD交通咨询系统";
	outtextxy(160, 150, intruduction2);

	settextstyle(25, 0, _T("楷书"));
	settextcolor(WHITE);
	TCHAR intruduction3[50] = L"欢迎进入 交通咨询系统";
	TCHAR intruduction4[50] = L"点击开始体验";

	outtextxy(170, 250, intruduction3);
	int change = 10;
	int flag = 0;
	ExMessage m;
	button(200, 350, 200, 50, intruduction4, GREEN, WHITE, WHITE);
	while (!flag)
	{
		BeginBatchDraw();
		m = getmessage(EX_MOUSE);
		switch (m.message)
		{
		case WM_MOUSEMOVE:
			button_move(m, change, 200, 350, 200, 50, intruduction4, GREEN, WHITE, WHITE, BACKGROUND_COLOR);
			break;

		case WM_LBUTTONDOWN:
			flag = button_click(m, change, 200, 350, 200, 50, intruduction4, GREEN, WHITE, WHITE);
			break;
		default:
			break;
		}
	}

	closegraph();

	System(pG);

	return 0;
}


int System(CITY_GRAPH* pG) {
	int Mode = 0;

	initgraph(1340, 800);//新建窗口
	cleardevice();

	setfillcolor(0xDCF8FF);
	fillrectangle(0, 0, 1340, 800);

	IMAGE background;
	loadimage(&background, L"./image/bg.png");
	putimage(0, 0, &background);//绘制图像到屏幕，图片左上角坐标为(0,0)

	TCHAR s0[50] = L"1.插入行程";
	TCHAR s1[50] = L"2.删除行程";
	TCHAR s2[50] = L"3.搜索路线";
	TCHAR s3[50] = L"4.打印时刻表";
	TCHAR s4[50] = L"5.文档导入";
	TCHAR s5[50] = L"6.退出";

	settextstyle(35, 0, _T("隶书"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	TCHAR intruduction[50] = L"现有城市:";
	outtextxy(70, 100, intruduction);

	settextstyle(35, 0, _T("隶书"));

	int height = 140;
	int weight = 70;
	int num = 1;
	for (int i = 0; i < MAXCITY; i++) {
		if (pG->city[i].exist) {
			settextstyle(35, 0, _T("隶书"));
			string str = pG->city[i].name;
			str = to_string(num++) + ": " + str;
			TCHAR* s;
			s = multiByteToWideChar(str);

			outtextxy(70, height, s);
			height += 40;
			if (height >= 750) {
				height = 140;
				weight += 120;
			}
		}
	}

	if (num == 1) {
		TCHAR s6[50] = L"无";
		outtextxy(90, 160, s6);
	}

	settextstyle(10, -10, _T("隶书"));

	button(20, 20, 200, 50, s0, GREEN, WHITE, WHITE);
	button(240, 20, 200, 50, s1, GREEN, WHITE, WHITE);
	button(460, 20, 200, 50, s2, GREEN, WHITE, WHITE);
	button(680, 20, 200, 50, s3, GREEN, WHITE, WHITE);
	button(900, 20, 200, 50, s4, GREEN, WHITE, WHITE);
	button(1120, 20, 200, 50, s5, GREEN, WHITE, WHITE);

	int change = 10;
	ExMessage m;
	while (!Mode)
	{
		BeginBatchDraw();
		m = getmessage(EX_MOUSE);
		switch (m.message)
		{
		case WM_MOUSEMOVE:
			button_move(m, change, 20, 20, 200, 50, s0, GREEN, WHITE, WHITE, BACKGROUND_COLOR);
			button_move(m, change, 240, 20, 200, 50, s1, GREEN, WHITE, WHITE, BACKGROUND_COLOR);
			button_move(m, change, 460, 20, 200, 50, s2, GREEN, WHITE, WHITE, BACKGROUND_COLOR);
			button_move(m, change, 680, 20, 200, 50, s3, GREEN, WHITE, WHITE, BACKGROUND_COLOR);
			button_move(m, change, 900, 20, 200, 50, s4, GREEN, WHITE, WHITE, BACKGROUND_COLOR);
			button_move(m, change, 1120, 20, 200, 50, s5, GREEN, WHITE, WHITE, BACKGROUND_COLOR);
			break;

		case WM_LBUTTONDOWN:
			if (button_click(m, change, 20, 20, 200, 50, s0, GREEN, WHITE, WHITE))
			{
				Mode = 1;
				GUI_Insert(pG);
			}
			if (button_click(m, change, 240, 20, 200, 50, s1, GREEN, WHITE, WHITE))
			{
				Mode = 2;
				GUI_Delete(pG);
			}
			else if (button_click(m, change, 460, 20, 200, 50, s2, GREEN, WHITE, WHITE))
			{
				Mode = 3;
				GUI_Search(pG);
			}
			else if (button_click(m, change, 680, 20, 200, 50, s3, GREEN, WHITE, WHITE))
			{
				Mode = 4;
				GUI_Print(*pG);
			}
			else if (button_click(m, change, 900, 20, 200, 50, s4, GREEN, WHITE, WHITE))
			{
				Mode = 5;
				GUI_File(pG);
			}
			else if (button_click(m, change, 1120, 20, 200, 50, s5, GREEN, WHITE, WHITE))
			{
				Mode = 6;
				GUI_Quit(pG);
			}
			break;
		default:
			break;
		}
	}
	return 0;
}



//Insert Ticket
void GUI_Insert(CITY_GRAPH* pG) {
	int Mode = 0;

	TCHAR departure[MAXLEN];
	TCHAR destination[MAXLEN];
	TCHAR Boarding_Time[MAXLEN];
	TCHAR During_Time[MAXLEN];
	TCHAR Week[MAXLEN];
	TCHAR traffic_mode[MAXLEN];
	TCHAR price[MAXLEN];
	TCHAR No[MAXLEN];

	TICKET ticket;
	char* temp;
	Init_Ticket(&ticket);

	int status = 0;
	while (!status) {
		InputBox(departure, MAXLEN, L"请输入出发地");
		if (departure[0] == L'\0') {
			MessageBox(NULL, L"出发城市名不能为空", L"提示", MB_OK);
			continue;
		}
		temp = TCHARToChar(departure);
		strcpy(ticket.departure, temp);
		status = 1;
	}

	status = 0;
	while (!status) {
		InputBox(destination, MAXLEN, L"请输入目的地");
		if (destination[0] == L'\0') {
			MessageBox(NULL, L"目的城市名不能为空", L"提示", MB_OK);
			continue;
		}
		temp = TCHARToChar(destination);
		strcpy(ticket.destination, temp);
		status = 1;
	}

	TIME* T = new TIME;

	//trafic mode
	status = 0;
	while (!status) {
		InputBox(traffic_mode, MAXLEN, L"请输入插入的是机票还是火车票");
		temp = TCHARToChar(traffic_mode);
		status = GUI_Get_Traffic_Mode(&ticket, temp);
		if (!status) {
			MessageBox(NULL, L"输入格式有误,请重新输入", L"提示", MB_OK);
		}
	}

	//No
	status = 0;
	while (!status) {
		InputBox(No, MAXLEN, L"请输入航班号/车次");
		if (No[0] == L'\0') {
			MessageBox(NULL, L"航班号/车次不能为空", L"提示", MB_OK);
			continue;
		}
		temp = TCHARToChar(No);
		strcpy(ticket.No, temp);
		status = 1;
	}

	//Boarding time
	status = 0;
	while (!status) {
		InputBox(Boarding_Time, MAXLEN, L"请输入起始时间");
		temp = TCHARToChar(Boarding_Time);
		status = GUI_Get_Ticket_Time(T, temp, 1);
		if (!status) {
			MessageBox(NULL, L"输入格式有误,请重新输入", L"提示", MB_OK);
		}
		ticket.Boarding_Time = *T;
	}

	//During time
	status = 0;
	while (!status) {
		InputBox(During_Time, MAXLEN, L"请输入经过时间");
		temp = TCHARToChar(During_Time);
		status = GUI_Get_Ticket_Time(T, temp, 0);
		if (!status) {
			MessageBox(NULL, L"输入格式有误,请重新输入", L"提示", MB_OK);
		}
		ticket.During_Time = *T;
	}

	//Arrival time
	Time_Addition(ticket.Boarding_Time, ticket.During_Time, &ticket.Arrival_Time);

	//price
	status = 0;
	while (!status) {
		InputBox(price, MAXLEN, L"请输入票价");
		temp = TCHARToChar(price);
		status = GUI_Get_Price(&ticket, temp);
		if (!status) {
			MessageBox(NULL, L"输入格式有误,请重新输入", L"提示", MB_OK);
		}
	}

	//Week schedule
	status = 0;
	while (!status) {
		InputBox(Week, MAXLEN, L"请输入安排");
		temp = TCHARToChar(Week);
		status = GUI_Fill_Ticket_week(&ticket, temp);
		if (!status) {
			MessageBox(NULL, L"输入格式有误,请重新输入", L"提示", MB_OK);
		}
	}

	Status result = Insert_Ticket(pG, ticket);

	initgraph(1340, 800);//新建窗口
	cleardevice();

	IMAGE background;
	loadimage(&background, L"./image/bg.png");
	putimage(0, 0, &background);//绘制图像到屏幕，图片左上角坐标为(0,0)

	TCHAR  s[50] = L"插入成功";
	TCHAR  s1[50] = L"插入失败";

	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(50, 0, _T("隶书"));

	if (result)
		outtextxy(550, 370, s);
	else
		outtextxy(480, 370, s1);


	TCHAR  s2[50] = L"返回";
	TCHAR  s3[50] = L"继续插入";
	button(570, 500, 200, 50, s2, GREEN, WHITE, WHITE);
	button(570, 600, 200, 50, s3, GREEN, WHITE, WHITE);



	while (!Mode)
	{
		while (_kbhit())//如果有按键则进入，否则不进入循环
		{
			char userkey = 0;
			userkey = _getch();
			if (userkey == VK_ESCAPE)
			{
				Mode = 0;
			}
		}

		if (Mode == 0)
		{
			ExMessage msg;
			if (peekmessage(&msg, EX_MOUSE))
			{
				switch (msg.message)
				{
				case WM_LBUTTONDOWN:
					if (msg.x >= 570 && msg.x <= 570 + 200 && msg.y >= 500 && msg.y <= 500 + 50)
					{
						Mode = 1;
						closegraph();
						System(pG);
					}
					if (msg.x >= 570 && msg.x <= 570 + 200 && msg.y >= 600 && msg.y <= 600 + 50)
					{
						Mode = 2;
						GUI_Insert(pG);
					}
					break;
				default:
					break;
				}
			}

		}
	}
}



//Delete Ticket
void GUI_Delete(CITY_GRAPH* pG) {
	int Mode = 0;

	TCHAR departure[MAXLEN];
	TCHAR destination[MAXLEN];
	TCHAR traffic_mode[MAXLEN];
	TCHAR No[MAXLEN];
	char* temp;

	TICKET ticket;
	Init_Ticket(&ticket);

	int status = 0;
	while (!status) {
		InputBox(departure, MAXLEN, L"请输入出发地");
		if (departure[0] == L'\0') {
			MessageBox(NULL, L"出发城市名不能为空", L"提示", MB_OK);
			continue;
		}
		temp = TCHARToChar(departure);
		strcpy(ticket.departure, temp);
		status = 1;
	}

	status = 0;
	while (!status) {
		InputBox(destination, MAXLEN, L"请输入目的地");
		if (destination[0] == L'\0') {
			MessageBox(NULL, L"目的城市名不能为空", L"提示", MB_OK);
			continue;
		}
		temp = TCHARToChar(destination);
		strcpy(ticket.destination, temp);
		status = 1;
	}

	TIME* T = new TIME;

	//trafic mode
	status = 0;
	while (!status) {
		InputBox(traffic_mode, MAXLEN, L"请输入删除的是机票还是火车票");
		temp = TCHARToChar(traffic_mode);
		status = GUI_Get_Traffic_Mode(&ticket, temp);
		if (!status) {
			MessageBox(NULL, L"输入格式有误,请重新输入", L"提示", MB_OK);
		}
	}

	//No
	status = 0;
	while (!status) {
		InputBox(No, MAXLEN, L"请输入航班号/车次");
		if (No[0] == L'\0') {
			MessageBox(NULL, L"航班号/车次不能为空", L"提示", MB_OK);
			continue;
		}
		temp = TCHARToChar(No);
		strcpy(ticket.No, temp);
		status = 1;
	}


	Status result = Delete_Ticket(pG, ticket);

	initgraph(1340, 800);//新建窗口
	cleardevice();

	IMAGE background;
	loadimage(&background, L"./image/bg.png");
	putimage(0, 0, &background);//绘制图像到屏幕，图片左上角坐标为(0,0)

	TCHAR  s[50] = L"删除成功";
	TCHAR  s1[50] = L"删除失败，该票不存在";

	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("隶书"));

	if (result)
		outtextxy(550, 370, s);
	else
		outtextxy(400, 370, s1);


	TCHAR  s2[50] = L"返回";
	TCHAR  s3[50] = L"继续删除";
	button(570, 500, 200, 50, s2, GREEN, WHITE, WHITE);
	button(570, 600, 200, 50, s3, GREEN, WHITE, WHITE);


	while (!Mode)
	{
		while (_kbhit())//如果有按键则进入，否则不进入循环
		{
			char userkey = 0;
			userkey = _getch();
			if (userkey == VK_ESCAPE)
			{
				Mode = 0;
			}
		}

		if (Mode == 0)
		{
			ExMessage msg;
			if (peekmessage(&msg, EX_MOUSE))
			{
				switch (msg.message)
				{
				case WM_LBUTTONDOWN:
					if (msg.x >= 570 && msg.x <= 570 + 200 && msg.y >= 500 && msg.y <= 500 + 50)
					{
						Mode = 1;
						closegraph();
						System(pG);
					}
					if (msg.x >= 570 && msg.x <= 570 + 200 && msg.y >= 600 && msg.y <= 600 + 50)
					{
						Mode = 2;
						GUI_Delete(pG);
					}
					break;

				default:
					break;
				}
			}

		}
	}
}



void GUI_File(CITY_GRAPH* pG) {
	int Mode = 0;

	TCHAR TEXT[MAXLEN];

	TICKET ticket;
	Init_Ticket(&ticket);

	InputBox(TEXT, MAXLEN, L"请输入导入的文件名");

	char* text = TCHARToChar(TEXT);

	Status result = Operate_Ticket_from_File(pG, text);

	initgraph(1340, 800);//新建窗口
	cleardevice();

	IMAGE background;
	loadimage(&background, L"./image/bg.png");
	putimage(0, 0, &background);//绘制图像到屏幕，图片左上角坐标为(0,0)

	TCHAR  s[50] = L"导入成功";
	TCHAR  s1[50] = L"导入失败，文件不存在或格式有误";

	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("隶书"));

	if (result)
		outtextxy(550, 370, s);
	else
		outtextxy(360, 370, s1);


	TCHAR  s2[50] = L"返回";
	TCHAR  s3[50] = L"继续导入";
	button(570, 500, 200, 50, s2, GREEN, WHITE, WHITE);
	button(570, 600, 200, 50, s3, GREEN, WHITE, WHITE);


	while (!Mode)
	{
		while (_kbhit())//如果有按键则进入，否则不进入循环
		{
			char userkey = 0;
			userkey = _getch();
			if (userkey == VK_ESCAPE)
			{
				Mode = 0;
			}
		}

		if (Mode == 0)
		{
			ExMessage msg;
			if (peekmessage(&msg, EX_MOUSE))
			{
				switch (msg.message)
				{
				case WM_LBUTTONDOWN:
					if (msg.x >= 570 && msg.x <= 570 + 200 && msg.y >= 500 && msg.y <= 500 + 50)
					{
						Mode = 1;
						closegraph();
						System(pG);
					}
					if (msg.x >= 570 && msg.x <= 570 + 200 && msg.y >= 600 && msg.y <= 600 + 50)
					{
						Mode = 2;
						GUI_File(pG);
					}
					break;

				default:
					break;
				}
			}

		}
	}
}



void GUI_Quit(CITY_GRAPH* pG) {
	closegraph();
}
