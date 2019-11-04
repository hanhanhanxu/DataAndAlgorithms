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