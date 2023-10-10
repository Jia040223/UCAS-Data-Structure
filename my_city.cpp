#include "head_file.h"

//��ʼ������֮��ı�
CITY_EDGE* Init_CITY_EDGE()
{
    CITY_EDGE* p = (CITY_EDGE*)malloc(sizeof(CITY_EDGE));
    if (!p) {
        printf("Init_CITY_EDGE error : Overflow.\n");
        return NULL;
    }

    p->first_plane_ticket = p->first_train_ticket = NULL;

    p->departure = (char*)malloc(MAXLEN);
    p->destination = (char*)malloc(MAXLEN);

    p->next_edge = NULL;

    return p;
}


//��ʼ�����нڵ�
void init_CITY_VERTEX(CITY_VERTEX* p)
{
    p->exist = 0;
    p->name = NULL;
    p->first_edge = NULL;
}


//ͨ���������֣���ʼ�����нڵ�
CITY_VERTEX* get_CITY_VERTEX(const char* name, const int exist)
{
    CITY_VERTEX* p = (CITY_VERTEX*)malloc(sizeof(CITY_VERTEX));
    if (!p) { 
        printf("get_CITY_VERTEX error : Overflow.\n"); 
        return NULL; 
    }

    p->name = (char*)malloc(MAXLEN);
    if (!p->name) {
        printf("get_CITY_VERTEX error : get name failed, Overflow\n");
        return NULL;
    }

    strcpy(p->name, name);
    p->exist = exist;
    p->if_destination = 0;
    p->first_edge = NULL;

    return p;
}


//һ���򵥵��ַ�����ϣ���������������ַ���hash�������ڽӱ��е���������������̽������ϣ��ͻ (����Ϊ�������ַ���s)
unsigned CITY_VERTEX_to_index(const char* s)
{
    static char* hash_record[MAXCITY];
    static int existed[MAXCITY] = { 0 };
    static int initialization_finished = 0;

    if (!initialization_finished)
    {
        for (int i = 0; i < MAXCITY; i++)
            hash_record[i] = (char*)malloc(MAXLEN);
        initialization_finished = 1;
    }

    unsigned idx = 0, i = 0;

    while (s[i])
        idx = (idx * 43 + s[i++]) % 101;

    while (existed[idx] && strcmp(hash_record[idx], s))
        idx = (idx + 1) % 101;

    if (!strcmp(hash_record[idx], s)) return idx;
    else {
        existed[idx] = 1;
        strcpy(hash_record[idx], s);
        return idx;
    }
}


//���нڵ�ĸ���
void CITY_VERTEX_Copy(const CITY_VERTEX* from, CITY_VERTEX* to)
{
    to->exist = from->exist;
    strcpy(to->name, from->name);
    to->first_edge = from->first_edge;
}


//�ж��������е������Ƿ���ͬ
int Judge_City_Name_Same(const char* c1, const char* c2)
{
    return (!strcmp(c1, c2));
}