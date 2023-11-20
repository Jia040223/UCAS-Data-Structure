#include "head_file.h"

extern const char* WEEK_DAY_NAME[7];



//得到初始化的路径信息
ROUTE* Init_Route(Traffic_Mode traffic_mode)
{
    ROUTE* p = (ROUTE*)malloc(sizeof(ROUTE));
    if (!p) {
        printf("Init_Route error : Overflow.\n");
        return NULL; 
    }

    p->first_ticket = NULL;
    p->traffic_mode = traffic_mode;
    p->Transit_Num = 0;
    p->price = 0;

    Init_Time(&p->Boarding_Time);
    Init_Time(&p->Arrival_Time);
    Init_Time(&p->During_Time);
    Init_Time(&p->During_Time_on_Route);
    Init_Time(&p->Transit_Time);

    return p;
}



//从TICKET_QUEUE中创建Route
ROUTE* Creat_Route_from_Ticket_Queue(TICKET_QUEUE Q, WILL user_will)
{
    if (Ticket_Queue_Empty(Q)) {
        return NULL;
    }

    ROUTE* r = Init_Route(Q.ticket[Q.front].traffic_mode);

    if (!r)
    {
        printf("Creat_Route_from_Ticket_Queue error : Overflow.\n");
        return NULL;
    }


    //构造r的ticket信息
    TICKET t_info = Dequeue_Ticket_from_front(&Q);

    r->first_ticket = (TICKET*)malloc(sizeof(TICKET));
    Fill_Ticket_Info(r->first_ticket, t_info.No, t_info.departure, t_info.destination, t_info.traffic_mode, t_info.Boarding_Time, t_info.Arrival_Time, t_info.price);
    Fill_Ticket_week(r->first_ticket, t_info.week);

    TICKET* pti = r->first_ticket;

    while (!Ticket_Queue_Empty(Q))
    {
        r->Transit_Num++;
        r->price += t_info.price;
        Time_Addition(t_info.During_Time, r->During_Time_on_Route, &r->During_Time_on_Route);

        TIME last_Arrival_time = t_info.Arrival_Time;
        last_Arrival_time.day = 0;
        t_info = Dequeue_Ticket_from_front(&Q);
        TIME* temp = new TIME;
        Init_Time(temp);

        Time_Subtraction(t_info.Boarding_Time, last_Arrival_time, temp);
        Time_Addition(*temp, r->Transit_Time, &r->Transit_Time);
        delete temp;

        pti->next_ticket = (TICKET*)malloc(sizeof(TICKET));
        Fill_Ticket_Info(pti->next_ticket, t_info.No, t_info.departure, t_info.destination, t_info.traffic_mode, t_info.Boarding_Time, t_info.Arrival_Time, t_info.price);
        Fill_Ticket_week(pti->next_ticket, t_info.week);

        pti = pti->next_ticket;
    }
    r->price += t_info.price;
    Time_Addition(t_info.During_Time, r->During_Time_on_Route, &r->During_Time_on_Route);

    pti = r->first_ticket;
    
    //构造r的其它信息
    pti = r->first_ticket;
    while (pti->next_ticket) {
        pti = pti->next_ticket;
    }

    Time_Copy(&r->first_ticket->Boarding_Time, &r->Boarding_Time);  //出发时间等于第一张票的出发时间，到达时间等于最后一张票的到达时间
    Time_Copy(&pti->Arrival_Time, &r->Arrival_Time);

    Time_Addition(r->During_Time_on_Route, r->Transit_Time, &r->During_Time);

    return r;
}

