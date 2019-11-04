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

AMGraph G;//地图的创建

void point1()//添加地图
{
	char **dbResult;
	int nRow,nColumn;
	int rc;
	char sql[200];
	char *errmsg;//错误信息集
	char tabname[30],tabnamea[30],tabnameb[30];//表名


	cout<<"请输入地图名字 : ";
	cin>>tabname;//地图名
	//比较
	sprintf(sql,"SELECT name FROM sqlite_master WHERE type='table'  ");//sqlite3查看所有表只显示表名 不用.table的方法
	rc = sqlite3_get_table(db,sql,&dbResult,&nRow,&nColumn,&errmsg);
	if( rc != SQLITE_OK )
	{
		cout<<"error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
	}
	while(nRow--)
	{
		if(strcmp(tabname,dbResult[nRow])==0)//相等，表名重复
		{
			cout<<"表名重复，创建失败!(提示：为避免重复可在表明后面加1,2等数字)\n";
			return ;
		}
	}
	strcpy(tabnamea,tabname);
	strcat(tabnamea,"a");
	strcpy(tabnameb,tabname);
	strcat(tabnameb,"b");
	
	//创表阶段  tabname tabnamea tabnameb
	sprintf(sql , "CREATE TABLE %s(mnum int,dnum int)" , tabname );//数量表  mnum 地点数目 dnum 边的数目
	rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(rc != SQLITE_OK)
	{
		cout<<"SQL error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
		return ;
	}
	sprintf(sql , "CREATE TABLE %s(name varchar(20))" , tabnamea );//地名表
	rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(rc != SQLITE_OK)
	{
		cout<<"SQL error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
		return ;
	}
	sprintf(sql , "CREATE TABLE %s(cname varchar(20),pname varchar(20),distan double)" , tabnameb );//地点与地点之间的联系距离表
	rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(rc !=SQLITE_OK)
	{
		cout<<"SQL error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
		return ;
	}

	//把数据导入数据库
	int n,m,i=0,j=0;
	double weight;
	char sdname[20],scname[20],spname[20];
	cout<<"请输入地点数 : ";
	while(scanf("%d",&n)!=1)
	{
		getchar();
		cout<<"输入错误!\n请继续输入 : ";
	}
	cout<<"请输入路径数 : ";
	while(scanf("%d",&m)!=1)
	{
		getchar();
		cout<<"输入错误!\n请继续输入 : ";
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
		cout<<"请输入地点名 : ";
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
		cout<<"请输入有通路的两个地点及之间的路径长度:\n";
		cout<<"地点1 : ";
		cin>>scname;
		cout<<"地点2 : ";
		cin>>spname;
		cout<<"路径长度(单位/米) : ";
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
	
	cout<<"\n导入地图完成！\n";
	return ;
}

void point2()//删除地图
{
	char sql[200],tabname[30],tabnamea[30],tabnameb[30];
	char **dbResult,*errmsg;
	int i=0,j=1,n=0,rc,nRow,nColumn;
	sprintf(sql,"SELECT name FROM sqlite_master WHERE type='table'  ");//sqlite3查看所有表只显示表名 不用.table的方法
	rc = sqlite3_get_table(db,sql,&dbResult,&nRow,&nColumn,&errmsg);
	if( rc != SQLITE_OK )
	{
		cout<<"error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
	}
	else if(nRow==0)
		cout<<"无任何表!\n";
	else//开始删除
	{		
		cout<<"0 : 返回上层\n\n";
		for(i=nColumn;i<(nRow+1)*nColumn;i=i+3,j++)//i=i+3 只显示地图的表名，不显示其他
			cout<<j<<" : "<<dbResult[i]<<endl;

		cout<<"请选择删除哪个地图或返回 : ";
		if(scanf("%d",&n)!=1 || n>nRow/3 || n<0)//输入大于行数，行数是表示多少个表 
		{
			cout<<"非法输入!\n";
			getchar();			
		}
		else if(n==0)
			return ;
		else//输入要删除的地图编号才会删除  n只能是 0<=n<=nRow  0~表数
		{			
			n=(n-1)*3+1;//输入2 删除4,5,6
			strcpy(tabnamea,dbResult[n]);
			strcat(tabnamea,"a");
			strcpy(tabnameb,dbResult[n]);
			strcat(tabnameb,"b");
			sprintf( sql,"drop table %s" , dbResult[n] );//连带删除另两个表
			rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
			if(rc!=SQLITE_OK)
			{
				cout<<"error : "<<errmsg<<endl;
				sqlite3_free(errmsg);
			}
			else
				cout<<"删除完成!\n";

			sprintf( sql,"drop table %s" , tabnamea );//连带删除另两个表
			rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
			if(rc!=SQLITE_OK)
			{
				cout<<"error : "<<errmsg<<endl;
				sqlite3_free(errmsg);
			}

			sprintf( sql,"drop table %s" , tabnameb );//连带删除另两个表
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


void point3()//查找最短路径
{
	int vs=0,n=0,judge=0;//vs起点  n终点
	int e,i=0,j=1,h=0,rc,nRow,nColumn,tn=0;
	char Fname[20],Ename[20],sql[200],tabname[30];
	char **dbResult,*errmsg;

	Queue Q;
	InitQueue(Q);

	sprintf(sql,"SELECT name FROM sqlite_master WHERE type='table'  ");//sqlite3查看所有表只显示表名 不用.table的方法
	rc = sqlite3_get_table(db,sql,&dbResult,&nRow,&nColumn,&errmsg);
	if( rc != SQLITE_OK )
	{
		cout<<"error : "<<errmsg<<endl;
		sqlite3_free(errmsg);
	}
	else if(nRow==0)
	{
		cout<<"暂无任何表，先导入地图再来吧!\n";
		return ;
	}
	else
	{
		cout<<"已导入地图有 : \n\n";
		for(i=nColumn;i<(nRow+1)*nColumn;i=i+3,j++)//i=i+3 只显示地图的表名，不显示其他
			cout<<j<<" : "<<dbResult[i]<<endl;
		//sqlite3_free_table(dbResult);
	}
	cout<<"\n请问想要在哪个地图上查找呢？ (输入编号): ";
	while(scanf("%d",&tn)!=1 || tn>nRow/3 || tn<=0)//输入大于行数，行数是表示多少个表
	{
		cout<<"非法输入!请继续 : ";
		getchar();
	}
	tn=(tn-1)*3+1;//输入的tn并不是要使用的坐标tn,转换一下
	strcpy(tabname,dbResult[tn]);//表名
	CreateUDG(G,tabname);//创建图

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

	cout<<"请输入地点名称或代号:\n";
	do{
		if(vs==-1)//检查上一次
			cout<<"输入错误！重新输入 : \n";

	cout<<"起点:";
	cin>>Fname;
	if(atoi(Fname)<=G.vexnum && atoi(Fname)!=0)//输入代号  //之可能是1~n的数值
		vs=atoi(Fname)-1;//vs在下面直接作为下标，所以从1~n,要变为从0~n-1  (n为地点总数)
	else//输入名称
		vs=LocateVex(G,Fname);//非法是-1
	}while(vs==-1);

	dijkstra(G, vs);//以vs为源点用迪杰斯特拉算法计算出vs通过各点的最短路径

	do{
		if(n==-1)
			cout<<"输入错误！重新输入 : \n";
	cout<<"终点:";
	cin>>Ename;
	if(atoi(Ename)<=G.vexnum && atoi(Ename)!=0)
		n=atoi(Ename)-1;
	else
		n=LocateVex(G,Ename);//-1原因 同vs
	}while(n==-1);

	cout<<"最短路径长度为:    "<<dist[n]<<endl;
	cout<<"路径:    ";
    for(i=0;i<G.vexnum && prev[n][i]>-1;i++)
	{
		h=prev[n][i];
		cout<<G.vexs[h]<<"->";
	}
	cout<<"导航完成！"<<endl<<endl;
//		cout<<prev[n][i]+1<<"->";//代号显示  //+1是因为：大门是第一个，是1 ，客户眼里没有0
		cout<<"是否继续 : \n1:是  0:否\n";
		while(scanf("%d",&judge)!=1)
		{
			getchar();
			cout<<"非法输入，请继续输入是否 : ";
		}
	}while(judge==1);//0返回
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
		cout<<"\t0:退出\n\t1:添加地图\n\t2:删除地图\n\t3:查找最短路径(可显示拥有的地图)\n请输入 : ";
		//cin>>n;
		if(scanf("%d",&n)!=1)//输入1a,2a之类的仍会进入
		{
			cout<<"非法输入!\n";
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
大门
图书馆
操场
6号宿舍
小礼堂
下沉广场
学宛餐厅
专家公寓
教工3村
荷花池
9#教学楼
12#教学楼
15#教学楼
学生广场
网球场
东南餐厅

大门
操场
90
大门
图书馆
60
大门
6号宿舍
100
图书馆
操场
20
图书馆
6号宿舍
50
操场
小礼堂
65
图书馆
小礼堂
40
小礼堂
下沉广场
55
6号宿舍
下沉广场
80
下沉广场
学宛餐厅
50
下沉广场
专家公寓
100
学宛餐厅
专家公寓
130
专家公寓
教工3村
150
操场
教工3村
250
教工3村
荷花池
100
荷花池
9#教学楼
40
荷花池
12#教学楼
60
9#教学楼
12#教学楼
30
9#教学楼
学生广场
75
12#教学楼
学生广场
15
12#教学楼
15#教学楼
50
15#教学楼
学生广场
45
15#教学楼
网球场
160
学生广场
网球场
100
网球场
东南餐厅
120
*/

/*
4
6
火车站
南阳理工
南阳师范
汽车站

火车站
汽车站
10
火车站
南阳理工
800
火车站
南阳师范
600
南阳理工
南阳师范
100
南阳理工
汽车站
700
南阳师范
汽车站
500
*/