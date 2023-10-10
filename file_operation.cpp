#include "head_file.h"
/*
Status Operate_Ticket_from_File(CITY_GRAPH *G, const char *file_name);
Status Add_Ticket_from_File(CITY_GRAPH *G, FILE *fp, TICKET *ticket, int *flag);
Status Delete_Ticket_from_File(CITY_GRAPH *G, FILE *fp, TICKET *ticket, int *flag);
*/

/*
E:\Program-C\project\traffic\test.txt
*/
Status Operate_Ticket_from_File(CITY_GRAPH *G, const char *file_name)
{
    char c;
    CITY_GRAPH Origin = *G;

    FILE *fp = fopen(file_name, "r");
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
            Insert_Ticket (G, ticket);
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

Status Add_Ticket_from_File(CITY_GRAPH *G, FILE *fp, TICKET *ticket, int *flag)
{
    char c;
    int i;

    i = 0;
    while (fscanf(fp, "%c", &c) != EOF && c != ' ')
        ticket->departure[i++] = c;
    ticket->departure[i] = '\0';
    if (c == EOF) 
        return ERROR;
    
    i = 0;
    while (fscanf(fp, "%c", &c) != EOF && c != ' ')
        ticket->destination[i++] = c;
    ticket->destination[i] = '\0';
    if (c == EOF) 
        return ERROR;
    
    fscanf(fp, "%c", &c);
    if (c == 'p')
        ticket->traffic_mode = PLANE;
    else if (c == 't')
        ticket->traffic_mode = TRAIN;
    else
    {
        printf("ERROR: TRAFFIC MODE PART SHOULD BE 'p' or 't'!");
        return ERROR;
    }
    if (fscanf(fp, "%c", &c) == EOF)
        return ERROR;

    i = 0;
    while (fscanf(fp, "%c", &c) != EOF && c != ' ')
        ticket->No[i++] = c;
    ticket->No[i++] = '\0';
    if (c == EOF) 
        return ERROR;

    while (fscanf(fp, "%c", &c) != EOF && c != ':' && isdigit(c))
        ticket->Boarding_Time.hour = ticket->Boarding_Time.hour * 10 + c - '0';
    if (c == EOF || (c != ':' && !isdigit(c)) || ticket->Boarding_Time.hour < 0 || ticket->Boarding_Time.hour >= 24)
    {
        printf("ERROR: WRONG FORMAT OF HOUR (BOARDING TIME)!");
        return ERROR;
    }
    while (fscanf(fp, "%c", &c) != EOF && c != ' ' && isdigit(c))
        ticket->Boarding_Time.minute = ticket->Boarding_Time.minute * 10 + c - '0';
    if (c == EOF || (c != ' ' && !isdigit(c)) || ticket->Boarding_Time.minute < 0 || ticket->Boarding_Time.minute >= 60)
    {
        printf("ERROR: WRONG FORMAT OF MINUTE (BOARDING TIME)!");
        return ERROR;
    }
    while (fscanf(fp, "%c", &c) != EOF && c != ':' && isdigit(c))
        ticket->During_Time.hour = ticket->During_Time.hour * 10 + c - '0';
    if (c == EOF || (c != ':' && !isdigit(c)) || ticket->During_Time.hour < 0)
    {
        printf("ERROR: WRONG FORMAT OF HOUR (DURING TIME)!");
        return ERROR;
    }
    while (fscanf(fp, "%c", &c) != EOF && c != ' ' && isdigit(c))
        ticket->During_Time.minute = ticket->During_Time.minute * 10 + c - '0';
    if (c == EOF || (c != ' ' && !isdigit(c)) || ticket->During_Time.minute < 0 || ticket->During_Time.minute >= 60)
    {
        printf("ERROR: WRONG FORMAT OF MINUTE (DURING TIME)!\n");
        return ERROR;
    }

    Time_Addition(ticket->Boarding_Time, ticket->During_Time, &ticket->Arrival_Time);

    while (fscanf(fp, "%c", &c) != EOF && c != ' ' && c >= '1' && c <= '7')
        ticket->week[c - '0' - 1] = 1;
    if ((c < '1' && c != ' ') || (c > '7' && c != ' ')|| c == EOF)
    {
        printf("ERROR: WRONG FORMAT OF SCEDULE!\n");
        return ERROR;
    }

    while (fscanf(fp, "%c", &c) != EOF && c != '\n' && isdigit(c))
        ticket->price = ticket->price * 10 + c - '0';
    if (c != EOF && c != '\n' && !isdigit(c))
    {
        printf("ERROR: WRONG FORMAT OF PRICE!\n");
        return ERROR;
    }
    if (c == '\n')
        *flag = 1;
    else if (c == EOF)
        *flag = 0;
    return OK;
}

Status Delete_Ticket_from_File(CITY_GRAPH *G, FILE *fp, TICKET *ticket, int *flag)
{
    char c;
    int i;

    i = 0;
    while (fscanf(fp, "%c", &c) != EOF && c != ' ')
        ticket->departure[i++] = c;
    ticket->departure[i] = '\0';
    if (c == EOF) 
        return ERROR;
    
    i = 0;
    while (fscanf(fp, "%c", &c) != EOF && c != ' ')
        ticket->destination[i++] = c;
    ticket->destination[i] = '\0';
    if (c == EOF) 
        return ERROR;
    
    fscanf(fp, "%c", &c);
    if (c == 'p')
        ticket->traffic_mode = PLANE;
    else if (c == 't')
        ticket->traffic_mode = TRAIN;
    else
    {
        printf("ERROR: TRAFFIC MODE PART SHOULD BE 'p' or 't'!");
        return ERROR;
    }

    i = 0;
    while (fscanf(fp, "%c", &c) != EOF && c != '\n')
        ticket->No[i++] = c;
    ticket->No[i++] = '\0';
    
    if (c == '\n')
        *flag = 1;
    else if (c == EOF)
        *flag = 0;
    return OK;
}