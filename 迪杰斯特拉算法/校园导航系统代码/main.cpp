#include<iostream>
#include<string>
#include"string.h"
#include"sqlite3.h"
#pragma comment(lib,"sqlite3.lib")

sqlite3 *db;

#define MVNum 100
int prev[MVNum][MVNum] ;
int dist[MVNum] = {0};
#include"AMGraph.h"
#include"Status.h"
using namespace std;

AMGraph G;//��ͼ�Ĵ���

void point1()//��ӵ�ͼ
{
	char **dbResult;
	int nRow,nColumn;
	int rc;
	char sql[200];
	char *errmsg;//������Ϣ��
	char tabname[30],tabnamea[30],tabnameb[30];//����


	cout<<"�������ͼ���� : ";
	cin>>tabname;//��ͼ��
	//�Ƚ�
	sprintf(sql,"SELECT name FROM sqlite_master WHERE type='table'  ");//sqlite3�鿴���б�ֻ��ʾ���� ����.table�ķ���
	rc = sqlite3_get_table(db,sql,&dbResult,&nRow,&nColumn,&errmsg);
	if( rc != SQLITE_OK )
	{
		cout<<"error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
	}
	while(nRow--)
	{
		if(strcmp(tabname,dbResult[nRow])==0)//��ȣ������ظ�
		{
			cout<<"�����ظ�������ʧ��!(��ʾ��Ϊ�����ظ����ڱ��������1,2������)\n";
			return ;
		}
	}
	strcpy(tabnamea,tabname);
	strcat(tabnamea,"a");
	strcpy(tabnameb,tabname);
	strcat(tabnameb,"b");
	
	//����׶�  tabname tabnamea tabnameb
	sprintf(sql , "CREATE TABLE %s(mnum int,dnum int)" , tabname );//������  mnum �ص���Ŀ dnum �ߵ���Ŀ
	rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(rc != SQLITE_OK)
	{
		cout<<"SQL error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
		return ;
	}
	sprintf(sql , "CREATE TABLE %s(name varchar(20))" , tabnamea );//������
	rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(rc != SQLITE_OK)
	{
		cout<<"SQL error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
		return ;
	}
	sprintf(sql , "CREATE TABLE %s(cname varchar(20),pname varchar(20),distan double)" , tabnameb );//�ص���ص�֮�����ϵ�����
	rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(rc !=SQLITE_OK)
	{
		cout<<"SQL error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
		return ;
	}

	//�����ݵ������ݿ�
	int n,m,i=0,j=0;
	double weight;
	char sdname[20],scname[20],spname[20];
	cout<<"������ص��� : ";
	while(scanf("%d",&n)!=1)
	{
		getchar();
		cout<<"�������!\n��������� : ";
	}
	cout<<"������·���� : ";
	while(scanf("%d",&m)!=1)
	{
		getchar();
		cout<<"�������!\n��������� : ";
	}
	sprintf(sql , "INSERT INTO %s VALUES(%d,%d)",tabname,n,m);
	rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(rc != SQLITE_OK)
	{
		cout<<"SQL error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
	}

	while(i<n)
	{
		cout<<"������ص��� : ";
		cin>>sdname;
		sprintf(sql , "INSERT INTO %s VALUES('%s')",tabnamea,sdname);//tabnamea
		rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
		if(rc != SQLITE_OK)
		{
			cout<<"SQL error : "<<errmsg<<endl;
			sqlite3_free(errmsg);
		}
		i++;
	}
	while(j<m)
	{
		cout<<"��������ͨ·�������ص㼰֮���·������:\n";
		cout<<"�ص�1 : ";
		cin>>scname;
		cout<<"�ص�2 : ";
		cin>>spname;
		cout<<"·������(��λ/��) : ";
		cin>>weight;
		sprintf(sql , "INSERT INTO %s VALUES('%s','%s',%lf)",tabnameb,scname,spname,weight);//tabnameb
		rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
		if(rc != SQLITE_OK)
		{
			cout<<"SQL error : "<<errmsg<<endl;
			sqlite3_free(errmsg);
		}
		j++;
	}
	
	cout<<"\n�����ͼ��ɣ�\n";
	return ;
}

void point2()//ɾ����ͼ
{
	char sql[200],tabname[30],tabnamea[30],tabnameb[30];
	char **dbResult,*errmsg;
	int i=0,j=1,n=0,rc,nRow,nColumn;
	sprintf(sql,"SELECT name FROM sqlite_master WHERE type='table'  ");//sqlite3�鿴���б�ֻ��ʾ���� ����.table�ķ���
	rc = sqlite3_get_table(db,sql,&dbResult,&nRow,&nColumn,&errmsg);
	if( rc != SQLITE_OK )
	{
		cout<<"error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
	}
	else if(nRow==0)
		cout<<"���κα�!\n";
	else//��ʼɾ��
	{		
		cout<<"0 : �����ϲ�\n\n";
		for(i=nColumn;i<(nRow+1)*nColumn;i=i+3,j++)//i=i+3 ֻ��ʾ��ͼ�ı���������ʾ����
			cout<<j<<" : "<<dbResult[i]<<endl;

		cout<<"��ѡ��ɾ���ĸ���ͼ�򷵻� : ";
		if(scanf("%d",&n)!=1 || n>nRow/3 || n<0)//������������������Ǳ�ʾ���ٸ��� 
		{
			cout<<"�Ƿ�����!\n";
			getchar();			
		}
		else if(n==0)
			return ;
		else//����Ҫɾ���ĵ�ͼ��ŲŻ�ɾ��  nֻ���� 0<=n<=nRow  0~����
		{			
			n=(n-1)*3+1;//����2 ɾ��4,5,6
			strcpy(tabnamea,dbResult[n]);
			strcat(tabnamea,"a");
			strcpy(tabnameb,dbResult[n]);
			strcat(tabnameb,"b");
			sprintf( sql,"drop table %s" , dbResult[n] );//����ɾ����������
			rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
			if(rc!=SQLITE_OK)
			{
				cout<<"error : "<<errmsg<<endl;
				sqlite3_free(errmsg);
			}
			else
				cout<<"ɾ�����!\n";

			sprintf( sql,"drop table %s" , tabnamea );//����ɾ����������
			rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
			if(rc!=SQLITE_OK)
			{
				cout<<"error : "<<errmsg<<endl;
				sqlite3_free(errmsg);
			}

			sprintf( sql,"drop table %s" , tabnameb );//����ɾ����������
			rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
			if(rc!=SQLITE_OK)
			{
				cout<<"error : "<<errmsg<<endl;
				sqlite3_free(errmsg);
			}
		}
		sqlite3_free_table(dbResult);
	}
	return ;
}


void point3()//�������·��
{
	int vs=0,n=0,judge=0;//vs���  n�յ�
	int e,i=0,j=1,h=0,rc,nRow,nColumn,tn=0;
	char Fname[20],Ename[20],sql[200],tabname[30];
	char **dbResult,*errmsg;

	Queue Q;
	InitQueue(Q);

	sprintf(sql,"SELECT name FROM sqlite_master WHERE type='table'  ");//sqlite3�鿴���б�ֻ��ʾ���� ����.table�ķ���
	rc = sqlite3_get_table(db,sql,&dbResult,&nRow,&nColumn,&errmsg);
	if( rc != SQLITE_OK )
	{
		cout<<"error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
	}
	else if(nRow==0)
	{
		cout<<"�����κα��ȵ����ͼ������!\n";
		return ;
	}
	else
	{
		cout<<"�ѵ����ͼ�� : \n\n";
		for(i=nColumn;i<(nRow+1)*nColumn;i=i+3,j++)//i=i+3 ֻ��ʾ��ͼ�ı���������ʾ����
			cout<<j<<" : "<<dbResult[i]<<endl;
		//sqlite3_free_table(dbResult);
	}
	cout<<"\n������Ҫ���ĸ���ͼ�ϲ����أ� (������): ";
	while(scanf("%d",&tn)!=1 || tn>nRow/3 || tn<=0)//������������������Ǳ�ʾ���ٸ���
	{
		cout<<"�Ƿ�����!����� : ";
		getchar();
	}
	tn=(tn-1)*3+1;//�����tn������Ҫʹ�õ�����tn,ת��һ��
	strcpy(tabname,dbResult[tn]);//����
	CreateUDG(G,tabname);//����ͼ

	cout<<"\nTraverseDFS : "<<endl;
	TraverseBFS(G,Q);
	i=0;
	while(DelQueue(Q,e))
	{
		cout<<i+1<<" : "<<G.vexs[e]<<endl;
		i++;
	}
	cout<<endl;


	do{

	cout<<"������ص����ƻ����:\n";
	do{
		if(vs==-1)//�����һ��
			cout<<"��������������� : \n";

	cout<<"���:";
	cin>>Fname;
	if(atoi(Fname)<=G.vexnum && atoi(Fname)!=0)//�������  //֮������1~n����ֵ
		vs=atoi(Fname)-1;//vs������ֱ����Ϊ�±꣬���Դ�1~n,Ҫ��Ϊ��0~n-1  (nΪ�ص�����)
	else//��������
		vs=LocateVex(G,Fname);//�Ƿ���-1
	}while(vs==-1);

	dijkstra(G, vs);//��vsΪԴ���õϽ�˹�����㷨�����vsͨ����������·��

	do{
		if(n==-1)
			cout<<"��������������� : \n";
	cout<<"�յ�:";
	cin>>Ename;
	if(atoi(Ename)<=G.vexnum && atoi(Ename)!=0)
		n=atoi(Ename)-1;
	else
		n=LocateVex(G,Ename);//-1ԭ�� ͬvs
	}while(n==-1);

	cout<<"���·������Ϊ:    "<<dist[n]<<endl;
	cout<<"·��:    ";
    for(i=0;i<G.vexnum && prev[n][i]>-1;i++)
	{
		h=prev[n][i];
		cout<<G.vexs[h]<<"->";
	}
	cout<<"������ɣ�"<<endl<<endl;
//		cout<<prev[n][i]+1<<"->";//������ʾ  //+1����Ϊ�������ǵ�һ������1 ���ͻ�����û��0
		cout<<"�Ƿ���� : \n1:��  0:��\n";
		while(scanf("%d",&judge)!=1)
		{
			getchar();
			cout<<"�Ƿ����룬����������Ƿ� : ";
		}
	}while(judge==1);//0����
	return ;
}

int main(void)
{
	int n,rc;
	rc = sqlite3_open("Map.db",&db);
	if(rc!=SQLITE_OK)
	{
		cout<<"error : Can't open the Datebase , "<<sqlite3_errmsg(db);
		exit(0);
	}
	else
	do{
		cout<<"\t0:�˳�\n\t1:��ӵ�ͼ\n\t2:ɾ����ͼ\n\t3:�������·��(����ʾӵ�еĵ�ͼ)\n������ : ";
		//cin>>n;
		if(scanf("%d",&n)!=1)//����1a,2a֮����Ի����
		{
			cout<<"�Ƿ�����!\n";
			getchar();
		}
		else
		switch (n)
		{
			case 1:point1();break;
			case 2:point2();break;
			case 3:point3();break;
			default:break;
		}
		system("pause");
		system("cls");
	}while(n!=0);

	sqlite3_close(db);
	return 0;
}
//14->13->11->9->8->2->1->0->
/*:
16
25
����
ͼ���
�ٳ�
6������
С����
�³��㳡
ѧ�����
ר�ҹ�Ԣ
�̹�3��
�ɻ���
9#��ѧ¥
12#��ѧ¥
15#��ѧ¥
ѧ���㳡
����
���ϲ���

����
�ٳ�
90
����
ͼ���
60
����
6������
100
ͼ���
�ٳ�
20
ͼ���
6������
50
�ٳ�
С����
65
ͼ���
С����
40
С����
�³��㳡
55
6������
�³��㳡
80
�³��㳡
ѧ�����
50
�³��㳡
ר�ҹ�Ԣ
100
ѧ�����
ר�ҹ�Ԣ
130
ר�ҹ�Ԣ
�̹�3��
150
�ٳ�
�̹�3��
250
�̹�3��
�ɻ���
100
�ɻ���
9#��ѧ¥
40
�ɻ���
12#��ѧ¥
60
9#��ѧ¥
12#��ѧ¥
30
9#��ѧ¥
ѧ���㳡
75
12#��ѧ¥
ѧ���㳡
15
12#��ѧ¥
15#��ѧ¥
50
15#��ѧ¥
ѧ���㳡
45
15#��ѧ¥
����
160
ѧ���㳡
����
100
����
���ϲ���
120
*/

/*
4
6
��վ
������
����ʦ��
����վ

��վ
����վ
10
��վ
������
800
��վ
����ʦ��
600
������
����ʦ��
100
������
����վ
700
����ʦ��
����վ
500
*/