#include "head_file.h"
/*
Status Print_Graph (CITY_GRAPH G);
Status Print_Edge (CITY_VERTEX v);
Status Print_Ticket(TICKET ticket);
Status Print_Clock(TIME time, int flag);

*/

extern const char *WEEK_DAY_NAME[7];

Status Print_Graph (CITY_GRAPH G)
{
    for (int i = 0; i < MAXCITY; i++)
        if (G.city[i].exist && G.city[i].first_edge)
            Print_Edge(G.city[i]);

    return OK;
}

Status Print_Edge (CITY_VERTEX v)
{
    CITY_EDGE *edge;

    printf("The Route from %s:\n", v.name);
    edge = v.first_edge;
    if (!edge)
    {
        printf("There's no route from %s\n", v.name);
        return ERROR;
    }

    printf("Air Route:\n");
    while (edge)
    {
        TICKET *plane = edge->first_plane_ticket;
        while (plane)
        {
            Print_Ticket(*plane);
            plane = plane->next_ticket;
            printf("\n");
        }
        edge = edge->next_edge;
    }

    edge = v.first_edge;
    printf("Rail Route:\n");

    while (edge)
    {
        TICKET *train = edge->first_train_ticket;
        while (train)
        {
            Print_Ticket(*train);
            train = train->next_ticket;
            printf("\n");
        }
        edge = edge->next_edge;
    }

    return OK;
}

Status Print_Ticket(TICKET ticket)
{
    Print_Ticket_Line();
    if (ticket.traffic_mode == PLANE)
        printf("PLANE ");
    else
        printf("TRAIN ");
    printf("No.: %s\n", ticket.No);
    printf("\t%-16s", ticket.departure);
    printf("----->");
    printf("\t%-16s\n", ticket.destination);
    printf("\t");
    Print_Clock(ticket.Boarding_Time, 0);
    printf("\t\t");
    Print_Clock(ticket.During_Time, 1);
    printf("\t");
    Print_Clock(ticket.Arrival_Time, 0);
    TIME day;
    if (ticket.Arrival_Time.day)
        printf("(+%d)", ticket.Arrival_Time.day);
    printf("\n\n\tPRICE: %d\n", ticket.price);
    printf("\tSCHEDULE: ");
    for(int i = 0; i < 7; i++)
        if (ticket.week[i])
            printf("%s ", WEEK_DAY_NAME[i]);
    printf("\n");
    Print_Ticket_Line();
    return OK;
}

Status Print_Route(ROUTE route)
{
    TICKET *ticket = route.first_ticket;
    while (ticket)
    {
        Print_Ticket(*ticket);
        ticket = ticket->next_ticket;
    }
    printf("\n");
    Print_Ticket_Line();
    printf("Statics:\n");
    printf("\tBoarding Time: ");
    Print_Clock(route.Boarding_Time, 0);
    printf("\n\tDuration Time: ");
    Print_Clock(route.During_Time, 1);
    printf("\n\tArrival Time: ");
    Print_Clock(route.Arrival_Time, 0);
    printf("\n\tTotal Time on Board: ");
    Print_Clock(route.During_Time_on_Route, 1);
    printf("\n\tTotal Time spent on Transitting: ");
    Print_Clock(route.Transit_Time, 1);
    printf("\n\tTransfer Times: %d", route.Transit_Num);
    printf("\n\tPrice: %d\n", route.price);
    Print_Ticket_Line();

    return 0;
}

Status Print_Clock(TIME time, int flag)
{
    int hour = time.hour;
    if(flag)
        hour += time.day * 24;

    printf("%02d:%02d", hour, time.minute);

    return 0;
}

void Print_Line()
{
    printf("##################################################\n");
}

void Print_Ticket_Line()
{
    printf("===================================================\n");
}