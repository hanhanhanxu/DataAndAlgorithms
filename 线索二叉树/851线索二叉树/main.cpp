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
	cout<<"以先序遍历的方式输入二叉树(空结点输入空格，结束时输入'#'):\n";
	while((ele=getchar())!='#')//输入
	{
		getchar();//吸收换行
		if( Q.PushQueue(ele)!=true )
		cout<<"error : PushQueue , Queue full !";
	}
	if(CiT.CreateBiT(Q)==true)//创建
	{
		CiT.InOrderThreading(Thrt);//线索化并连接到Thrt上
		CiT.InOrderTraverse_Thr(Thrt,Q1);//中序遍历线索二叉树
		cout<<"中序遍历输出:\n";
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