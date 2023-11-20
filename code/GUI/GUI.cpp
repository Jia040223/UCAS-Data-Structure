#include "head_file.h"
#include <conio.h>
#include <windows.h>

#define BUFFERSIZE 1024
#define BACKGROUND_COLOR 0xDCF8FF
extern unsigned int month_day[13];


int Welcome() {
	int Mode = 0;
	initgraph(640, 480);//�½�����
	cleardevice();

	CITY_GRAPH* pG = new CITY_GRAPH;
	Init_CITY_GRAPH(pG);

	setfillcolor(0xDCF8FF);
	fillrectangle(0, 0, 640, 480);

	IMAGE background;
	loadimage(&background, L"./image/bg.png");
	putimage(0, 0, &background);//����ͼ����Ļ��ͼƬ���Ͻ�����Ϊ(0,0)

	settextstyle(35, 0, _T("����"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	TCHAR intruduction[50] = L"���ݽṹ����ҵչʾ";
	TCHAR intruduction1[50] = L"���ߣ��ֳ�� �����";
	outtextxy(20, 20, intruduction);
	outtextxy(70, 70, intruduction1);

	settextstyle(40, 0, _T("����"));
	settextcolor(BLACK);
	TCHAR intruduction2[50] = L"GKD��ͨ��ѯϵͳ";
	outtextxy(160, 150, intruduction2);

	settextstyle(25, 0, _T("����"));
	settextcolor(WHITE);
	TCHAR intruduction3[50] = L"��ӭ���� ��ͨ��ѯϵͳ";
	TCHAR intruduction4[50] = L"�����ʼ����";

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

	initgraph(1340, 800);//�½�����
	cleardevice();

	setfillcolor(0xDCF8FF);
	fillrectangle(0, 0, 1340, 800);

	IMAGE background;
	loadimage(&background, L"./image/bg.png");
	putimage(0, 0, &background);//����ͼ����Ļ��ͼƬ���Ͻ�����Ϊ(0,0)

	TCHAR s0[50] = L"1.�����г�";
	TCHAR s1[50] = L"2.ɾ���г�";
	TCHAR s2[50] = L"3.����·��";
	TCHAR s3[50] = L"4.��ӡʱ�̱�";
	TCHAR s4[50] = L"5.�ĵ�����";
	TCHAR s5[50] = L"6.�˳�";

	settextstyle(35, 0, _T("����"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	TCHAR intruduction[50] = L"���г���:";
	outtextxy(70, 100, intruduction);

	settextstyle(35, 0, _T("����"));

	int height = 140;
	int weight = 70;
	int num = 1;
	for (int i = 0; i < MAXCITY; i++) {
		if (pG->city[i].exist) {
			settextstyle(35, 0, _T("����"));
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
		TCHAR s6[50] = L"��";
		outtextxy(90, 160, s6);
	}

	settextstyle(10, -10, _T("����"));

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
		InputBox(departure, MAXLEN, L"�����������");
		if (departure[0] == L'\0') {
			MessageBox(NULL, L"��������������Ϊ��", L"��ʾ", MB_OK);
			continue;
		}
		temp = TCHARToChar(departure);
		strcpy(ticket.departure, temp);
		status = 1;
	}

	status = 0;
	while (!status) {
		InputBox(destination, MAXLEN, L"������Ŀ�ĵ�");
		if (destination[0] == L'\0') {
			MessageBox(NULL, L"Ŀ�ĳ���������Ϊ��", L"��ʾ", MB_OK);
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
		InputBox(traffic_mode, MAXLEN, L"�����������ǻ�Ʊ���ǻ�Ʊ");
		temp = TCHARToChar(traffic_mode);
		status = GUI_Get_Traffic_Mode(&ticket, temp);
		if (!status) {
			MessageBox(NULL, L"�����ʽ����,����������", L"��ʾ", MB_OK);
		}
	}

	//No
	status = 0;
	while (!status) {
		InputBox(No, MAXLEN, L"�����뺽���/����");
		if (No[0] == L'\0') {
			MessageBox(NULL, L"�����/���β���Ϊ��", L"��ʾ", MB_OK);
			continue;
		}
		temp = TCHARToChar(No);
		strcpy(ticket.No, temp);
		status = 1;
	}

	//Boarding time
	status = 0;
	while (!status) {
		InputBox(Boarding_Time, MAXLEN, L"��������ʼʱ��");
		temp = TCHARToChar(Boarding_Time);
		status = GUI_Get_Ticket_Time(T, temp, 1);
		if (!status) {
			MessageBox(NULL, L"�����ʽ����,����������", L"��ʾ", MB_OK);
		}
		ticket.Boarding_Time = *T;
	}

	//During time
	status = 0;
	while (!status) {
		InputBox(During_Time, MAXLEN, L"�����뾭��ʱ��");
		temp = TCHARToChar(During_Time);
		status = GUI_Get_Ticket_Time(T, temp, 0);
		if (!status) {
			MessageBox(NULL, L"�����ʽ����,����������", L"��ʾ", MB_OK);
		}
		ticket.During_Time = *T;
	}

	//Arrival time
	Time_Addition(ticket.Boarding_Time, ticket.During_Time, &ticket.Arrival_Time);

	//price
	status = 0;
	while (!status) {
		InputBox(price, MAXLEN, L"������Ʊ��");
		temp = TCHARToChar(price);
		status = GUI_Get_Price(&ticket, temp);
		if (!status) {
			MessageBox(NULL, L"�����ʽ����,����������", L"��ʾ", MB_OK);
		}
	}

	//Week schedule
	status = 0;
	while (!status) {
		InputBox(Week, MAXLEN, L"�����밲��");
		temp = TCHARToChar(Week);
		status = GUI_Fill_Ticket_week(&ticket, temp);
		if (!status) {
			MessageBox(NULL, L"�����ʽ����,����������", L"��ʾ", MB_OK);
		}
	}

	Status result = Insert_Ticket(pG, ticket);

	initgraph(1340, 800);//�½�����
	cleardevice();

	IMAGE background;
	loadimage(&background, L"./image/bg.png");
	putimage(0, 0, &background);//����ͼ����Ļ��ͼƬ���Ͻ�����Ϊ(0,0)

	TCHAR  s[50] = L"����ɹ�";
	TCHAR  s1[50] = L"����ʧ��";

	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(50, 0, _T("����"));

	if (result)
		outtextxy(550, 370, s);
	else
		outtextxy(480, 370, s1);


	TCHAR  s2[50] = L"����";
	TCHAR  s3[50] = L"��������";
	button(570, 500, 200, 50, s2, GREEN, WHITE, WHITE);
	button(570, 600, 200, 50, s3, GREEN, WHITE, WHITE);



	while (!Mode)
	{
		while (_kbhit())//����а�������룬���򲻽���ѭ��
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
		InputBox(departure, MAXLEN, L"�����������");
		if (departure[0] == L'\0') {
			MessageBox(NULL, L"��������������Ϊ��", L"��ʾ", MB_OK);
			continue;
		}
		temp = TCHARToChar(departure);
		strcpy(ticket.departure, temp);
		status = 1;
	}

	status = 0;
	while (!status) {
		InputBox(destination, MAXLEN, L"������Ŀ�ĵ�");
		if (destination[0] == L'\0') {
			MessageBox(NULL, L"Ŀ�ĳ���������Ϊ��", L"��ʾ", MB_OK);
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
		InputBox(traffic_mode, MAXLEN, L"������ɾ�����ǻ�Ʊ���ǻ�Ʊ");
		temp = TCHARToChar(traffic_mode);
		status = GUI_Get_Traffic_Mode(&ticket, temp);
		if (!status) {
			MessageBox(NULL, L"�����ʽ����,����������", L"��ʾ", MB_OK);
		}
	}

	//No
	status = 0;
	while (!status) {
		InputBox(No, MAXLEN, L"�����뺽���/����");
		if (No[0] == L'\0') {
			MessageBox(NULL, L"�����/���β���Ϊ��", L"��ʾ", MB_OK);
			continue;
		}
		temp = TCHARToChar(No);
		strcpy(ticket.No, temp);
		status = 1;
	}


	Status result = Delete_Ticket(pG, ticket);

	initgraph(1340, 800);//�½�����
	cleardevice();

	IMAGE background;
	loadimage(&background, L"./image/bg.png");
	putimage(0, 0, &background);//����ͼ����Ļ��ͼƬ���Ͻ�����Ϊ(0,0)

	TCHAR  s[50] = L"ɾ���ɹ�";
	TCHAR  s1[50] = L"ɾ��ʧ�ܣ���Ʊ������";

	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("����"));

	if (result)
		outtextxy(550, 370, s);
	else
		outtextxy(400, 370, s1);


	TCHAR  s2[50] = L"����";
	TCHAR  s3[50] = L"����ɾ��";
	button(570, 500, 200, 50, s2, GREEN, WHITE, WHITE);
	button(570, 600, 200, 50, s3, GREEN, WHITE, WHITE);


	while (!Mode)
	{
		while (_kbhit())//����а�������룬���򲻽���ѭ��
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

	InputBox(TEXT, MAXLEN, L"�����뵼����ļ���");

	char* text = TCHARToChar(TEXT);

	Status result = Operate_Ticket_from_File(pG, text);

	initgraph(1340, 800);//�½�����
	cleardevice();

	IMAGE background;
	loadimage(&background, L"./image/bg.png");
	putimage(0, 0, &background);//����ͼ����Ļ��ͼƬ���Ͻ�����Ϊ(0,0)

	TCHAR  s[50] = L"����ɹ�";
	TCHAR  s1[50] = L"����ʧ�ܣ��ļ������ڻ��ʽ����";

	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("����"));

	if (result)
		outtextxy(550, 370, s);
	else
		outtextxy(360, 370, s1);


	TCHAR  s2[50] = L"����";
	TCHAR  s3[50] = L"��������";
	button(570, 500, 200, 50, s2, GREEN, WHITE, WHITE);
	button(570, 600, 200, 50, s3, GREEN, WHITE, WHITE);


	while (!Mode)
	{
		while (_kbhit())//����а�������룬���򲻽���ѭ��
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
