
#pragma once
#include"AMGraph.h"
//˳��ѭ������
typedef struct
{
	int data[MVNum];
	int front , rear;
}Queue;

void InitQueue(Queue &Q)
{
	Q.front = Q.rear = 0;
}
bool EmptyQueue(Queue Q)
{
	if( Q.front==Q.rear )
		return true;
	return false;
}
bool EnterQueue(Queue &Q,int e)//e������
{
	if( (Q.rear+1)%MVNum==Q.front )//������
		return false;
	Q.data[Q.rear] = e;
	Q.rear = (Q.rear+1)%MVNum;
	return true;
}
bool DelQueue(Queue &Q,int &e)//e������
{
	if( Q.front==Q.rear )//���п�
		return false;
	e = Q.data[Q.front];
	Q.front = (Q.front+1)%MVNum;
	return true;
}