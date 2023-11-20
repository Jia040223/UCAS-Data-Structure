#include "head_file.h"

TCHAR* WEEK_DAY_CHINESE[7] = { L"周一", L"周二", L"周三", L"周四", L"周五", L"周六", L"周日" };

int Print_Route_Info(ROUTE route, WILL will, int cnt);
void Print_Search_Background(ROUTE route, WILL will);

string str_Date(TIME time);

//Search Route
void GUI_Search(CITY_GRAPH* pG)
{
	int Mode = 0;

	int cnt = 0;
	int FLAG[5] = { 0 };
	int idx = 0;

	WILL will;
	int route_num;
	char c;
	ROUTE* route = (ROUTE*)malloc(MAXROUTE * sizeof(ROUTE));

	TCHAR departure[MAXLEN];
	TCHAR destination[MAXLEN];
	TCHAR Expection_Time[MAXLEN];
	TCHAR traffic_mode[MAXLEN];
	TCHAR priority[MAXLEN];
	char* temp;

	will.departure = (char*)malloc(MAXLEN * sizeof(char));
	will.destination = (char*)malloc(MAXLEN * sizeof(char));

	int status = 0;
	while (!status) {
		InputBox(departure, MAXLEN, L"请输入出发地");
		if (departure[0] == '\0') {
			MessageBox(NULL, L"出发城市名不能为空", L"提示", MB_OK);
			continue;
		}
		temp = TCHARToChar(departure);
		strcpy(will.departure, temp);
		status = 1;
	}

	status = 0;
	while (!status) {
		InputBox(destination, MAXLEN, L"请输入目的地");
		if (destination[0] == '\0') {
			MessageBox(NULL, L"目的城市名不能为空", L"提示", MB_OK);
			continue;
		}
		temp = TCHARToChar(destination);
		strcpy(will.destination, temp);
		status = 1;
	}

	TIME* T = new TIME;

	//trafic mode
	status = 0;
	while (!status) {
		InputBox(traffic_mode, MAXLEN, L"请输入搜索的是机票还是火车票");
		temp = TCHARToChar(traffic_mode);
		status = GUI_Get_WILL_Traffic_Mode(&will, temp);
		if (!status) {
			MessageBox(NULL, L"输入格式有误,请重新输入", L"提示", MB_OK);
		}
	}

	//priority
	status = 0;
	while (!status) {
		InputBox(priority, MAXLEN, L"请输入您的优先策略");
		temp = TCHARToChar(priority);
		status = GUI_Get_WILL_Priority(&will, temp);
		if (!status) {
			MessageBox(NULL, L"输入格式有误,请重新输入", L"提示", MB_OK);
		}
	}


	//Boarding time
	status = 0;
	while (!status) {
		InputBox(Expection_Time, MAXLEN, L"请输入起始时间");
		temp = TCHARToChar(Expection_Time);
		status = GUI_Get_Expectation_Time(T, temp);
		if (!status) {
			MessageBox(NULL, L"输入格式有误,请重新输入", L"提示", MB_OK);
		}
		will.boarding_time = *T;
	}

	//Week Schedule
	will.Week_day = Get_Week_Day(will.boarding_time);

	route_num = Find_Route(*pG, will, route);

	initgraph(1340, 800);//新建窗口

	setfillcolor(BACKGROUND);
	fillrectangle(0, 0, 1340, 800);

	TCHAR  s[50] = L"查找成功";
	TCHAR  s1[50] = L"查找失败,无可达路径";

	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("隶书"));

RETURN_PRETABLE:
	if (route_num)
	{
		for (int i = 0; i < route_num && i < 5; i++, idx++)
		{
			if (FLAG[idx])
				continue;
			else
				FLAG[idx] = 1;

			int print_result = Print_Route_Info(route[i], will, cnt);
			if (print_result == 1) {
				goto Print_Result1;
			}
			else if (print_result == 2) {
				goto Print_Result2;
			}


			TCHAR  pre[10] = L"上一个方案";
			TCHAR  next[10] = L"下一个方案";
			button(1000, 730, 200, 50, next, GREEN, WHITE, WHITE);
			if (cnt) 
				button(140, 730, 200, 50, pre, GREEN, WHITE, WHITE);

			FlushBatchDraw();
			int Mode = 0;
			ExMessage m;
			while (!Mode)
			{
				BeginBatchDraw();
				m = getmessage(EX_MOUSE);
				switch (m.message)
				{
				case WM_MOUSEMOVE:
					button_move(m, 10, 1000, 730, 200, 50, next, GREEN, WHITE, WHITE, BACKGROUND);
					if (cnt)
						button_move(m, 10, 140, 730, 200, 50, pre, GREEN, WHITE, WHITE, BACKGROUND);
					break;

				case WM_LBUTTONDOWN:
					if (button_click(m, 10, 1000, 730, 200, 50, next, GREEN, WHITE, WHITE))
					{
                    Print_Result1:
						Mode = 1;
						FlushBatchDraw();
						continue;
					}
					else if (cnt && button_click(m, 10, 140, 730, 200, 50, pre, GREEN, WHITE, WHITE))
					{
					Print_Result2:
						Mode = 2;	
						FlushBatchDraw();

						int return_idx = idx - 1;
						for (;return_idx <= idx;return_idx++) {
							FLAG[return_idx] = 0;
						}

						cnt--;
						idx = 0;
						goto RETURN_PRETABLE;
					}
					break;
				default:
					break;
				}
			}

			cnt++;

		}
		cleardevice();
		setfillcolor(BACKGROUND);
		fillrectangle(0, 0, 1340, 800);
	}
	//		outtextxy(550, 370, s);
	else
		outtextxy(400, 370, s1);
	

	TCHAR  s2[50] = L"返回首页";
	TCHAR  s3[50] = L"继续查找";
	TCHAR  s4[50] = L"返回最后方案";
	TCHAR  s5[50] = L"返回首个方案";
	button(570, 300, 200, 50, s5, GREEN, WHITE, WHITE);
	button(570, 400, 200, 50, s4, GREEN, WHITE, WHITE);
	button(570, 500, 200, 50, s2, GREEN, WHITE, WHITE);
	button(570, 600, 200, 50, s3, GREEN, WHITE, WHITE);
	FlushBatchDraw();
	ExMessage m;

	//	IMAGE background;
	//	loadimage(&background, L"./image/bg.png");
	//  putimage(0, 0, &background);//绘制图像到屏幕，图片左上角坐标为(0,0)

	while (!Mode)
	{
		BeginBatchDraw();
		m = getmessage(EX_MOUSE);
		switch (m.message)
		{
		case WM_MOUSEMOVE:
			button_move(m, 10, 570, 300, 200, 50, s5, GREEN, WHITE, WHITE, BACKGROUND);
			button_move(m, 10, 570, 400, 200, 50, s4, GREEN, WHITE, WHITE, BACKGROUND);
			button_move(m, 10, 570, 500, 200, 50, s2, GREEN, WHITE, WHITE, BACKGROUND);
			button_move(m, 10, 570, 600, 200, 50, s3, GREEN, WHITE, WHITE, BACKGROUND);
			break;

		case WM_LBUTTONDOWN:
			if (button_click(m, 10, 570, 500, 200, 50, s2, GREEN, WHITE, WHITE))
			{
				Mode = 1;
				closegraph();
				System(pG);
			}
			else if (button_click(m, 10, 570, 600, 200, 50, s3, GREEN, WHITE, WHITE))
			{
				Mode = 2;
				GUI_Search(pG);
			}
			else if (button_click(m, 10, 570, 400, 200, 50, s3, GREEN, WHITE, WHITE))
			{
				Mode = 0;
				FLAG[idx - 1] = 0;
				idx = 0;
				cnt--;
				goto RETURN_PRETABLE;
			}
			else if (button_click(m, 10, 570, 300, 200, 50, s3, GREEN, WHITE, WHITE))
			{
				Mode = 0;
				for (int i = 0;i < 5;i++) {
					FLAG[i] = 0;
				}
				idx = 0;
				cnt = 0;
				goto RETURN_PRETABLE;
			}
			break;
		default:
			break;
		}
	}

	free(route);
	delete T;
	free(will.departure);
	free(will.destination);
}

int Print_Route_Info(ROUTE route, WILL will, int plan_cnt)
{	
	int num = 0;
	int FLAG[MAXFLAG] = { 0 };
	int CNT[MAXFLAG] = { 0 };
	int idx = 0;
	int cnt_idx = 0;

RETURN_PRETABLE:
	BeginBatchDraw();
	TICKET* ticket;
	ticket = route.first_ticket;
	int cnt = 1;

	while (ticket)
	{
		cleardevice();
		Print_Search_Background(route, will);

		setlinecolor(BLACK);
		line(0, 700, 1340, 700);
		line(0, 450, 1340, 450);
		line(670, 200, 670, 700);

		int return_cnt = 0;
		for (int j = 0; j <= 1 && ticket; j++)
		{
			for (int i = 0; i <= 1 && ticket; i++, return_cnt++)
			{
				if (FLAG[idx])
				{
					ticket = ticket->next_ticket;
					idx++;
					continue;
				}

				setlinecolor(BLACK);
				setfillcolor(0x98CB82);
				fillrectangle(j * 700 + 180, i * 250 + 210, j * 700 + 600, i * 250 + 410);

				setfillcolor(BLACK);
				line(j * 700 + 300, i * 250 + 240, j * 700 + 300, i * 250 + 390);
				fillcircle(j * 700 + 300, i * 250 + 240, 5);
				fillcircle(j * 700 + 300, i * 250 + 390, 5);

				string ticket_str1 = ticket->departure;
				string ticket_str2 = ticket->destination;
				string ticket_str3 = ticket->No;
				string ticket_str4 = to_string(ticket->price);
				string ticket_str5 = str_Time(ticket->Boarding_Time, 0);
				string ticket_str6 = str_Time(ticket->Arrival_Time, 0);
				string ticket_str7 = str_During_Time(ticket->During_Time, 1);

				settextstyle(30, 0, _T("宋体"));
				TCHAR path[5] = L"行程";
				TCHAR* path_num = multiByteToWideChar(to_string(cnt));
				outtextxy(j * 700 + 35, i * 240 + 235, path);
				outtextxy(j * 700 + 95, i * 240 + 235, path_num);

				TCHAR* ticket_info1 = multiByteToWideChar(ticket_str1);
				outtextxy(j * 700 + 325, i * 250 + 225, ticket_info1);
				TCHAR* ticket_info2 = multiByteToWideChar(ticket_str2);
				outtextxy(j * 700 + 325, i * 250 + 370, ticket_info2);
				TCHAR* ticket_info3 = multiByteToWideChar(ticket_str3);
				outtextxy(j * 700 + 325, i * 250 + 280, ticket_info3);
				TCHAR* ticket_info4 = multiByteToWideChar(ticket_str4);
				TCHAR name_price[5] = L"价格：";
				outtextxy(j * 700 + 325, i * 250 + 325, name_price);
				outtextxy(j * 700 + 415, i * 250 + 325, ticket_info4);
				settextstyle(30, 0, _T("宋体"));
				TCHAR* ticket_info5 = multiByteToWideChar(ticket_str5);
				outtextxy(j * 700 + 200, i * 250 + 225, ticket_info5);
				TCHAR* ticket_info6 = multiByteToWideChar(ticket_str6);
				outtextxy(j * 700 + 200, i * 250 + 370, ticket_info6);
				TCHAR* ticket_info7 = multiByteToWideChar(ticket_str7);
				settextstyle(25, 0, _T("宋体"));
				settextcolor(0x8B8B8B);
				outtextxy(j * 700 + 200, i * 250 + 300, ticket_info7);
				settextcolor(BLACK);
				if (ticket->next_ticket)
				{
					TCHAR* change = L"停留时间：";
					TIME during;
					Time_Subtraction(ticket->next_ticket->Boarding_Time, ticket->Arrival_Time, &during);
					TCHAR* change_time = multiByteToWideChar(str_During_Time(during, 0));
					outtextxy(j * 700 + 200, i * 250 + 420, change);
					outtextxy(j * 700 + 350, i * 250 + 420, change_time);
					cnt++;
				}
				else
				{
					TCHAR* stop = L"已到达终点";
					outtextxy(j * 700 + 200, i * 250 + 420, stop);
				}

				ticket = ticket->next_ticket;
				FLAG[idx++] = 1;

				TCHAR  next[5] = L"下一页";
				TCHAR  pre[5] = L"上一页";
				TCHAR  pre_plan[10] = L"上一个方案";
				TCHAR  next_plan[10] = L"下一个方案";
				if (i == 1 && j == 1 && ticket) {
					button(1000, 730, 200, 50, next, GREEN, WHITE, WHITE);
				}
				else if (!ticket) {
					button(1000, 730, 200, 50, next_plan, GREEN, WHITE, WHITE);
				}

				if (num) {
					button(140, 730, 200, 50, pre, GREEN, WHITE, WHITE);
				}
				else if (plan_cnt) {
					button(140, 730, 200, 50, pre_plan, GREEN, WHITE, WHITE);
				}

				

				if (i == 1 && j == 1 && ticket || num) {
					FlushBatchDraw();
					int Mode = 0;
					ExMessage m;
					while (!Mode)
					{
						BeginBatchDraw();
						m = getmessage(EX_MOUSE);
						switch (m.message)
						{
						case WM_MOUSEMOVE:
							if (i == 1 && j == 1 && ticket) {
								button_move(m, 10, 1000, 730, 200, 50, next, GREEN, WHITE, WHITE, BACKGROUND);
							}
							else if (!ticket) {
								button_move(m, 10, 1000, 730, 200, 50, next_plan, GREEN, WHITE, WHITE, BACKGROUND);
							}

							if (num) {
								button_move(m, 10, 140, 730, 200, 50, pre, GREEN, WHITE, WHITE, BACKGROUND);
							}
							else if (plan_cnt) {
								button_move(m, 10, 140, 730, 200, 50, pre_plan, GREEN, WHITE, WHITE, BACKGROUND);
							}
							break;

						case WM_LBUTTONDOWN:
							if ((i == 1 && j == 1 && ticket) && button_click(m, 10, 1000, 730, 200, 50, next, GREEN, WHITE, WHITE))
							{
								Mode = 1;
								FlushBatchDraw();
								continue;
							}
							else if (!ticket && button_click(m, 10, 1000, 730, 200, 50, next_plan, GREEN, WHITE, WHITE)) 
							{
								Mode = 2;
								FlushBatchDraw();

								return 1;
							}
							else if (num && button_click(m, 10, 140, 730, 200, 50, pre, GREEN, WHITE, WHITE))
							{
								Mode = 3;
								FlushBatchDraw();

								int return_idx = idx - return_cnt - CNT[cnt_idx - 1] - 1;
								for (; return_idx <= idx; return_idx++)
									FLAG[return_idx] = 0;

								num--;
								idx = 0;
								cnt_idx = 0;
								goto RETURN_PRETABLE;
							}
							else if (!num && plan_cnt && button_click(m, 10, 140, 730, 200, 50, pre, GREEN, WHITE, WHITE)) 
							{
								Mode = 4;
								FlushBatchDraw();

								return 2;
							}
							break;
						default:
							break;
						}
					}

					num++;
				}
			}
		}
		FlushBatchDraw();
		CNT[cnt_idx++] = return_cnt;
	}

	return 0;
}

string str_Date(TIME time)
{
	string str = "";
	str = to_string(time.year) + ".";
	if (time.month < 10)
		str = str + "0";
	str = str + to_string(time.month) + ".";
	if (time.day < 10)
		str = str + "0";
	str = str + to_string(time.day);

	return str;
}

void Print_Search_Background(ROUTE route, WILL will)
{
	if (route.traffic_mode == PLANE)
		setfillcolor(0XF7D19F);
	else
		setfillcolor(0x9FF7AB);
	fillrectangle(0, 0, 1340, 200);
	setfillcolor(BACKGROUND);
	fillrectangle(0, 200, 1340, 800);

	settextstyle(50, 0, _T("宋体"));
	settextcolor(BLACK);

	TCHAR* info0[10];
	if (route.traffic_mode == PLANE)
		*info0 = L"航班行程信息：";
	else
		*info0 = L"火车行程信息：";
	outtextxy(35, 30, *info0);
	string str3 = will.departure;
	string str4 = will.destination;
	TCHAR* info3 = multiByteToWideChar(str3);
	outtextxy(400, 30, info3);
	TCHAR* info4 = multiByteToWideChar(str4);
	outtextxy(800, 30, info4);
	TCHAR info14[3] = L"至";
	outtextxy(600, 30, info14);

	settextstyle(35, 0, _T("宋体"));
	//信息栏第一行
	string str1 = str_Date(will.boarding_time);

	string str5 = str_During_Time(route.During_Time, 1);

	TCHAR* info1 = multiByteToWideChar(str1);
	outtextxy(100, 100, info1);
	TCHAR* info2 = WEEK_DAY_CHINESE[will.Week_day];
	outtextxy(400, 100, info2);
	TCHAR* info5 = multiByteToWideChar(str5);
	outtextxy(1175, 100, info5);
	TCHAR info15[5] = L"总时间：";
	outtextxy(1000, 100, info15);

	//信息栏第二行
	string str6 = to_string(route.price);
	string str7 = to_string(route.Transit_Num);
	string str8 = str_During_Time(route.During_Time_on_Route, 1);
	string str9 = str_During_Time(route.Transit_Time, 1);

	TCHAR* info6 = multiByteToWideChar(str6);
	outtextxy(215, 150, info6);
	TCHAR* info7 = multiByteToWideChar(str7);
	outtextxy(575, 150, info7);
	TCHAR* info8 = multiByteToWideChar(str8);
	outtextxy(825, 150, info8);
	TCHAR* info9 = multiByteToWideChar(str9);
	outtextxy(1175, 150, info9);

	TCHAR info10[5] = L"价格：";
	outtextxy(100, 150, info10);
	TCHAR info11[10] = L"中转次数：";
	outtextxy(400, 150, info11);
	TCHAR info12[10] = L"路程时间：";
	outtextxy(650, 150, info12);
	TCHAR info13[10] = L"中转时间：";
	outtextxy(1000, 150, info13);
}