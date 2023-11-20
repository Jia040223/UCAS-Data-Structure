#include "head_file.h"

//初始化TICKET队列
void Init_TICKET_Queue(TICKET_QUEUE* pQ)
{
    if (!pQ) { 
        printf("Init_TICKET_Queue error : Overflow.\n"); 
        return;
    }
    pQ->front = 0;
    pQ->rear = 0;

    for (int i = 0; i < MAXQUEUE; i++)
    {
        pQ->ticket[i].next_ticket = NULL;
        pQ->ticket[i].No = (char*)malloc(MAXLEN);
        pQ->ticket[i].departure = (char*)malloc(MAXLEN);
        pQ->ticket[i].destination = (char*)malloc(MAXLEN);
        Init_Time(&pQ->ticket[i].Arrival_Time);
        Init_Time(&pQ->ticket[i].During_Time);
        Init_Time(&pQ->ticket[i].Boarding_Time);
        pQ->ticket[i].price = 0;
        for (int j = 0; j < 7; j++) {
            pQ->ticket[i].week[j] = 0;
        }
    }
}


//入队列
void Enqueue_Ticket(TICKET_QUEUE* pQ, TICKET t)
{
    if (Ticket_Queue_Full(*pQ)) {
        printf("Enqueue_Ticket error : Queue full. Cannot enqueue.\n"); 
        return; 
    }

    Ticket_Copy(&t, &pQ->ticket[pQ->rear]);
    pQ->rear = (pQ->rear + 1) % MAXQUEUE;
}


//从队列头出队列
TICKET Dequeue_Ticket_from_front(TICKET_QUEUE* pQ)
{
    if (Ticket_Queue_Empty(*pQ)) {
        printf("Dequeue_Ticket_from_front error : Queue empty. Cannot de_queue.\n"); 
    }

    TICKET t = pQ->ticket[pQ->front];
    pQ->front = (pQ->front + 1) % MAXQUEUE;
    return t;

}


//从队列尾出队列
TICKET Dequeue_Ticket_from_rear(TICKET_QUEUE* pQ)
{
    if (Ticket_Queue_Empty(*pQ)) {
        printf("Dequeue_Ticket error : Queue empty. Cannot dequeue.\n");
    }

    TICKET t = pQ->ticket[pQ->rear];
    pQ->rear = (pQ->rear - 1) % MAXQUEUE;
    return t;
}


//判断队列满
int Ticket_Queue_Full(TICKET_QUEUE Q)
{
    return ((Q.rear + 1) % MAXQUEUE == Q.front);
}


//判断队列空
int Ticket_Queue_Empty(TICKET_QUEUE Q)
{
    return (Q.rear == Q.front);
}


//返回队列元素数量
int Ticket_Queue_Length(TICKET_QUEUE Q)
{
    return (Q.rear - Q.front + MAXQUEUE) % MAXQUEUE;
}


//拷贝队列
void Ticket_Queue_Copy(TICKET_QUEUE* from, TICKET_QUEUE* to)
{
    if (!from || !to)
    {
        printf("Ticket_Queue_Copy error : Invalid TICKET_QUEUE pointer.");
        return;
    }

    for (int i = 0; i < MAXQUEUE; i++) {
        to->ticket[i] = from->ticket[i];
    }

    to->rear = from->rear;
    to->front = from->front;
}