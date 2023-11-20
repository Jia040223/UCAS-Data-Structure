#include "head_file.h"
#include <conio.h>
#include <windows.h>
#include <string>

#define BUFFERSIZE 4096
extern unsigned int month_day[13];


//不要忘记在使用完wchar_t*后delete[]释放内存
TCHAR* multiByteToWideChar(const string& pKey)
{
	const char* pCStrKey = pKey.c_str();
	//第一次调用返回转换后的字符串长度，用于确认为wchar_t*开辟多大的内存空间
	int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
	wchar_t* pWCStrKey = new wchar_t[pSize];
	//第二次调用将单字节字符串转换成双字节字符串
	MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
	return pWCStrKey;
}


/*//String to TCHAR
TCHAR* TransForm(string s) {
	TCHAR result[BUFFERSIZE];
	char c[BUFFERSIZE];
	strcpy_s(c, s.c_str());
	MultiByteToWideChar(CP_ACP, 0, c, -1, result, BUFFERSIZE);
	return result;
}*/


//TCHAR TO CHAR
char* TCHARToChar(TCHAR* pTchar)
{	
	char* pChar = nullptr;
	int nLen = wcslen(pTchar) + 1;
	for (int i = 0; i < nLen;i++) {
		if (pTchar[i] == L'：') {
			pTchar[i] = L':';
		}
	}

	pChar = new char[nLen * 2];
	WideCharToMultiByte(CP_ACP, 0, pTchar, nLen, pChar, 2 * nLen, NULL, NULL);
	return pChar;
}


//Char* to TIME
Status GUI_Get_Ticket_Time(TIME* t, char* str, int flag)
{
	char c;
	t->year = t->month = t->day = t->hour = t->minute = 0;

	int i = 0;
	c = str[i++];
	if (!isdigit(c))
	{
		printf("Wrong Format of Time\n");
		return ERROR;
	}
	while (isdigit(c))
	{
		t->hour = t->hour * 10 + c - '0';
		c = str[i++];
	}
	if (c == ':')
		c = str[i++];
	else
	{
		printf("Wrong Format of TIME\n");
		return ERROR;
	}
	while (isdigit(c))
	{
		t->minute = t->minute * 10 + c - '0';
		c = str[i++];
	}

	if (t->hour < 0 || (flag && t->hour >= 24))
	{
		printf("Wrong Format of Hour\n");
		return ERROR;
	}
	if (t->minute < 0 || t->minute >= 60)
	{
		printf("Wrong Format of Minute\n");
		return ERROR;
	}
	return OK;
}


//Fill_Ticket_week from char*
Status GUI_Fill_Ticket_week(TICKET* p, const char* s)
{
	int i = 0;
	char c;

	while ((c = s[i++]) != '\0' && i <= 7) {
		if (isdigit(c)) {
			p->week[c - '0' - 1] = 1;
		}
		else {
			return ERROR;
		}
	}

	if (c != '\0') {
		return ERROR;
	}

	return OK;
}


//GUI_Get_Price from char*
Status GUI_Get_Price(TICKET* ticket, const char* str)
{
	ticket->price = 0;
	char c;
	int i = 0;

	while ((c = str[i++]) != '\0')
	{
		if (!isdigit(c))
		{
			printf("ERROR: Please enter number\n");
			return ERROR;
		}
		else
			ticket->price = ticket->price * 10 + c - '0';
	}

	return OK;
}


//GUI_Get_Traffic_Mode from char*
Status GUI_Get_Traffic_Mode(TICKET* ticket, char* s)
{
	lowercase_string(s);
	if (!strcmp(s, "p") || !strcmp(s, "机票")) {
		ticket->traffic_mode = PLANE;
		return OK;
	}
	else if (!strcmp(s, "t") || !strcmp(s, "火车票")) {
		ticket->traffic_mode = TRAIN;
		return OK;
	}
	else
	{
		printf("ERROR: Please enter the letter 'p' or 't' or input '机票' or '火车票'\n");
		return ERROR;
	}
}


//GUI_Get_WILL_Priority from char*
Status GUI_Get_WILL_Priority(WILL* will, char* s)
{
	if (!StrComp(s, "t"))
		will->priority = Time;
	else if (!StrComp(s, "p"))
		will->priority = Price;
	else if (!StrComp(s, "d"))
		will->priority = Direct;
	else
	{
		printf("ERROR: Please enter the letter 't', 'p' or 'd'\n");
		return ERROR;
	}

	return OK;
}


//GUI_Get_WILL_Traffic_Mode from char*
Status GUI_Get_WILL_Traffic_Mode(WILL* will, char* s)
{
	lowercase_string(s);
	if (!strcmp(s, "p") || !strcmp(s, "机票")) {
		will->traffic_mode = PLANE;
		return OK;
	}
	else if (!strcmp(s, "t") || !strcmp(s, "火车票")) {
		will->traffic_mode = TRAIN;
		return OK;
	}
	else
	{
		printf("ERROR: Please enter the letter 'p' or 't' or input '机票' or '火车票'\n");
		return ERROR;
	}
}


//Get User Expectation Time from char*
Status GUI_Get_Expectation_Time(TIME* t, char* s)
{
	char c;
	int month, day;
	t->year = t->month = t->week = t->hour = t->minute = 0;
	month = day = 0;
	int i = 0;

	c = s[i++];
	if (!isdigit(c))
	{
		printf("Wrong Format of TIME\n");
		return ERROR;
	}
	while (isdigit(c))
	{
		t->year = t->year * 10 + c - '0';
		c = s[i++];
	}
	if (c == '.')
		c = s[i++];
	else
	{
		printf("Wrong Format of TIME\n");
		return ERROR;
	}
	if (!isdigit(c))
	{
		printf("Wrong Format of TIME\n");
		return ERROR;
	}
	while (isdigit(c))
	{
		month = month * 10 + c - '0';
		c = s[i++];
	}
	if (c == '.')
		c = s[i++];
	else
	{
		printf("Wrong Format of TIME\n");
		return ERROR;
	}
	if (!isdigit(c))
	{
		printf("Wrong Format of TIME\n");
		return ERROR;
	}
	while (isdigit(c))
	{
		day = day * 10 + c - '0';
		c = s[i++];
	}
	if (month < 1 || month > 12)
	{
		printf("Wrong Format of MONTH\n");
		return ERROR;
	}
	if (day < 1 || day > month_day[month])
	{
		if (!(t->year % 4 == 0 && month == 2 && day == 29))
		{
			printf("Wrong Format of DAY\n");
			return ERROR;
		}
	}
	if (t->year < 2023 || (t->year == 2023 && month < 6) || (t->year == 2023 && month == 6 && day < 10))
	{
		printf("Time should not be earlier than 2023.6.10\n");
		return ERROR;
	}
	t->month = month;
	t->day = day;
	return OK;
}


Status GUI_Operate_Ticket_from_File(CITY_GRAPH* G, const char* file_name)
{
	char c;
	CITY_GRAPH Origin = *G;

	FILE* fp = fopen(file_name, "r");
	if (!fp)
	{
		printf("CAN'T OPEN FILE: %s\n", file_name);
		return ERROR;
	}

	while (fscanf(fp, "%c", &c) != EOF)
	{
		int flag;
		TICKET ticket;
		Init_Ticket(&ticket);
		if (c == 'a')
		{
			if (fscanf(fp, "%c", &c) == EOF)
			{
				*G = Origin;
				fclose(fp);
				return ERROR;
			}
			if (!Add_Ticket_from_File(G, fp, &ticket, &flag))
			{
				*G = Origin;
				fclose(fp);
				return ERROR;
			}
			Insert_Ticket(G, ticket);
			if (!flag)
				break;
		}
		else if (c == 'd')
		{
			if (fscanf(fp, "%c", &c) == EOF)
			{
				*G = Origin;
				fclose(fp);
				return ERROR;
			}
			if (!Delete_Ticket_from_File(G, fp, &ticket, &flag))
			{
				*G = Origin;
				fclose(fp);
				return ERROR;
			}
			Delete_Ticket(G, ticket);
			if (!flag)
				break;
		}
		else
		{
			printf("ERROR: EVERY LINE SHOULE START WITH 'a' or 'd'!\n");
			*G = Origin;
			fclose(fp);
			return ERROR;
		}
	}
	fclose(fp);
	return OK;
}