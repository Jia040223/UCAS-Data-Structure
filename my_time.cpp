#include "head_file.h"

unsigned int month_day[13] = { INT_MAX, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const char* WEEK_DAY_NAME[7] = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };

//��ʼ��ʱ����Ϣ
void Init_Time(TIME* p)
{
    p->year = p->month = p->day = p->hour = p->minute = 0;
}


//�ж�A�Ƿ���B��ǰ��
Status time_A_before_time_B(TIME A, TIME B)
{
    Status year = (A.year == B.year);
    Status month = (A.month == B.month);
    Status day = (A.day == B.day);
    Status hour = (A.hour == B.hour);

    if (A.year < B.year) {
        return OK;
    }
    else if (year && A.month < B.month) {
        return OK;
    }
    else if (year && month && A.day < B.day) {
        return OK;
    }
    else if (year && month && day && A.hour < B.hour) {
        return OK;
    }
    else if (year && month && day && hour && A.minute < B.minute) {
        return OK;
    }
    else {
        return ERROR;
    }
}

Status time_A_before_time_B_for_search(TIME A, TIME B)
{
    Status hour = (A.hour == B.hour);

    if (A.hour < B.hour) {
        return OK;
    }
    else if (hour && A.minute < B.minute) {
        return OK;
    }
    else {
        return ERROR;
    }
}


//�ж��Ƿ�Ϊ����
Status is_leap_year(unsigned year)
{
    if (year % 400 == 0) {
        return OK;
    }
    else if (year % 100 == 0) {
        return ERROR;
    }
    else if (year % 4 == 0) {
        return OK;
    }
    else {
        return ERROR;
    }
}


//Time += 1(����һ��)
void Add_one_Day(TIME* pt)
{
    pt->day++;
    if (pt->day > month_day[pt->month])
    {   
        //�������⴦��
        if (is_leap_year(pt->year) && pt->month == Feb && pt->day == 29) return;
        if (is_leap_year(pt->year) && pt->month == Feb && pt->day == 30)
            pt->day--;

        pt->day -= month_day[pt->month];
        pt->month++;
        if (pt->month > 12)
        {
            pt->month -= 12;
            pt->year++;
        }
    }
}


//Time���
Status Time_Subtraction(TIME ta, TIME tb, TIME* ptc)
{
    ptc->minute = ta.minute - tb.minute;
    while (ptc->minute < 0) { 
        ptc->minute += 60; 
        ta.hour--; 
    }
    ptc->hour = ta.hour - tb.hour;
    while (ptc->hour < 0) {
        ptc->hour += 24; 
        ta.day--; 
    }
    ptc->day = ta.day - tb.day;
    while (ptc->day < 0) {
        ptc->day += month_day[ta.month]; 
        ta.month--; 
    }
    ptc->month = ta.month - tb.month;
    while (ptc->month < 0) {
        ptc->month += 12; 
        ta.year--; 
    }
    ptc->year = ta.year - tb.year;

    //����(taС��tb)
    if (ptc->year < 0)
    {
        //printf("Time_Subtraction error: Time subtraction result is less than 0. Failure in subtraction.\n");
        return ERROR;
    }
    if (ptc->month)
    {
        printf("Time_Subtraction error: The time subtraction result must be less than 1 month, since no train/air traffic will cost more than 1 month within China.\n");
        return ERROR;
    }
    return OK;
}


//TIme���
Status Time_Addition(TIME ta, TIME tb, TIME* ptc)
{
    ptc->year = ptc->month = ptc->day = ptc->hour = ptc->minute = 0;

    ptc->minute = ta.minute + tb.minute;
    while (ptc->minute >= 60)
    {
        ptc->minute -= 60;
        ptc->hour++;
    }

    ptc->hour += ta.hour + tb.hour;
    while (ptc->hour >= 24)
    {
        ptc->hour -= 24;
        ptc->day++;
    }

    ptc->day += ta.day + tb.day;
    ptc->month += ta.month + tb.month;
    while (ptc->day > month_day[ptc->month])
    {
        ptc->day -= month_day[ptc->month];
        if (is_leap_year(ptc->year) && ptc->month == Feb) { //�������⴦��
            ptc->day--;
        }
        ptc->month++;
    }


    while (ptc->month > 12)
    {
        ptc->month -= 12;
        ptc->year++;
    }

    ptc->year += ta.year + tb.year;

    return OK;
}


//�ж�Time��Ӧ���ڼ�
Week Get_Week_Day(TIME t)
{
    int week = 0;
    int AllDay = 0;
    int AllYearDay = 0;
    int AllMonthDay = 0;
    int i = 0;

    //�������������
    for (i = 1970;i < t.year;i++)
    {
        if (((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0))  //����
        {
            AllYearDay += 366;    //31��--7����-217  30��--4����-120  2��-29
        }
        else
        {
            AllYearDay += 365;
        }
    }

    //�����µ�������
    for (i = 1;i < t.month;i++)
    {
        if ((i == 1) || (i == 3) || (i == 5) || (i == 7) || (i == 8) || (i == 10) || (i == 12))
        {
            AllMonthDay += 31;
        }
        else if (i == 2)
        {
            if (((t.year % 4 == 0) && (t.year % 100 != 0)) || (t.year % 400 == 0))
            {
                AllMonthDay += 29;
            }
            else
            {
                AllMonthDay += 28;
            }
        }
        else
        {
            AllMonthDay += 30;
        }
    }

    //����1970-01-01����ǰʱ��������������
    AllDay = AllYearDay + AllMonthDay + t.day;

    //���㵱ǰΪ�ܼ���0Ϊ���� 1��6��Ӧ��һ������
    week = (AllDay - 4) % 7;  //1970-01-01 Ϊ��4

    return (Week)week;
}


//Time�ĸ���
void Time_Copy(const TIME* from, TIME* to)
{
    to->year = from->year;
    to->month = from->month;
    to->day = from->day;
    to->hour = from->hour;
    to->minute = from->minute;
    to->week = from->week;
}


//�Ƚ�����TIME�Ƿ����
int Judge_Time_Same(const TIME t1, const TIME t2)
{
    return (t1.year == t2.year && t1.month == t2.month && t1.day == t2.day && t1.hour == t2.hour && t1.minute == t2.minute);
}
