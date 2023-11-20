#include "head_file.h"


//初始化城市交通网
void Init_CITY_GRAPH(CITY_GRAPH* pG)
{
    for (int i = 0; i < MAXCITY; i++) {
        init_CITY_VERTEX(&pG->city[i]);
    }
    pG->city_num = 0;
}


//删除城市交通网
void Delete_CITY_GRAPH(CITY_GRAPH* pG)
{
    if (pG) {
        free(pG);
    }
}


//重新计算交通网中城市数量
void Recount_City_Num(CITY_GRAPH* pG)
{
    pG->city_num = 0;

    for (int i = 0; i < MAXCITY; i++) {
        if (pG->city[i].exist) {
            pG->city_num++;
        }
    }
}


//在交通网中插入城市节点
Status Insert_City(CITY_GRAPH* pG, const CITY_VERTEX city)
{
    unsigned idx = CITY_VERTEX_to_index(city.name);

    if (pG->city[idx].exist)
    {
        printf("Insert_City error : This city exists already. Cannot insert again.\n");
        return ERROR;
    }

    pG->city[idx].name = (char*)malloc(MAXLEN);

    CITY_VERTEX_Copy(&city, &pG->city[idx]);
    pG->city[idx].exist = 1;
    pG->exist[pG->city_num] = idx;
    pG->city_num++;

    return OK;
}


//在交通网中删除城市节点
Status Delete_City(CITY_GRAPH* pG, const char* name)
{
    unsigned idx = CITY_VERTEX_to_index(name);

    if (!pG->city[idx].exist)
    {
        printf("Delete_City error : This city does not exist. Cannot delete.\n");
        return ERROR;
    }

    pG->city[idx].exist = 0;

    CITY_EDGE* p = pG->city[idx].first_edge;
    while (p->next_edge)
    {
        CITY_EDGE* q = p;
        p = p->next_edge;
        free(q);
    }

    return OK;
}


//更新城市的是否否有路线到达的值
void Update_if_destination(CITY_GRAPH* pG)
{
    if (!pG) {
        printf("Update_if_destination error : Graph does not exist.\n");
        return;
    }

    for (int i = 0; i < MAXCITY; i++) {
        pG->city[i].if_destination = 0;
    }

    for (int i = 0; i < MAXCITY; i++) {
        if (pG->city[i].first_edge) {
            CITY_EDGE* p = pG->city[i].first_edge;
            while (p) {
                pG->city[CITY_VERTEX_to_index(p->destination)].if_destination = 1;
                p = p->next_edge;
            }
        }
    }
}


//在图的邻接表中插入新的票价信息(如果涉及新的城市，则相应添加)
Status Insert_Ticket(CITY_GRAPH* pG, TICKET t_info)
{
    if (!pG) {
        printf("Insert_Ticket error : Graph not exists.\n");
        return ERROR;
    }

    Time_Subtraction(t_info.Arrival_Time, t_info.Boarding_Time, &t_info.During_Time);

    //如果目标城市不存在，则添加该城市
    if (!pG->city[CITY_VERTEX_to_index(t_info.destination)].exist)
    {
        pG->city[CITY_VERTEX_to_index(t_info.destination)].if_destination = 1;
        pG->city[CITY_VERTEX_to_index(t_info.destination)].exist = 1;
        pG->city[CITY_VERTEX_to_index(t_info.destination)].name = (char*)malloc(MAXLEN);
        pG->exist[pG->city_num] = CITY_VERTEX_to_index(t_info.destination);
        pG->city_num++;

        strcpy(pG->city[CITY_VERTEX_to_index(t_info.destination)].name, t_info.destination);
    }

    //匹配出发城市(若图中不存在票上的出发城市的CITY或者没有从该城市出发的交通方式)
    unsigned idx = CITY_VERTEX_to_index(t_info.departure);

    CITY_EDGE* current_city_edge = NULL;


    //图中不存在票上的出发城市的CITY或者没有从该城市出发的交通方式
    if (!pG->city[idx].exist || !pG->city[idx].first_edge)
    {
        // 初始化CITY
        if (!pG->city[idx].exist) {
            pG->exist[pG->city_num] = idx;
            pG->city_num++;
        }

        pG->city[idx].exist = 1;
        pG->city[idx].name = (char*)malloc(MAXLEN);
        strcpy(pG->city[idx].name, t_info.departure);

        // 插入一个新的CITY_EDGE
        pG->city[idx].first_edge = Init_CITY_EDGE();
        current_city_edge = pG->city[idx].first_edge;

        strcpy(current_city_edge->departure, t_info.departure);
        strcpy(current_city_edge->destination, t_info.destination);
    }
    else {          // 图中存在票上的出发城市的CITY且有从该城市出发的交通线路
        current_city_edge = pG->city[idx].first_edge;


        CITY_EDGE* pre_city_edge = Init_CITY_EDGE();
        pre_city_edge->next_edge = current_city_edge;

        // pre_city_edge 始终指向current_city_edge的前一位置
        while (current_city_edge && strcmp(current_city_edge->destination, t_info.destination))
        {
            pre_city_edge = current_city_edge;
            current_city_edge = current_city_edge->next_edge;
        }

        // 未找到从出发地到目的地的CITY_EDGE,则添加
        if (!current_city_edge)
        {
            pre_city_edge->next_edge = Init_CITY_EDGE();
            strcpy(pre_city_edge->next_edge->departure, t_info.departure);
            strcpy(pre_city_edge->next_edge->destination, t_info.destination);
            current_city_edge = pre_city_edge->next_edge;
        }
        else {        // 找到了从出发地到目的地的CITY_EDGE
        // assert equal
            if (strcmp(current_city_edge->departure, t_info.departure) && strcmp(current_city_edge->destination, t_info.destination))
            {
                printf("Insert_Ticket error : Unkown Error.\n");
            }
        }
    }

    if (!current_city_edge)
    {
        printf("Insert_Ticket error : current_city_edge NULL.\n");
        return ERROR;
    }

    TICKET* current_ticket;
    switch (t_info.traffic_mode)
    {
    case PLANE:
        current_ticket = current_city_edge->first_plane_ticket;
        break;
    case TRAIN:
        current_ticket = current_city_edge->first_train_ticket;
        break;
    default:
        printf("Insert_Ticket error : Wrong Traffic_Mode.\n");
        return ERROR;
    }


    // pre_ticket 始终指向current_ticket的前一位置
    TICKET* pre_ticket = (TICKET*)malloc(sizeof(TICKET));
    pre_ticket->next_ticket = current_ticket;


    //复制t_info信息给t
    TICKET* t = (TICKET*)malloc(sizeof(TICKET));
    Fill_Ticket_Info(t, t_info.No, t_info.departure, t_info.destination, t_info.traffic_mode, t_info.Boarding_Time, t_info.Arrival_Time, t_info.price);
    Fill_Ticket_week(t, t_info.week);

    //若当前边没有ticket
    if (!current_ticket)
    {
        if (t_info.traffic_mode == PLANE) {
            current_city_edge->first_plane_ticket = t;
        }
        else if (t_info.traffic_mode == TRAIN) {
            current_city_edge->first_train_ticket = t;
        }
        printf("\nTicket %s inserted:\n", t_info.No);
        Print_Ticket(t_info);
        return OK;
    }

    //若当前边有ticket
    while (current_ticket && !time_A_before_time_B(t->Boarding_Time, current_ticket->Boarding_Time))
    {
        //No相同
        if (Judge_Ticket_Same(*current_ticket, t_info))
        {
            printf("\nWARNING: Ticket %s already exists:\n", current_ticket->No);
            Print_Ticket(*current_ticket);
            return ERROR;
        }

        pre_ticket = current_ticket;
        current_ticket = current_ticket->next_ticket;
    }

    switch (t_info.traffic_mode)
    {
    case PLANE:
        if (current_ticket == current_city_edge->first_plane_ticket)
        {
            t->next_ticket = current_ticket;
            current_city_edge->first_plane_ticket = t;
        }
        break;
    case TRAIN:
        if (current_ticket == current_city_edge->first_train_ticket)
        {
            t->next_ticket = current_ticket;
            current_city_edge->first_train_ticket = t;
        }
        break;
    default:
        printf("Insert_Ticket error : Wrong traffic_mode.\n");
        return ERROR;
    }

    pre_ticket->next_ticket = t;
    t->next_ticket = current_ticket;
    strcpy(t->No, t_info.No);

    printf("\nTicket %s inserted:\n", t->No);
    Print_Ticket(*t);

    return OK;
}



//删除的TICKET信息t_info
Status Delete_Ticket(CITY_GRAPH* pG, const TICKET t_info)
{
    //匹配出发城市
    unsigned idx = CITY_VERTEX_to_index(t_info.departure);

    CITY_VERTEX* current_city = &pG->city[idx];
    CITY_EDGE* current_city_edge = current_city->first_edge;
    CITY_EDGE* pre_city_edge = Init_CITY_EDGE();

    pre_city_edge->next_edge = current_city_edge;


    //匹配目的城市
    while (current_city_edge) {
        if (strcmp(t_info.destination, current_city_edge->destination) == 0) {
            break;
        }
        current_city_edge = current_city_edge->next_edge;
        pre_city_edge = pre_city_edge->next_edge;
    }

    if (!current_city_edge)
    {
        printf("Delete_TICKET error : Ticket info below does not exist. Cannot delete.\n");
        Print_Ticket(t_info);
        return ERROR;
    }

    TICKET* current_ticket;

    //匹配出行方式
    switch (t_info.traffic_mode)
    {
    case PLANE: {
        current_ticket = current_city_edge->first_plane_ticket;
        break;
    }
    case TRAIN: {
        current_ticket = current_city_edge->first_train_ticket;
        break;
    }
    default:
        printf("delete_TICKET error : Wrong traffic_mode. Should be PLANE or TRAIN.\n");
        return ERROR;
    }

    TICKET* pre_ticket = (TICKET*)malloc(sizeof(TICKET));
    pre_ticket->next_ticket = current_ticket;

    //匹配票
    while (current_ticket)
    {
        if (strcmp(current_ticket->No, t_info.No) == 0)
        {
            if (current_ticket == current_city_edge->first_train_ticket) {
                current_city_edge->first_train_ticket = current_ticket->next_ticket;
                break;
            }
            if (current_ticket == current_city_edge->first_plane_ticket) {
                current_city_edge->first_plane_ticket = current_ticket->next_ticket;
                break;
            }

            pre_ticket->next_ticket = current_ticket->next_ticket;

            printf("# Ticket info deleted:\n");
            Print_Ticket(*current_ticket);

            free(current_ticket);
            break;
        }
        current_ticket = current_ticket->next_ticket;
        pre_ticket = pre_ticket->next_ticket;
    }

    if (!current_ticket)
    {
        printf("Delete_TICKET error : Ticket info below does not exist. Cannot delete.\n");
        return ERROR;
    }

    //删除
    if (!current_city_edge->first_plane_ticket && !current_city_edge->first_train_ticket)
    {
        if (current_city_edge == pG->city[idx].first_edge)
        {
            pG->city[idx].first_edge = current_city_edge->next_edge;
        }
        else {
            pre_city_edge->next_edge = current_city_edge->next_edge;
        }
        free(current_city_edge);
    }

    //更新City信息
    Update_if_destination(pG);

    if (!pG->city[idx].first_edge && !pG->city[idx].if_destination)
        pG->city[idx].exist = 0;

    idx = CITY_VERTEX_to_index(t_info.destination);
    if (!pG->city[idx].first_edge && !pG->city[idx].if_destination)
        pG->city[idx].exist = 0;

    return OK;
}



//查找从该城市出发的下一个票价信息
TICKET* Get_Next_Ticket(const CITY_GRAPH G, const char* from, TICKET t_current)
{
    //当前票有下一个票指针
    if (t_current.next_ticket) {
        return t_current.next_ticket;
    }

    //当前票无下一个票指针
    CITY_EDGE* pclb = G.city[CITY_VERTEX_to_index(from)].first_edge;
    while (pclb && strcmp(t_current.destination, pclb->destination) != 0) {
        pclb = pclb->next_edge;
    }

    if (!pclb)
    {
        printf("Get_Next_Ticket error : Cannot find a city link between your starting point and destination.\n");
        return NULL;
    }

    pclb = pclb->next_edge;

    //判断pclb下有无需要的票
    switch (t_current.traffic_mode)
    {
    case PLANE:
        while (pclb && !pclb->first_plane_ticket) {
            pclb = pclb->next_edge;
        }
        if (pclb) {
            return pclb->first_plane_ticket;
        }
        break;
    case TRAIN:
        while (pclb && !pclb->first_train_ticket) {
            pclb = pclb->next_edge;
        }
        if (pclb) {
            return pclb->first_train_ticket;
        }
        break;
    default:
        printf("Get_Next_Ticket error : Wrong Traffic_Mode. Should be PLANE or TRAIN.\n");
        break;
    }

    return NULL;
}



//找到从出发城市出发的第一张票价信息（特定交通方式）
TICKET* Get_First_Ticket(const CITY_GRAPH G, const char* from, const Traffic_Mode traffic_mode)
{
    CITY_EDGE* pclb = G.city[CITY_VERTEX_to_index(from)].first_edge;
    TICKET* pti = NULL;

    //判断pclb下有无需要的票
    switch (traffic_mode)
    {
    case PLANE:
        while (pclb && !pclb->first_plane_ticket) {
            pclb = pclb->next_edge;
        }

        if (pclb) {
            pti = pclb->first_plane_ticket;
        }

        break;
    case TRAIN:
        while (pclb && !pclb->first_train_ticket) {
            pclb = pclb->next_edge;
        }

        if (pclb) {
            pti = pclb->first_train_ticket;
        }

        break;
    default:
        printf("Get_First_Ticket error : Wrong Traffic_Mode. Should be PLANE or TRAIN.\n");
        break;
    }

    return pti;
}



//给定的交通图G（邻接表）中，按照决策原则，找到所有符合条件的交通路径
int Find_Route(const CITY_GRAPH G, WILL user_will, ROUTE* pR)
{
    int num_found = 0;

    TICKET_QUEUE Q;
    Init_TICKET_Queue(&Q);

    int visited[MAXCITY];
    for (int i = 0;i < MAXCITY;i++) {
        visited[i] = 0;
    }

    visited[CITY_VERTEX_to_index(user_will.departure)] = 1;

    TICKET* pti = Get_First_Ticket(G, user_will.departure, user_will.traffic_mode);

    Find_Route_Resursor(G, user_will, pti, pR, &num_found, &Q, visited, 0);

    Sort_Route(pR, num_found, user_will.priority);

    return num_found;
}


//find_route的子函数（用于深度优先搜索）
int Find_Route_Resursor(const CITY_GRAPH G, WILL user_will, TICKET* t_current, ROUTE* pR, int* n, TICKET_QUEUE* pQ, int* visited, int day_temp)
{
    TIME current_Arrival_Time;

    while (t_current && visited[CITY_VERTEX_to_index(t_current->destination)]) {
        t_current = Get_Next_Ticket(G, t_current->departure, *t_current);
    }

    if (!t_current) {
        return 0;
    }

    visited[CITY_VERTEX_to_index(t_current->destination)] = 1;

    TIME last_Arrival_Time;
    TIME departure = { 0, 0, 0, user_will.boarding_time.hour, user_will.boarding_time.minute };

    while (t_current)
    {
        current_Arrival_Time = { 0,0,0,day_temp,0,0 };

        // 不在班期内或出发时间早于用户指定的票直接跳过
        if (!t_current->week[(user_will.Week_day + current_Arrival_Time.day - 1) % 7] || time_A_before_time_B_for_search(t_current->Boarding_Time, departure))
        {
            t_current = Get_Next_Ticket(G, t_current->departure, *t_current);
            continue;
        }

        if (pQ && !Ticket_Queue_Empty(*pQ))
        {
            last_Arrival_Time = pQ->ticket[(pQ->rear - 1) % MAXQUEUE].Arrival_Time;

            //比最后一程到达的时间早直接跳过
            if (time_A_before_time_B_for_search(t_current->Boarding_Time, last_Arrival_Time))
            {
                t_current = Get_Next_Ticket(G, t_current->departure, *t_current);
                continue;
            }
        }

        current_Arrival_Time = t_current->Boarding_Time;
        current_Arrival_Time.day = day_temp;
        Time_Addition(current_Arrival_Time, t_current->During_Time, &current_Arrival_Time);
        Enqueue_Ticket(pQ, *t_current);
        
        //找到目的地城市的路线
        if (Judge_City_Name_Same(t_current->destination, user_will.destination))
        {
            TICKET_QUEUE Qt;
            Init_TICKET_Queue(&Qt);
            Ticket_Queue_Copy(pQ, &Qt);
            *(pR + (*n)) = *(Creat_Route_from_Ticket_Queue(Qt, user_will));
            (*n)++;

            Dequeue_Ticket_from_rear(pQ);
            visited[CITY_VERTEX_to_index(t_current->destination)] = 0;
            t_current = Get_Next_Ticket(G, t_current->departure, *t_current);
        }
        else {
            TICKET* t_next = Get_First_Ticket(G, t_current->destination, user_will.traffic_mode);

            // 出发时间早于上一程的到达时间，则跳过
            // 出发日期没有排班，则跳过
            while (t_next && (time_A_before_time_B_for_search(t_next->Boarding_Time, current_Arrival_Time) || !t_next->week[(user_will.Week_day + current_Arrival_Time.day - 1) % 7])) {
                t_next = Get_Next_Ticket(G, t_next->departure, *t_next);
            }

            //递归调用，深度优先搜索
            if (t_next) {
                Find_Route_Resursor(G, user_will, t_next, pR, n, pQ, visited, current_Arrival_Time.day);
            }

            Dequeue_Ticket_from_rear(pQ);
            t_current = Get_Next_Ticket(G, t_current->departure, *t_current);
        }
    }

    return 0;
}


//按照用户给定的优先级，将所有搜索到的路径排序
void Sort_Route(ROUTE* pR, int n, Priority priority)
{
    if (priority == Time)                                               //ʱ������
    {
        merge_sort_up_to_down(pR, 0, n - 1, Route_Time_Cmp);
    }
    else if (priority == Price)                                         //�۸�����
    {
        merge_sort_up_to_down(pR, 0, n - 1, Route_Price_Cmp);
    }
    else if (priority == Direct)                                        //��ת��������
    {
        merge_sort_up_to_down(pR, 0, n - 1, Route_Transit_Cmp);
    }
    else
    {
        printf("Sort_Route error : Invalid PRIORITY. Should be ECON, SPEED or DIRECT.\n");
        return;
    }
}



//归并
void merge(ROUTE* a, int start, int mid, int end, int (*cmp)(ROUTE, ROUTE))
{
    ROUTE* tmp = (ROUTE*)malloc((end - start + 1) * sizeof(ROUTE));
    if (!tmp) {
        printf("merge error : Overflow.\n");
        return;
    }

    int i = start;
    int j = mid + 1;
    int k = 0;
    while (i <= mid && j <= end) {
        if ((*cmp)(a[i], a[j])) {
            tmp[k++] = a[i++];
        }
        else {
            tmp[k++] = a[j++];
        }
    }
    while (i <= mid) {
        tmp[k++] = a[i++];
    }
    while (j <= end) {
        tmp[k++] = a[j++];
    }

    for (i = 0; i < k; i++) {
        a[start + i] = tmp[i];
    }
    free(tmp);
}



//归并排序
void merge_sort_up_to_down(ROUTE* a, int start, int end, int (*cmp)(ROUTE, ROUTE))
{
    if (a == NULL || start >= end) {
        return;
    }

    int mid = (end + start) / 2;
    merge_sort_up_to_down(a, start, mid, cmp); // 递归排序a[start..mid]
    merge_sort_up_to_down(a, mid + 1, end, cmp); // 递归排序a[mid..end]
    merge(a, start, mid, end, cmp);
}


int Route_Time_Cmp(ROUTE a, ROUTE b) {
    return (time_A_before_time_B(a.During_Time, b.During_Time));
}


int Route_Price_Cmp(ROUTE a, ROUTE b) {
    return(a.price < b.price);
}


int Route_Transit_Cmp(ROUTE a, ROUTE b) {
    return(a.Transit_Num < b.Transit_Num);
}