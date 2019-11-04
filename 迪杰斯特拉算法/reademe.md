> 这个校园导航系统是大一下学期的数据结构课程作业，在导航部分核心代使用到了迪杰斯特拉算法，算出两个点之间的最短距离，迪杰斯特拉算法是典型的贪心算法，本质是广度优先遍历了一张图，类似的最短路径算法还有弗罗伊德算法，这个效率好像比迪杰斯特拉算法高，不过我那时候比较懒只看了一个，就到现在也没有看另一个，有时间一定要学习学习。。。

### C语言，sqlite3数据库，迪杰斯特拉算法，图的创建遍历



需求分析描述：
校园导航系统，在进行实用功能前需要先对系统进行数据导入，导入数据库一个地图，包括地图的地点总数及名称，路径总数及名称，系统对sqlite3数据库进行了链接，所以可以向其中导入多个地图，由于数据库中表名是名字，所以地图名字不能重复，导入地图后，用户查找路径时，只需输入相应地图编号，表示要在哪个地图上查找，后再输入相应起点终点即可，系统会显示出起点到终点的最短路径距离和具体每一步都要走的地点。

系统结构设计：
本系统分三个模块，添加地图，删除地图，查找路径。
添加地图时需要输入地图名字，地点数目及地点名称，路径数目及个路径的两端地点名称和这条路径的长度，这些数据输入成功后直接保存到数据库中。
删除地图，输出已有的地图名字，需要删除某个地图直接输入即可删除。
查找路径，用户查找路径时输入在某个地图上查找，系统会根据数据库中的数据生成相应的一个图，输入起点，会按照这个起点作为源点对图进行迪杰斯特拉算法遍历，期间会把全局变量一维数组dist[n]和二维数组prev[n][]，进行赋值，dist[n]表示源点到n点的最短路径长度，prev[n][]表示源点到n点的最短路径经过的每一个点。用户输入终点后会给n赋值，利用dist和prev进行需要内容的输出。

总结和体会：
迪杰斯特拉算法求最短路径时prev[n]只保存了到达n点的前一个点，而且有时候更新最短路径时会覆盖上次的内容，所以我可以把每次的内容利用起来，保存在二维数组里，这样每次的点都不会浪费，则具体每一步即是根据prev[n][x]中x从0到有值的过程中所有点构成的。

主要算法清单：
main:
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
	CreateUDG(G,tabname);//创建表

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
AMGraph.h:
#pragma once
#include<iostream>
#include<string>
#define MVNum 100//最大顶点数
using namespace std;
#include"Status.h"
#include"Queue.h"
#define MaxInt 9999//极大值


char sql[200];
int rc;
char **dbResult;
int nRow,nColumn;
char *errmsg;


typedef string VerTexType;//顶点数据类型
typedef int ArcType;//权值
typedef struct
{
	VerTexType vexs[MVNum];//顶点表
	ArcType arcs[MVNum][MVNum];//邻接矩阵
	int vexnum,arcnum;//当前顶点数和边数
}AMGraph;
bool visited[MVNum];

int LocateVex(AMGraph G,string e)//返回地点的位置，依照输入的顺序从0开始
{
	for(int i=0;i<G.vexnum;i++)
		if(e==G.vexs[i])
			return i;
	return -1;
}
Status CreateUDG(AMGraph &G,char *tabname)
{
	int i,j,k;
	double weight;
	char v1[20],v2[20],tabnamea[30],tabnameb[30];

	sprintf( sql,"select * from %s" , tabname );
	rc = sqlite3_get_table(db,sql,&dbResult,&nRow,&nColumn,&errmsg);
	if(rc !=SQLITE_OK || nRow==0)
	{
		cout<<"SQL error : "<<errmsg;
		sqlite3_free(errmsg);
	}
	G.vexnum=atoi(dbResult[2]);
	G.arcnum=atoi(dbResult[3]);

	strcpy(tabnamea,tabname);
	strcat(tabnamea,"a");
	sprintf( sql,"select * from %s" , tabnamea );
	rc = sqlite3_get_table(db,sql,&dbResult,&nRow,&nColumn,&errmsg);
	if(rc !=SQLITE_OK)
	{
		cout<<"SQL error : "<<errmsg;
		sqlite3_free(errmsg);
	}
	for(i=nColumn;i<(nRow+1)*nColumn;i=i+nColumn)
		G.vexs[i-nColumn]=dbResult[i];

	for(i=0;i<G.vexnum;i++)
		for(j=0;j<G.vexnum;j++)
		{
			if(i==j)
				G.arcs[i][j]=0;
			else
			G.arcs[i][j]=MaxInt;//初始化为无穷大
		}

	strcpy(tabnameb,tabname);
	strcat(tabnameb,"b");
	sprintf(sql,"select * from %s" , tabnameb);
	rc = sqlite3_get_table(db,sql,&dbResult,&nRow,&nColumn,&errmsg);
	if(rc !=SQLITE_OK)
	{
		cout<<"SQL error : "<<errmsg;
		sqlite3_free(errmsg);
	}
	for(k=nColumn;k<(nRow+1)*nColumn;k=k+nColumn)
	{
		strcpy(v1,dbResult[k]);strcpy(v2,dbResult[k+1]);
		weight=strtod(dbResult[k+2],NULL);
		i=LocateVex(G,v1);
		j=LocateVex(G,v2);
		if(i!=-1 && j!=-1)
		G.arcs[j][i]=G.arcs[i][j]=weight;//或者换成输入权值
	}
	return OK;
}

void DFS(AMGraph G,int i)
{
	int j;
	cout<<G.vexs[i]<<"  ";
	visited[i]=true;
	//依次看看横坐标为v的纵坐标是否存在边且未被遍历
	for(j=0;j<G.vexnum;j++)//检查这一行
		if((G.arcs[i][j]!=0) && (!visited[j]))
			DFS(G,j);//遇到一个就进去，所以是深度遍历
}
void TraverseDFS(AMGraph G)
{
	int i;
	for(i=0;i<G.vexnum;i++)
		visited[i]=false;
	for(i=0;i<G.vexnum;i++)
		if(!visited[i])//必不可少
		DFS(G,i);

}

void BFS(AMGraph G,int i,Queue &Qh)
{
	int j,k;
	Queue Q;
	InitQueue(Q);
	EnterQueue(Qh,i);
//	cout<<G.vexs[i]<<"  ";//把输出改成入队列下标
	visited[i]=true;
	EnterQueue(Q,i);
	while(!EmptyQueue(Q))
	{
		if(DelQueue(Q,k)==true)//刚出队的k
		for(j=0;j<G.vexnum;j++)//依次检查i和所有点是否有边，有边是否遍历过      //先入队列的先出来
			if((G.arcs[k][j]!=0) && (!visited[j]))//先出对的k先遍历它这一行
			{
				EnterQueue(Qh,j);
				//cout<<G.vexs[j]<<"  ";
				visited[j]=true;
				EnterQueue(Q,j);
			}
	}
}

void TraverseBFS(AMGraph G,Queue &Qh)
{
	int i;
	for(i=0;i<G.vexnum;i++)
		visited[i]=false;
	for(i=0;i<G.vexnum;i++)
		if(!visited[i])
		BFS(G,i,Qh);
}

/*
 * Dijkstra最短路径。
 * 即，统计图(G)中"顶点vs"到其它各个顶点的最短路径。
 *
 * 参数说明：
 *        G -- 图
 *       vs -- 起始顶点(start vertex)。即计算"顶点vs"到其它顶点的最短路径。
 *     prev[i][j] -- 前驱顶点数组。即，prev[i][j]当j从0到-1之前的值是"顶点vs"到"顶点i"的最短路径按顺序所经历的全部顶点的下标。
 *     dist[i] -- 长度数组。即，dist[i]是"顶点vs"到"顶点i"的最短路径的长度。
*/
void dijkstra(AMGraph G, int vs)
{
    int i,j,k,h;
    int min;//理想上的最小值,每次都更新
    int final[MVNum];      // flag[i]=1表示"顶点vs"到"顶点i"的最短路径已成功获取。
    
    // 初始化
    for (i = 0; i < G.vexnum; i++)
    {
        final[i] = 0;              // 顶点i的最短路径还没获取到。
        dist[i] = G.arcs[vs][i];// 初始化时，顶点vs到顶点i的最短路径为"顶点vs"到"顶点i"的权。
		for(j=0;j<G.vexnum;j++)//顶点vs到每个顶点i=0,1,2,3
			prev[i][j]=-1;//顶点vs到顶点i的每一步都是-1
		if(dist[i]<MaxInt)//对于直接相邻的点i  vs到i之间的步骤
		{
			prev[i][0]=vs;//顶点vs到顶点i的第一步是vs
			prev[i][1]=i;//因为直接临接  所以第二步就是到i
		}
    }

    // 对"顶点vs"自身进行初始化
    final[vs] = 1;//vs到vs已经“有最短路径”
    dist[vs] = 0;//为0

    // 遍历G.vexnum-1(除了自身其他点都要遍历，因为是从0开始，0是第一个，这个i等于几都无所谓，只是个计数的)次；每次找出一个顶点的最短路径。  
    for (i = 1; i < G.vexnum; i++)
    {
        // 寻找当前最小的路径； （不论是从源点到哪个点的都行，只要一个最小的值）    
        min = MaxInt;
        for (j = 0; j < G.vexnum; j++)
        {
            if ( (!final[j]) && dist[j]<min )// 即，在未获取最短路径的顶点中，找到离vs最近的顶点(k)。
            {
                min = dist[j];
                k = j;
            }
        }
        // 标记"顶点k"为已经获取到最短路径
        final[k] = 1;//因为k已经是现在从vs出发直达点的最短的路径了，所以考虑从vs到k再到某个点j,和vs直接到j比较两者的大小
																	//能不能用这个k做中间点换掉vs直接到j（不考虑vs到j有没有路径）

        // 修正当前最短路径和前驱顶点
        // 即，当已经"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。
        for (j = 0; j < G.vexnum; j++)
        {
			//利用已发现的从源点到k的路径最短的条件来找源点到其他点的路径能否由先经过k再到那个点的路径替换掉
            if ( (!final[j]) && (min+G.arcs[k][j] < dist[j]) )//dist[j]相当于dist[vs][j],就是把从源点到某个点j的距离 和 从源点先到离源点最短的路径那点k，再从那点k到某个点j的距离和  比较，看哪个短
            {
                dist[j] = min + G.arcs[k][j];//先经过k再由k到j比直接到j更短，那就更新源点到j的最短路径
				for(h=0; h<G.vexnum; h++)//修改p[j]，vs到j经过的顶点包括vs到k经过的所有顶点再加上顶点j  
                {										//因为到k的已经是最短路径了
                    prev[j][h]=prev[k][h];  //换成k是因为这个最短路径是经过k的，所以vs到k怎么走，vs到j的前几步也怎么走，先走到k点
                    if(prev[j][h]==-1)//在p[j][]第一个等于-1的地方换成顶点j
                    {  
                        prev[j][h]=j;//由vs到k后，下一步，就到j点
                        break;  //所以prev[j][]就是vs到j点的最短路径经过的点
                    }  //prev[j][h]的意思是  源点vs到j的路径中的第（h+1）步走的是那个点的坐标
                }//for
            }//if
        }//for
    }//for

	/*
    // 打印dijkstra最短路径的结果
    printf("dijkstra(%s): \n", G.vexs[vs].c_str());
	

    for (i = 0; i < G.vexnum; i++)
	{
        printf("  最短路径(%s, %s)=%d\n", G.vexs[vs].c_str(), G.vexs[i].c_str(), dist[i]);
		cout<< "  ";
		for(j=0;prev[i][j]>-1;j++)
		{
			h=prev[i][j];
			cout<<G.vexs[h]<<" -> ";
		}
		cout<<endl;
		cout<<"  代号显示为:\n  ";
		for(j=0;prev[i][j]>-1;j++)
			cout<<prev[i][j]+1<<" -> ";
		cout<<"\b\b\b\b";
		cout<<endl<<endl;
	}
	*/

}
Queue.h:

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
Status.h:
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
typedef int Status;


参考文献：数据结构（c语言版）
开发环境：win7旗舰版，vc6.0
