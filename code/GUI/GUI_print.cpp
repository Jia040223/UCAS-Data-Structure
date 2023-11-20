#include "head_file.h"

/*
E:\easyx\new\test.txt
*/


void GUI_Print(CITY_GRAPH G)
{
	int Mode;
	int ticket_num;
	int cnt = 0;

	TCHAR departure[MAXLEN];
	TCHAR destination[MAXLEN];
	TICKET ticket;

	int FLAG[MAXFLAG] = { 0 };
	int CNT[MAXFLAG] = { 0 };
	int idx = 0;
	int cnt_idx = 0;

	Init_Ticket(&ticket);

	initgraph(1340, 800);//新建窗口
	cleardevice();
	BeginBatchDraw();

	TCHAR* city_name;
	setfillcolor(0xFFFFFF);
	fillrectangle(0, 0, 1340, 100);

	FlushBatchDraw();

	if (G.city_num == 0)
		GUI_Print_Empty(&G);

RETURN_PRETABLE:
	for (int i = 0; i < G.city_num; i++)
	{
		for (int flag = 0; flag <= 1; flag++)
		{
			COLORREF background_color;
			if (flag == 0)
				background_color = 0xD1955A;
			else
				background_color = 0x56C476;
			cleardevice();

			TCHAR* city_name;

			CITY_VERTEX city = G.city[G.exist[i]];
			TICKET* ticket;


			city_name = multiByteToWideChar(city.name);

			TCHAR* information[15];
			CITY_EDGE* edge = city.first_edge;
			if (!edge)
				break;

			if (flag == 0)
				ticket = city.first_edge->first_plane_ticket;
			else
				ticket = city.first_edge->first_train_ticket;

			if (!ticket)
				continue;


			int return_cnt = 0;
			int temp = 0;
			while (ticket)
			{
				cleardevice();

				Print_Background(flag);

				settextstyle(50, 0, _T("隶书"));
				setbkmode(TRANSPARENT);
				settextcolor(BLACK);
				outtextxy(350, 0, city_name);

				if (flag == 0)
					*information = L"机场大屏";
				else
					*information = L"车站大屏";
				settextstyle(50, 0, _T("宋体"));
				outtextxy(570, 0, *information);
				Print_Info(flag);

				setlinecolor(BLACK);
				line(200, 100, 200, 700);
				line(500, 100, 500, 700);
				line(650, 100, 650, 700);
				line(800, 100, 800, 700);
				line(1000, 100, 1000, 700);
				line(1150, 100, 1150, 700);

				return_cnt = 0;
				temp = 0;
				for (int position = 3; position <= 13 && ticket; position++, return_cnt++)
				{
					if (FLAG[idx])
					{
						ticket = ticket->next_ticket;
						if (!ticket && edge->next_edge)
						{
							edge = edge->next_edge;
							if (flag == 0)
								ticket = edge->first_plane_ticket;
							else
								ticket = edge->first_train_ticket;
						}


						idx++;
						continue;
					}

					temp++;

					settextstyle(35, 0, _T("宋体"));
					string str0 = ticket->No;
					string str1 = ticket->destination;
					string str2 = str_Time(ticket->Boarding_Time, 0);
					string str3 = str_Time(ticket->During_Time, 1);
					string str4 = str_Time(ticket->Arrival_Time, 2);
					string str5;
					for (int week_day = 0; week_day < 7; week_day++)
						if (ticket->week[week_day])
							str5 = str5 + to_string(week_day + 1);
					string str6 = to_string(ticket->price);

					TCHAR* info0 = multiByteToWideChar(str0);
					outtextxy(40, position * 50, info0);
					TCHAR* info1 = multiByteToWideChar(str1);
					outtextxy(240, position * 50, info1);
					TCHAR* info2 = multiByteToWideChar(str2);
					outtextxy(520, position * 50, info2);
					TCHAR* info3 = multiByteToWideChar(str3);
					outtextxy(670, position * 50, info3);
					TCHAR* info4 = multiByteToWideChar(str4);
					outtextxy(820, position * 50, info4);
					TCHAR* info5 = multiByteToWideChar(str5);
					outtextxy(1010, position * 50, info5);
					TCHAR* info6 = multiByteToWideChar(str6);
					outtextxy(1170, position * 50, info6);


					ticket = ticket->next_ticket;

					if (position == 13 && ticket)
					{
						TCHAR work[10] = L"继续";
						TCHAR work2[10] = L"返回上一页";

						button(1000, 730, 200, 50, work, GREEN, WHITE, WHITE);
						if (cnt) {
							button(140, 730, 200, 50, work2, GREEN, WHITE, WHITE);
						}

						int button_flag = 0;
						ExMessage m;
						while (!button_flag)
						{
							BeginBatchDraw();
							m = getmessage(EX_MOUSE);
							switch (m.message)
							{
							case WM_MOUSEMOVE:
								button_move(m, 10, 1000, 730, 200, 50, work, GREEN, WHITE, WHITE, background_color);
								if (cnt)
									button_move(m, 10, 140, 730, 200, 50, work2, GREEN, WHITE, WHITE, background_color);
								break;

							case WM_LBUTTONDOWN:
								if (button_click(m, 10, 1000, 730, 200, 50, work, GREEN, WHITE, WHITE))
									button_flag = 1;

								if (cnt) {
									if (button_click(m, 10, 140, 730, 200, 50, work, GREEN, WHITE, WHITE)) {
										int return_idx = idx - return_cnt - CNT[cnt_idx - 1];
										for (; return_idx <= idx; return_idx++)
											FLAG[return_idx] = 0;

										cnt--;
										idx = 0;
										cnt_idx = 0;
										goto RETURN_PRETABLE;
									}
								}
								break;
							default:
								break;
							}
						}

						cnt++;
					}

					if (!ticket && edge->next_edge)
					{
						edge = edge->next_edge;
						if (flag == 0)
							ticket = edge->first_plane_ticket;
						else
							ticket = edge->first_train_ticket;
					}

					FLAG[idx++] = 1;
				}

				CNT[cnt_idx++] = return_cnt;
			}

			FlushBatchDraw();

			if (temp)
			{
				TCHAR work[10] = L"继续";
				TCHAR work2[10] = L"返回上一页";

				button(1000, 730, 200, 50, work, GREEN, WHITE, WHITE);
				if (cnt) {
					button(140, 730, 200, 50, work2, GREEN, WHITE, WHITE);
				}

				int button_flag = 0;
				ExMessage m;
				while (!button_flag)
				{
					BeginBatchDraw();
					m = getmessage(EX_MOUSE);
					switch (m.message)
					{
					case WM_MOUSEMOVE:
						button_move(m, 10, 1000, 730, 200, 50, work, GREEN, WHITE, WHITE, background_color);
						if (cnt)
							button_move(m, 10, 140, 730, 200, 50, work2, GREEN, WHITE, WHITE, background_color);
						break;

					case WM_LBUTTONDOWN:
						if (button_click(m, 10, 1000, 730, 200, 50, work, GREEN, WHITE, WHITE))
							button_flag = 1;

						if (cnt) {
							if (button_click(m, 10, 140, 730, 200, 50, work, GREEN, WHITE, WHITE)) {
								int return_idx = idx - return_cnt - CNT[cnt_idx - 2];
								for (; return_idx <= idx; return_idx++)
									FLAG[return_idx] = 0;

								cnt--;
								idx = 0;
								cnt_idx = 0;
								goto RETURN_PRETABLE;
							}
						}
						break;
					default:
						break;
					}
				}

				cnt++;
			}
		}
	}

	Status end_if = GUI_Print_Finished(&G);

	if (!end_if) {
		int return_idx = idx - CNT[cnt_idx - 1];
		for (; return_idx <= idx; return_idx++)
			FLAG[return_idx] = 0;

		cnt--;
		idx = 0;
		cnt_idx = 0;
		goto RETURN_PRETABLE;
	}
}


Status GUI_Print_Finished(CITY_GRAPH* pG) {
	int Mode = 0;

	initgraph(1340, 800);//新建窗口
	cleardevice();

	//	IMAGE background;
	//	loadimage(&background, L"./image/bg.png");
	//	putimage(0, 0, &background);//绘制图像到屏幕，图片左上角坐标为(0,0)

	setfillcolor(0xDCF8FF);
	fillrectangle(0, 0, 1340, 800);

	TCHAR  s1[50] = L"已全部打印完毕";

	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("隶书"));

	outtextxy(430, 250, s1);

	TCHAR  s2[50] = L"返回首页";
	TCHAR  s3[50] = L"返回第一页";
	TCHAR  s4[50] = L"返回最后一页";
	button(500, 400, 200, 50, s2, GREEN, WHITE, WHITE);
	button(500, 500, 200, 50, s3, GREEN, WHITE, WHITE);
	button(500, 600, 200, 50, s4, GREEN, WHITE, WHITE);


	ExMessage m;
	while (!Mode)
	{
		BeginBatchDraw();
		m = getmessage(EX_MOUSE);
		switch (m.message)
		{
		case WM_MOUSEMOVE:
			button_move(m, 10, 500, 400, 200, 50, s2, GREEN, WHITE, WHITE, BACKGROUND);
			button_move(m, 10, 500, 500, 200, 50, s3, GREEN, WHITE, WHITE, BACKGROUND);
			button_move(m, 10, 500, 600, 200, 50, s4, GREEN, WHITE, WHITE, BACKGROUND);
			break;

		case WM_LBUTTONDOWN:
			if (button_click(m, 10, 500, 400, 200, 50, s2, GREEN, WHITE, WHITE))
			{
				Mode = 1;
				closegraph();
				System(pG);
			}
			else if (button_click(m, 10, 500, 500, 200, 50, s3, GREEN, WHITE, WHITE))
			{
				Mode = 2;
				GUI_Print(*pG);
			}
			else if (button_click(m, 10, 500, 600, 200, 50, s4, GREEN, WHITE, WHITE))
			{
				Mode = 3;
				return ERROR;
			}
			break;

		default:
			break;
		}
	}

	return OK;
}

void GUI_Print_Empty(CITY_GRAPH* pG)
{
	int Mode = 0;

	initgraph(1340, 800);//新建窗口
	cleardevice();

	IMAGE background;
	loadimage(&background, L"./image/bg.png");
	putimage(0, 0, &background);//绘制图像到屏幕，图片左上角坐标为(0,0)

	TCHAR  s1[50] = L"打印时刻表为空";

	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("隶书"));

	outtextxy(430, 300, s1);

	TCHAR  s3[50] = L"返回";
	button(500, 500, 200, 50, s3, GREEN, WHITE, WHITE);


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
					break;
				default:
					break;
				}
			}

		}
	}
}


void Print_Background(int flag)
{
	if (flag == 0)
	{
		setlinecolor(0xD1955A);
		setfillcolor(0xD1955A);
		fillrectangle(0, 0, 1340, 100);
		IMAGE background;
		loadimage(&background, L"E:/easyx/new/picture2.png");
		fillrectangle(0, 700, 1340, 800);
		putimage(900, 10, &background);
	}
	else
	{
		setlinecolor(0x56C476);
		setfillcolor(0x56C476);
		fillrectangle(0, 0, 1340, 100);
		IMAGE background;
		loadimage(&background, L"E:/easyx/new/picture3.png");
		fillrectangle(0, 700, 1340, 800);
		putimage(1010, 10, &background);
	}

	setlinecolor(BLACK);
	for (int i = 3; i <= 14; i++)
	{
		if (i % 2)
			setfillcolor(0xC4C4C4);
		else
			setfillcolor(0xFFFFFF);
		fillrectangle(0, (i - 1) * 50, 1340, i * 50);
	}
}


void Print_Info(int flag)
{
	settextcolor(0x0);
	TCHAR* info0[10];
	if (!flag)
		*info0 = L"航班号";
	else
		*info0 = L"车次";
	TCHAR info1[10] = L"目的地";
	TCHAR info2[10] = L"出发时间";
	TCHAR info3[10] = L"历时";
	TCHAR info4[10] = L"到达时间";
	TCHAR info5[10] = L"日程安排";
	TCHAR info6[10] = L"票价";

	settextstyle(30, 0, _T("宋体"));
	outtextxy(40, 107, *info0);
	outtextxy(240, 107, info1);
	outtextxy(520, 107, info2);
	outtextxy(670, 107, info3);
	outtextxy(820, 107, info4);
	outtextxy(1010, 107, info5);
	outtextxy(1170, 107, info6);
}


string str_Time(TIME time, int flag)
{
	string str = "";
	if (time.hour < 10)
		str = "0";
	str = str + to_string(time.hour) + ":";
	if (time.minute < 10)
		str = str + "0";
	str = str + to_string(time.minute);
	if (flag == 2 && time.day != 0)
		str = str + "(+" + to_string(time.day) + ")";
	return str;
}

string str_During_Time(TIME time, int flag)
{
	string str = "";
	int hour = time.hour;
	if (flag) {
		while (time.day > 0) {
			hour += 24;
			time.day--;
		}
	}

	str = to_string(hour) + "h";
	if (time.minute < 10)
		str = str + "0";
	str = str + to_string(time.minute) + "m";
	return str;
}
