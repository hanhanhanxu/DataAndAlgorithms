#pragma once
#define MaxSize 1001
//顺序循环队列
template<class B>
class Queue
{
public:
	void Init();//初始化
	bool PushQueue(B element);//入
	bool PopQueue(B &element);//出
	bool EmptyQueue();//空
	int LenghtQueue();
	bool NextQueue();

private:
	B data[MaxSize];
	int front;
	int rear;
};

template<class B>
void Queue<B>::Init()
{
	front = rear = 0;
}

template<class B>
bool Queue<B>::PushQueue(B element)//入
{
	if( (rear+1)%MaxSize==front )
		return false;
	this->data[this->rear] = element;
	this->rear = (this->rear + 1)%MaxSize;
	return true;
}

template<class B>
bool Queue<B>::PopQueue(B &element)//出
{
	if( front==rear )
		return false;
	element = data[front];
	front = (front + 1)%MaxSize;
	return true;
}

template<class B>
bool Queue<B>::EmptyQueue()//空
{
	if( front == rear )
		return true;
	return false;
}

template<class B>
int Queue<B>::LenghtQueue()
{
	if( rear>=front )
		return rear-front;
	return rear+MaxSize-front;
}
template<class T>
bool Queue<T>::NextQueue()
{
//	if( front = rear )
//		return false;
	if( 32 == data[front] )
		return true;//队头是空格，返回true
	return false;
}