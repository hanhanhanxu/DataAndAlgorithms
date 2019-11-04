#pragma once
#include"BiTNode.h"
#include"Queue.h"
template<class T,class B>
class ClueBiT
{
public:
	bool CreateSuc(Queue<B> Q);
	void Create(BiTNode<T> **pT,Queue<B> &Q);
	bool CreateBiT(Queue<B> Q);
	void InThreading(BiTNode<T>* p);//线索化二叉树
	void InOrderThreading(BiTNode<T>* Thrt);
	void InOrderTraverse_Thr(BiTNode<T>* Thrt,Queue<B> &Q);
private:
	BiTNode<T> *BT;
	BiTNode<T> *pre;//前驱结点
};

template<class T,class B>
bool ClueBiT<T,B>::CreateSuc(Queue<B> Q)
{
	B e;
	int a=0,b=0;
	if(Q.NextQueue())//第一个是空
		return false;
	while(Q.PopQueue(e)==true)
	{
		if(32==e)//a是空格数目
			a++;
		else
			b++;
	}//	cout<<"a:"<<a<<"b:"<<b<<endl;
	if(a!=b+1)
		return false;//不能构成二叉树
	return true;
}

template<class T,class B>
void ClueBiT<T,B>::Create(BiTNode<T>** pT,Queue<B> &Q)
{
	B e;
	Q.PopQueue(e);
	if(32==e)
		*pT=NULL;
	else{
		*pT = (BiTNode<B>*)malloc(sizeof(BiTNode<B>));
		(*pT)->data=e;
		(*pT)->LTag=(*pT)->RTag=0;
		Create(&((*pT)->lchild),Q);
		Create(&((*pT)->rchild),Q);
	}
}

template<class T,class B>
bool ClueBiT<T,B>::CreateBiT(Queue<B> Q)
{
	if( CreateSuc(Q)!=true )
	return false;
	Create(&BT,Q);
	return true;
}

template<class T,class B>
void ClueBiT<T,B>::InThreading(BiTNode<T>* p)//中序线索化二叉树
{
	if(p)
	{
		InThreading(p->lchild);
		if(!p->lchild)
		{
			p->LTag=1;
			p->lchild=pre;
		}
		if(!pre->rchild)
		{
			pre->RTag=1;
			pre->rchild=p;
		}
		pre=p;
		cout<<pre->data<<endl;
		InThreading(p->rchild);
	}
}
template<class T,class B>
void ClueBiT<T,B>::InOrderThreading(BiTNode<T>* Thrt)
{
	Thrt->RTag=1;
	Thrt->rchild=Thrt;
	Thrt->LTag=0;
	if(BT==NULL)
	Thrt->rchild=Thrt;
	else
	{
		Thrt->lchild=BT;
		pre=Thrt;
		InThreading(BT);
		pre->RTag=1;
		pre->rchild=Thrt;
		Thrt->rchild=pre;
	}
}
template<class T,class B>
void ClueBiT<T,B>::InOrderTraverse_Thr(BiTNode<T>* Thrt,Queue<B> &Q)//中序遍历中序线索化二叉树
{
	BiTNode<T> *p=Thrt->lchild;
	while(p!=Thrt)
	{
		while(p->LTag==0)
			p=p->lchild;
		//cout<<p->data;
		Q.PushQueue(p->data);
		while(p->RTag==1 && p->rchild!=Thrt)
		{
			p=p->rchild;
			//cout<<p->data;
			Q.PushQueue(p->data);
		}
		p=p->rchild;
	}
}
