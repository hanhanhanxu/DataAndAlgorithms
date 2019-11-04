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

int Min(HuffmanTree t,int n)//��������n�������Ȩֵ��С�Ľ�����
{
	int i,flag;
	int f=infinity;//fΪ���޴��ֵ
	for(i=1;i<=n;i++)
		if(t[i].weight<f && t[i].parent==0)
			f=t[i].weight,flag=i;
		t[flag].parent=1;
		return flag;
}

void Select(HuffmanTree *t,int n,int *s1,int *s2)//��n�������ѡ������Ȩֵ��С�Ľ����ţ�����s1,��С��s2��С
{
	int x;
	*s1=Min(*t,n);
	*s2=Min(*t,n);
	if((*t)[*s1].weight>(*t)[*s2].weight)//����
	{
		x=*s1;
		*s1=*s2;
		*s2=x;
	}
}

void HuffmanCoding(HuffmanTree *HT,HuffmanCode *HC,int *w,int n)//wΪn���ַ���Ȩֵ
{//�շ�����HT  �շ�������HC
	int m,i,s1,s2,start;
	int c,f;
	HuffmanTree p;
	char *cd;
	if(n<=1)
		return ;
	m=n*2-1;
	*HT=(HuffmanTree)malloc((m+1)*sizeof(HTNode));//��0����Ԫδ��  2n��
	for(p=*HT+1,i=1;i<=n;i++,p++,w++)
	{
		(*p).weight=*w;
		(*p).parent=0;
		(*p).lchild=0;
		(*p).rchild=0;
	}
	for(;i<=m;i++,p++)//��n-1����Ҷ�ӽ���˫�׽���ʼ��Ϊ0
		(*p).parent=0;
	for(i=n+1;i<=m;++i)//����ո�����
	{
		Select(HT,i-1,&s1,&s2);//��������Ȩֵ��С���������
		(*HT)[s1].parent=(*HT)[s2].parent=i;//����˫�׵�ֵΪ˫�׵��±�
		(*HT)[i].lchild=s1;//���Һ��ӵ�ֵΪ�����Һ��ӵ������±�
		(*HT)[i].rchild=s2;
		(*HT)[i].weight=(*HT)[s1].weight+(*HT)[s2].weight;//�����±�
	}
	//��Ҷ�ӽ�㵽�������ÿ���ַ��ĺշ�������
	*HC=(HuffmanCode)malloc((n+1)*sizeof(char*));
	cd=(char*)malloc(n*sizeof(char));//Ϊ�ո����������ռ�
	cd[n-1]='\0';
	//��n��Ҷ�ӽ��ĺշ�������
	for(i=1;i<=n;i++)
	{
		start=n-1;//���������λ��
		for(c=i,f=(*HT)[i].parent;f!=0;c=f,f=(*HT)[f].parent)//��Ҷ�ӽ�㵽���ڵ������
		
			if((*HT)[f].lchild==c)
				cd[--start]='0';
			else
				cd[--start]='1';
			(*HC)[i]=(char*)malloc((n-start)*sizeof(char));//Ϊ��i���ַ��������ռ�
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
	cout<<"������Ҷ�ӽ��ĸ���:";
	cin>>n;
	w=(int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++)
	{
		cout<<"�������"<<i+1<<"������Ȩֵ:";
		scanf("%d",w+i);
	}
	HuffmanCoding(&HT,&HC,w,n);
	cout<<"�ӵ�һ����ʼ:\n";
	for(i=1;i<=n;i++)
	{
		cout<<"�շ�������:";
		puts(HC[i]);
	}
	for(i=1;i<=n;i++)
		free(HC[i]);
	free(HC);
	free(HT);
	return 0;
}
