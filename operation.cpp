#include "head_file.h"

/*
void Terminal_Input();
void Print_Using_Method();
WILL User_Expectation();
Status Add_Ticket_From_Input (CITY_GRAPH *G);
Status Delete_Ticket_from_Input (CITY_GRAPH *G);
Status Get_Expectation_Time (TIME *t);
Status Get_Ticket_Time (TIME *t, int flag);
Status Get_Ticket_File (CITY_GRAPH *G);
Status StrComp (char s[], char t[]);
Status lowercase_string (char *s);
*/

extern unsigned int month_day[13];


void Terminal_Input()
{
    CITY_GRAPH G;
    char *s;

    Init_CITY_GRAPH(&G);
    Print_Using_Method();
    s = (char *)malloc(MAXLEN * sizeof(char));

    while (scanf("%s", s))
    {
        if (!strcmp(s, "a"))
        {
            if (!Add_Ticket_From_Input(&G))
                printf("ERROR: ADD NEW TICKET FAILED\n");
            else
                printf("ADD NEW TICKET SUCCESSFULLY\n");
        }
        else if (!strcmp(s, "d"))
        {
            Print_Graph(G);
            printf("CHOOSE A ROUTE TO DELETE\n");
            if (!Delete_Ticket_from_Input(&G))
                printf("ERROR: DELETE NEW TICKET FAILED\n");
            else
                printf("DELETE NEW TICKET SUCCESSFULLY\n");
        }
        else if (!strcmp(s, "t"))
        {
            if (!Get_Ticket_File(&G))
            {
                Print_Line();
                printf("ERROR: READING FILE FAILED\n");
                printf("GRAPH WILL BE RESTORED...\n");
                Print_Line();
            }
            else
                printf("READING FILE SUCCESSFULLY\n");
        }
        else if (!strcmp(s, "c"))
        {
            for (int i = 0; i < MAXCITY; i++)
                if (G.city[i].exist)
                    printf("%s\n", G.city[i].name);
            WILL will;
            int route_num;
            char c;
            ROUTE *route = (ROUTE *)malloc(MAXROUTE * sizeof(ROUTE));
            will = User_Expectation();
            route_num = Find_Route(G, will, route);
            if (route_num)
                for (int i = 0; i < route_num; i++)
                {
                    printf("\n\nPLAN %d:\n\n", i + 1);
                    Print_Route(route[i]);
                    if (i < route_num - 1)
                    {
NEXT_PLAN:
                        printf("Enter 'y'(yes) or 'n'(no) to decide whether print next plan or not\n");
                        c = getchar();
                        if (c == 'y')
                        {
                            if (c = getchar() == '\n')
                                continue;
                        }
                        else if (c == 'n')
                        {
                            if (c = getchar() == '\n')
                                break;
                        }
                        else    
                        {
                            printf("ERROR: PLEASE ENTER 'y' or 'n'!\n");
                            goto NEXT_PLAN;
                        }
                    }
                    else
                        printf("ALL THE PLAN HAS BEEN GIVEN!\n");
                }
            else 
                printf("There is no Route\n");
        }
        else if (!strcmp(s, "p"))
            Print_Graph(G);
        else if (!strcmp(s, "q"))
            break;
        else
        {
            printf("ERROR: PLEASE ENTER 'a', 'd', 't', 'c','p' or 'q'.\n");
            setbuf(stdin, NULL);
            continue;
        }
        setbuf(stdin, NULL);
        printf("Enter to continue...\n");
        getchar();
        setbuf(stdin, NULL);
        system("cls");
        Print_Using_Method();
    }
}

void Print_Using_Method()
{
    Print_Line();
    printf("Enter 'a' to add a new ticket.\n");
    printf("Enter 'd' to delete a ticket.\n");
    printf("Enter 't' to add tickets from txt file.\n");
    printf("Enter 'c' to consulte a way for journey.\n");
    printf("Enter 'p' to print the whole traffic information.\n");
    printf("Enter 'q' to quit the system.\n");
    Print_Line();
    printf("CHOOSE YOUR MODE.\n");
}

WILL User_Expectation()
{
    char s[MAXLEN];
    WILL will;
    will.departure = (char *)malloc(MAXLEN * sizeof(char));
    will.destination = (char *) malloc(MAXLEN * sizeof(char));

    printf("Enter the expectation of the journey\n");
    printf("Departure: \n");
    scanf("%s", will.departure);
    lowercase_string(will.departure);
    //exist? 小写
    printf("Destination: \n");
    scanf("%s", will.destination);
    lowercase_string(will.destination);
    //exist? 小写

    printf("PRIORITY: \n");
    printf("\tEnter 't' for time priority\n");
    printf("\tEnter 'p' for price priority\n");
    printf("\tEnter 'd' for least transfer times priority\n");
Enter_Priority:
    scanf("%s", s);
    lowercase_string(s);
    if (!StrComp(s, "t"))
        will.priority = Time;
    else if (!StrComp(s, "p"))
        will.priority = Price;
    else if (!StrComp(s, "d"))
        will.priority = Direct;
    else
    {
        printf("ERROR: Please enter the letter 't', 'p' or 'd'\n");
        goto Enter_Priority;
    }

    printf("The way to travel\n");
    printf("\tEnter 'p' to get plane tickets\n");
    printf("\tEnter 't' to get train tickets\n");
Enter_Mode:
    scanf("%s", s);
    lowercase_string(s);
    if (!StrComp(s, "p"))
        will.traffic_mode = PLANE;
    else if (!StrComp(s, "t"))
        will.traffic_mode = TRAIN;
    else
    {
        printf("ERROR: Please enter the letter 'p' or 't'\n");
        goto Enter_Mode;
    }

    printf("Depature Time:\n");
    printf("\tEnter time in the format: YYYY.MM.DD\n");
    setbuf(stdin, NULL);
    while (!Get_Expectation_Time(&will.boarding_time))
        setbuf(stdin, NULL);
    will.Week_day = Get_Week_Day(will.boarding_time);

    return will;
}

Status Add_Ticket_From_Input (CITY_GRAPH *G)
{
    if (!G)
    {
        printf("ERROR: GRAPH doesn't exist\n");
        return ERROR;
    }
    
    char *s, c;

    printf("Enter the information of the ticket\n");
    s = (char *)malloc(MAXLEN * sizeof(char));
    TICKET ticket;
    Init_Ticket(&ticket);

    printf("Departure:\n");
    scanf("%s", ticket.departure);
    printf("Destination:\n");
    scanf("%s", ticket.destination);

ENTER_MODE:
    printf("Traffic Mode:\n");
    printf("\tEnter 'p' to represent plane\n");
    printf("\tEnter 't' to represent train\n");
    scanf("%s", s);
    lowercase_string(s);
    if (!strcmp(s, "p")) ticket.traffic_mode = PLANE;
    else if (!strcmp(s, "t")) ticket.traffic_mode = TRAIN;
    else 
    {
        printf("ERROR: Please enter the letter 'p' or 't'\n");
        goto ENTER_MODE;
    }

    printf("Flight/Train Number:\n");
    scanf("%s", ticket.No);

    printf("Boarding Time:\n");
    printf("The format of time: hh:mm\n");
    setbuf(stdin, NULL);
    while (!Get_Ticket_Time(&ticket.Boarding_Time, 1))
        setbuf(stdin, NULL);

    printf("The Time that The Journey Lasts\n");
    printf("The format of time: hh:mm(hh can be any digit)\n");
    setbuf(stdin, NULL);
    while (!Get_Ticket_Time(&ticket.During_Time, 0))
        setbuf(stdin, NULL);
    Time_Addition(ticket.Boarding_Time, ticket.During_Time, &ticket.Arrival_Time);

    printf("The weekly scedule of the train/plane:\n");
    printf("Use number between 1 to 7 to represent the day of the week\n");
    printf("For example: use 67 to represent Saturday and Sunday\n");
ENTER_SCHEDULE:
    while ((c = getchar()) != '\n')
    {
        if (c < '1' | c > '7')
        {
            for (int i = 0; i < 7; i++)
                ticket.week[i] = 0;
            printf("Please enter number with digits between 1 - 7\n");
            printf("Please enter again\n");
            setbuf(stdin, NULL);
            goto ENTER_SCHEDULE;
        }
        else
            ticket.week[c - '0' - 1] = 1;
    }

    printf("Price:\n");
ENTER_TICKET_PRICE:
    ticket.price = 0;
    while ((c = getchar()) != '\n')
    {
        if (!isdigit(c))
        {
            printf("ERROR: Please enter number\n");
            goto ENTER_TICKET_PRICE;
        }
        else
            ticket.price = ticket.price * 10 + c - '0';
    }

    Insert_Ticket (G, ticket);
    return OK;
}

Status Delete_Ticket_from_Input (CITY_GRAPH *G)
{
    if (!G)
    {
        printf("ERROR: GRAPH doesn't exist\n");
        return ERROR;
    }
    char *s;

    printf("Enter the information of the ticket\n");
    s = (char *)malloc(MAXLEN * sizeof(char));
    TICKET ticket;
    Init_Ticket(&ticket);

    printf("Departure:\n");
    scanf("%s", ticket.departure);
    printf("Destination:\n");
    scanf("%s", ticket.destination);

ENTER_MODE:
    printf("Traffic Mode:\n");
    printf("\tEnter 'p' to represent plane\n");
    printf("\tEnter 't' to represent train\n");
    scanf("%s", s);
    lowercase_string(s);
    if (!strcmp(s, "p")) ticket.traffic_mode = PLANE;
    else if (!strcmp(s, "t")) ticket.traffic_mode = TRAIN;
    else 
    {
        printf("ERROR: Please enter the letter 'p' or 't'\n");
        setbuf(stdin, NULL);
        goto ENTER_MODE;
    }

    printf("Flight/Train Number:\n");
    scanf("%s", ticket.No);

    return Delete_Ticket(G, ticket);
}

Status Get_Expectation_Time (TIME *t)
{
    char c;
    int month, day;
    t->year = t->month = t->week = t->hour = t->minute = 0;
    month = day = 0;

    c = getchar();
    if(!isdigit(c))
    {
        printf("Wrong Format of TIME\n");
        return ERROR;
    }
    while (isdigit(c))
    {
        t->year = t->year * 10 + c - '0';
        c = getchar();
    }
    if (c == '.')
        c = getchar();
    else
    {
        printf("Wrong Format of TIME\n");
        return ERROR;
    }
    if(!isdigit(c))
    {
        printf("Wrong Format of TIME\n");
        return ERROR;
    }
    while (isdigit(c))
    {
        month = month * 10 + c - '0';
        c = getchar();
    }
    if (c == '.')
        c = getchar();
    else
    {
        printf("Wrong Format of TIME\n");
        return ERROR;
    }
    if(!isdigit(c))
    {
        printf("Wrong Format of TIME\n");
        return ERROR;
    }
    while (isdigit(c))
    {
        day = day * 10 + c - '0';
        c = getchar();
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

Status Get_Ticket_Time (TIME *t, int flag)
{
    char c;
    t->year = t->month = t->day = t->hour = t->minute = 0;
    
    c = getchar();
    if (!isdigit(c))
    {
        printf("Wrong Format of Time\n");
        return ERROR;
    }
    while (isdigit(c))
    {
        t->hour = t->hour * 10 + c - '0';
        c = getchar();
    }
    if (c == ':')
        c = getchar();
    else
    {
        printf("Wrong Format of TIME\n");
        return ERROR;
    }
    while (isdigit(c))
    {
        t->minute = t->minute * 10 + c - '0';
        c = getchar();
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

Status Get_Ticket_File (CITY_GRAPH *G)
{
    char *s, c;
    int i = 0;

    if (!G)
    {
        printf("ERROR: GRAPH DOESN'T EXIST!\n");
        return ERROR;
    }
    printf("ENTER TXT FILE NAME (end with .txt):\n");
    s = (char *)malloc(MAXLEN * sizeof(char));
    setbuf(stdin, NULL);
    while((c = getchar()) != '\n')
        s[i++] = c;
    s[i] = '\0';

    printf("READING %s...\n", s);

    if (!Operate_Ticket_from_File(G, s))
        return ERROR;
    return OK;
}

Status StrComp (char s[], const char t[])
{
    for (int i = 0; i < strlen(s) && i < strlen(t); i++)
        if (s[i] != t[i])
            return 1;
    return 0;
}

Status lowercase_string (char *s)
{
    for (int i = 0; i < strlen(s); i++)
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] - 'A' + 'a';
    return OK;
}