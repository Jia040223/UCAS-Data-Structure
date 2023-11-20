#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <easyx.h>
#include <string.h>
#include <string>
#include <conio.h>
#include <windows.h>

#define OK 1
#define ERROR 0
#define MAXLEN 100
#define MAXFLAG 200
#define MAXCITY 101
#define MAXROUTE 50
#define MAXQUEUE 100
#define BUFFERSIZE 1024
#define BACKGROUND 0xDCF8FF

using namespace std;

typedef int Status;
typedef enum { TRAIN, PLANE } Traffic_Mode;
typedef enum { Zero_Month, Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec } Month;
typedef enum { Zero_Week, Mon, Tue, Wes, Thu, Fri, Sat, Sun } Week;
typedef enum { Time, Price, Direct } Priority;

typedef struct TIME
{
	int year;
	int month;
	int week;
	int day;
	int hour;
	int minute;
} TIME;


typedef struct TICKET
{
	Traffic_Mode traffic_mode;
	char* departure;             //起点
	char* destination;          //终点
	char* No;                   //航班/火车号
	TIME Boarding_Time;         //出发时间
	TIME Arrival_Time;          //到达时间
	TIME During_Time;
	int week[7];
	int price;                  //票价
	struct TICKET* next_ticket;
} TICKET;


typedef struct TICKET_QUEUE
{
	TICKET ticket[MAXQUEUE];
	unsigned front;
	unsigned rear;
} TICKET_QUEUE;


typedef struct WILL
{
	Traffic_Mode traffic_mode;
	char* departure;             //起点
	char* destination;          //终点
	Priority priority;
	TIME boarding_time;
	int Week_day;
} WILL;

typedef struct CITY_EDGE
{
	char* departure;
	char* destination;
	TICKET* first_plane_ticket;
	TICKET* first_train_ticket;
	struct CITY_EDGE* next_edge;
} CITY_EDGE;

typedef struct CITY_VERTEX
{
	int exist;
	int if_destination;
	char* name;
	CITY_EDGE* first_edge;
} CITY_VERTEX;

typedef struct CITY_GRAPH
{
	CITY_VERTEX city[MAXCITY];
	int city_num;
	int exist[MAXCITY];
} CITY_GRAPH;


typedef struct {
	TICKET* first_ticket;                // 指向该路径的第一段旅程的票价信息
	Traffic_Mode traffic_mode;           // 交通方式
	TIME Boarding_Time;                  // 出发时间
	TIME Arrival_Time;                   // 到达时间
	TIME During_Time;                    // 整个行程的时长
	TIME During_Time_on_Route;           // 在路上的时长
	TIME Transit_Time;                   // 中转耗时
	int Transit_Num;                    // 中转的次数
	int price;                           // 全程总票价
} ROUTE;



void Terminal_Input();
void Print_Using_Method();
WILL User_Expectation();
Status Add_Ticket_From_Input(CITY_GRAPH* G);
Status Delete_Ticket_from_Input(CITY_GRAPH* G);
Status Get_Expectation_Time(TIME* t);
Status Get_Ticket_Time(TIME* t, int flag);
Status Get_Ticket_File(CITY_GRAPH* G);
Status StrComp(char s[], const char t[]);
Status lowercase_string(char* s);

Status Operate_Ticket_from_File(CITY_GRAPH* G, const char* file_name);
Status Add_Ticket_from_File(CITY_GRAPH* G, FILE* fp, TICKET* ticket, int* flag);
Status Delete_Ticket_from_File(CITY_GRAPH* G, FILE* fp, TICKET* ticket, int* flag);

Status Print_Graph(CITY_GRAPH G);
Status Print_Edge(CITY_VERTEX v);
Status Print_Ticket(TICKET ticket);
Status Print_Clock(TIME time, int flag);
Status Print_Route(ROUTE route);
void Print_Line();
void Print_Ticket_Line();


void Init_Ticket(TICKET* pt);
void Fill_Ticket_week(TICKET* p, const int* s);
Status Fill_Ticket_Info(TICKET* p, const char* No, const char* departure, const char* destination, const Traffic_Mode traffic_mode, const TIME Boarding_Time, const TIME Arrival_Time, const float price);
void Ticket_Copy(const TICKET* from, TICKET* to);
void Week_Copy(const int* from, int* to);
int Judge_Ticket_Same(const TICKET t1, const TICKET t2);
void Add_User_Will(TICKET* pti, TICKET t);


void Init_Time(TIME* p);
Status time_A_before_time_B(TIME A, TIME B);
Status time_A_before_time_B_for_search(TIME A, TIME B);
Status is_leap_year(unsigned year);
void Add_one_Day(TIME* pt);
Status Time_Subtraction(TIME ta, TIME tb, TIME* ptc);
Status Time_Addition(TIME ta, TIME tb, TIME* ptc);
Week Get_Week_Day(TIME t);
void Time_Copy(const TIME* from, TIME* to);
int Judge_Time_Same(const TIME t1, const TIME t2);


CITY_EDGE* Init_CITY_EDGE();
void init_CITY_VERTEX(CITY_VERTEX* p);
CITY_VERTEX* get_CITY_VERTEX(const char* name, const int exist);
unsigned CITY_VERTEX_to_index(const char* s);
void CITY_VERTEX_Copy(const CITY_VERTEX* from, CITY_VERTEX* to);
int Judge_City_Name_Same(const char* c1, const char* c2);


void Init_CITY_GRAPH(CITY_GRAPH* pG);
void Delete_CITY_GRAPH(CITY_GRAPH* pG);
void Recount_City_Num(CITY_GRAPH* pG);
Status Insert_City(CITY_GRAPH* pG, const CITY_VERTEX city);
Status Delete_City(CITY_GRAPH* pG, const char* name);
void Update_if_destination(CITY_GRAPH* pG);
Status Insert_Ticket(CITY_GRAPH* pG, TICKET t_info);
Status Delete_Ticket(CITY_GRAPH* pG, const TICKET t_info);
TICKET* Get_Next_Ticket(const CITY_GRAPH G, const char* from, TICKET t_current);
TICKET* Get_First_Ticket(const CITY_GRAPH G, const char* from, const Traffic_Mode traffic_mode);
int Find_Route(const CITY_GRAPH G, WILL user_will, ROUTE* pR);
int Find_Route_Resursor(const CITY_GRAPH G, WILL user_will, TICKET* t_current, ROUTE* pR, int* n, TICKET_QUEUE* pQ, int* visited, int day_temp);
void Sort_Route(ROUTE* pR, int n, Priority priority);
void merge(ROUTE* a, int start, int mid, int end, int(*cmp)(ROUTE, ROUTE));
void merge_sort_up_to_down(ROUTE* a, int start, int end, int(*cmp)(ROUTE, ROUTE));
int Route_Time_Cmp(ROUTE a, ROUTE b);
int Route_Price_Cmp(ROUTE a, ROUTE b);
int Route_Transit_Cmp(ROUTE a, ROUTE b);


void Init_TICKET_Queue(TICKET_QUEUE* pQ);
TICKET Dequeue_Ticket_from_rear(TICKET_QUEUE* pQ);
void Enqueue_Ticket(TICKET_QUEUE* pQ, TICKET t);
TICKET Dequeue_Ticket_from_front(TICKET_QUEUE* pQ);
int Ticket_Queue_Full(TICKET_QUEUE Q);
int Ticket_Queue_Empty(TICKET_QUEUE Q);
int Ticket_Queue_Length(TICKET_QUEUE Q);
void Ticket_Queue_Copy(TICKET_QUEUE* from, TICKET_QUEUE* to);

ROUTE* Init_Route(Traffic_Mode traffic_mode);
ROUTE* Creat_Route_from_Ticket_Queue(TICKET_QUEUE Q, WILL user_will);

int cmd();
int GUI();

int Welcome();
int System(CITY_GRAPH* pG);

void GUI_Insert(CITY_GRAPH* pG);
void GUI_Delete(CITY_GRAPH* pG);
void GUI_Print(CITY_GRAPH pG);
void GUI_File(CITY_GRAPH* pG);
void GUI_Search(CITY_GRAPH* pG);
void GUI_Quit(CITY_GRAPH* pG);

TCHAR* multiByteToWideChar(const string& pKey);
char* TCHARToChar(TCHAR* pTchar);

Status GUI_Get_Ticket_Time(TIME* t, char* str, int flag);
Status GUI_Fill_Ticket_week(TICKET* p, const char* s);
Status GUI_Get_Traffic_Mode(TICKET* ticket, char* s);
Status GUI_Get_Price(TICKET* ticket, const char* str);
Status GUI_Get_WILL_Priority(WILL* will, char* s);
Status GUI_Get_WILL_Traffic_Mode(WILL* will, char* s);
Status GUI_Get_Expectation_Time(TIME* t, char* s);

Status button(int x, int y, int w, int h, TCHAR* text, COLORREF fill_color, COLORREF line_color, COLORREF text_color);
Status button_move(ExMessage m, int change, int x, int y, int w, int h, TCHAR* text, COLORREF fill_color, COLORREF line_color, COLORREF text_color, COLORREF back_color);
Status button_click(ExMessage m, int change, int x, int y, int w, int h, TCHAR* text, COLORREF fill_color, COLORREF line_color, COLORREF text_color);
void Print_Background(int flag);
void Print_Info(int flag);
Status GUI_Print_Finished(CITY_GRAPH* pG);
void GUI_Print_Empty(CITY_GRAPH* pG);
string str_During_Time(TIME time, int flag);



string str_Time(TIME time, int flag);

