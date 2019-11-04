#include<iostream>
#include"Queue.h"
#include"BiTNode.h"
#include"ClueBiT.h"
using namespace std;

typedef char BiType;
int main(void)
{
	BiType ele;
	Queue<char> Q,Q1;
	ClueBiT<char,char> CiT;
	BiTNode<char> *Thrt;
	Thrt = (BiTNode<char>*)malloc(sizeof(BiTNode<char>));
	Q.Init();
	Q1.Init();
	cout<<"����������ķ�ʽ���������(�ս������ո񣬽���ʱ����'#'):\n";
	while((ele=getchar())!='#')//����
	{
		getchar();//���ջ���
		if( Q.PushQueue(ele)!=true )
		cout<<"error : PushQueue , Queue full !";
	}
	if(CiT.CreateBiT(Q)==true)//����
	{
		CiT.InOrderThreading(Thrt);//�����������ӵ�Thrt��
		CiT.InOrderTraverse_Thr(Thrt,Q1);//�����������������
		cout<<"����������:\n";
		while(Q1.PopQueue(ele))
			cout<<ele;
	}
	cout<<endl;
	return 0;
}
/*
A
B
D
H
 
 
I
 
 
E
 
J
 
 
C
F
 
 
G
 
 
#
*/