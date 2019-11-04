
#pragma once
#include"AMGraph.h"
//顺序循环队列
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
bool EnterQueue(Queue &Q,int e)//e是坐标
{
	if( (Q.rear+1)%MVNum==Q.front )//队列满
		return false;
	Q.data[Q.rear] = e;
	Q.rear = (Q.rear+1)%MVNum;
	return true;
}
bool DelQueue(Queue &Q,int &e)//e是坐标
{
	if( Q.front==Q.rear )//队列空
		return false;
	e = Q.data[Q.front];
	Q.front = (Q.front+1)%MVNum;
	return true;
}