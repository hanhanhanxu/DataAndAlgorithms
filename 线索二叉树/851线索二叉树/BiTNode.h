#pragma once
//���
template<class T>
class BiTNode
{
public:
	T data;
	BiTNode<T> *lchild;
	BiTNode<T> *rchild;
	int LTag,RTag;
};
