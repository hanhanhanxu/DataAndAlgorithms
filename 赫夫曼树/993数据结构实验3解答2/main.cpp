#include<iostream>
#include<cstdlib>
#include<cstring>
#define infinity 10000;
using namespace std;
typedef struct
{
	int weight;
	int parent,lchild,rchild;
}HTNode,*HuffmanTree;
typedef char** HuffmanCode;

int Min(HuffmanTree t,int n)//返回树中n个结点中权值最小的结点序号
{
	int i,flag;
	int f=infinity;//f为无限大的值
	for(i=1;i<=n;i++)
		if(t[i].weight<f && t[i].parent==0)
			f=t[i].weight,flag=i;
		t[flag].parent=1;
		return flag;
}

void Select(HuffmanTree *t,int n,int *s1,int *s2)//在n个结点中选择两个权值最小的结点序号，其中s1,最小，s2次小
{
	int x;
	*s1=Min(*t,n);
	*s2=Min(*t,n);
	if((*t)[*s1].weight>(*t)[*s2].weight)//交换
	{
		x=*s1;
		*s1=*s2;
		*s2=x;
	}
}

void HuffmanCoding(HuffmanTree *HT,HuffmanCode *HC,int *w,int n)//w为n个字符的权值
{//赫夫曼树HT  赫夫曼编码HC
	int m,i,s1,s2,start;
	int c,f;
	HuffmanTree p;
	char *cd;
	if(n<=1)
		return ;
	m=n*2-1;
	*HT=(HuffmanTree)malloc((m+1)*sizeof(HTNode));//第0个单元未用  2n个
	for(p=*HT+1,i=1;i<=n;i++,p++,w++)
	{
		(*p).weight=*w;
		(*p).parent=0;
		(*p).lchild=0;
		(*p).rchild=0;
	}
	for(;i<=m;i++,p++)//将n-1个非叶子结点的双亲结点初始化为0
		(*p).parent=0;
	for(i=n+1;i<=m;++i)//构造赫弗曼树
	{
		Select(HT,i-1,&s1,&s2);//查找树中权值最小的两个结点
		(*HT)[s1].parent=(*HT)[s2].parent=i;//孩子双亲的值为双亲的下标
		(*HT)[i].lchild=s1;//左右孩子的值为其左右孩子的数组下标
		(*HT)[i].rchild=s2;
		(*HT)[i].weight=(*HT)[s1].weight+(*HT)[s2].weight;//数组下标
	}
	//从叶子结点到根结点求每个字符的赫夫曼编码
	*HC=(HuffmanCode)malloc((n+1)*sizeof(char*));
	cd=(char*)malloc(n*sizeof(char));//为赫弗曼编码分配空间
	cd[n-1]='\0';
	//求n个叶子结点的赫夫曼编码
	for(i=1;i<=n;i++)
	{
		start=n-1;//编码结束符位置
		for(c=i,f=(*HT)[i].parent;f!=0;c=f,f=(*HT)[f].parent)//从叶子结点到根节点求编码
		
			if((*HT)[f].lchild==c)
				cd[--start]='0';
			else
				cd[--start]='1';
			(*HC)[i]=(char*)malloc((n-start)*sizeof(char));//为第i个字符编码分配空间
			strcpy((*HC)[i],&cd[start]);			
	}
	free(cd);
}
int main(void)
{
	HuffmanTree HT;
	HuffmanCode HC;
	int *w,n,i;
	double h;
	cout<<"请输入叶子结点的个数:";
	cin>>n;
	w=(int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++)
	{
		cout<<"请输入第"<<i+1<<"个结点的权值:";
		scanf("%d",w+i);
	}
	HuffmanCoding(&HT,&HC,w,n);
	cout<<"从第一个开始:\n";
	for(i=1;i<=n;i++)
	{
		cout<<"赫夫曼编码:";
		puts(HC[i]);
	}
	for(i=1;i<=n;i++)
		free(HC[i]);
	free(HC);
	free(HT);
	return 0;
}
