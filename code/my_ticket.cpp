#include "head_file.h"
#include <string.h>

//初始化票
void Init_Ticket(TICKET* pt)
{
    pt->departure = (char*)malloc(MAXLEN);
    pt->destination = (char*)malloc(MAXLEN);
    pt->No = (char*)malloc(MAXLEN);

    pt->traffic_mode = PLANE;

    for (int i = 0; i < 7; i++)
        pt->week[i] = 0;

    pt->price = 0;

    pt->Boarding_Time.year = pt->Boarding_Time.month = pt->Boarding_Time.week = pt->Boarding_Time.day = pt->Boarding_Time.hour = pt->Boarding_Time.minute = 0;
    pt->Arrival_Time.year = pt->Arrival_Time.month = pt->Arrival_Time.week = pt->Arrival_Time.day = pt->Arrival_Time.hour = pt->Arrival_Time.minute = 0;
    pt->During_Time.year = pt->During_Time.month = pt->During_Time.week = pt->During_Time.day = pt->During_Time.hour = pt->During_Time.minute = 0;

    pt->next_ticket = NULL;
}


//在票中填入排版的星期信息
void Fill_Ticket_week(TICKET* p, const int* s)
{
    for (int i = 0; i < 7; i++)
        p->week[i] = s[i];
}


//在票中填入基本信息
Status Fill_Ticket_Info(TICKET* p, const char* No, const char* departure, const char* destination, const Traffic_Mode traffic_mode, const TIME Boarding_Time, const TIME Arrival_Time, const float price)
{
    if (!p) {
        return ERROR;
    }

    p->No = (char*)malloc(MAXLEN);
    p->departure = (char*)malloc(MAXLEN);
    p->destination = (char*)malloc(MAXLEN);

    strcpy(p->No, No);
    strcpy(p->departure, departure);
    strcpy(p->destination, destination);

    p->traffic_mode = traffic_mode;

    Time_Copy(&Boarding_Time, &p->Boarding_Time);
    Time_Copy(&Arrival_Time, &p->Arrival_Time);
    Init_Time(&p->During_Time);

    // 跨过24点的During_Time需要特别处理
    if (time_A_before_time_B(p->Arrival_Time, p->Boarding_Time))  
    {
        printf("Fill_TICKET_info error: Arrival_Time is before Boarding_Time.\n");
        return ERROR;
    }
    else {
        Time_Subtraction(Arrival_Time, Boarding_Time, &p->During_Time);
    }

    p->price = price;
    p->next_ticket = NULL;

    return OK;
}


//票的复制
void Ticket_Copy(const TICKET* from, TICKET* to)
{
    strcpy(to->No, from->No);
    strcpy(to->departure, from->departure);
    strcpy(to->destination, from->destination);

    Time_Copy(&from->Arrival_Time, &to->Arrival_Time);
    Time_Copy(&from->Boarding_Time, &to->Boarding_Time);
    Time_Copy(&from->During_Time, &to->During_Time);

    Week_Copy(from->week, to->week);

    to->traffic_mode = from->traffic_mode;
    to->price = from->price;
    to->next_ticket = from->next_ticket;
}


//星期排班的复制
void Week_Copy(const int* from, int* to)
{
    for (int i = 0; i < 7; i++)
        to[i] = from[i];
}


//比较两个TICKET_INFO是否相等
int Judge_Ticket_Same(const TICKET t1, const TICKET t2)
{
    return (!strcmp(t1.No, t2.No));
}


//用于加上user的意愿出发年月日
void Add_User_Will(TICKET* pti, TICKET t)
{
    pti->Arrival_Time.year = t.Arrival_Time.year;
    pti->Arrival_Time.month = t.Arrival_Time.month;
    pti->Arrival_Time.day = t.Arrival_Time.day;

    // 跨天
    if (time_A_before_time_B(pti->Arrival_Time, pti->Boarding_Time))
        Add_one_Day(&pti->Arrival_Time);

    pti->Boarding_Time.year = t.Boarding_Time.year;
    pti->Boarding_Time.month = t.Boarding_Time.month;
    pti->Boarding_Time.day = t.Boarding_Time.day;

    return;
}