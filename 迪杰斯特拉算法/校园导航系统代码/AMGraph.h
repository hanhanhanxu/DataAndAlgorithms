#pragma once
#include<iostream>
#include<string>
#define MVNum 100//��󶥵���
using namespace std;
#include"Status.h"
#include"Queue.h"
#define MaxInt 9999//����ֵ


char sql[200];
int rc;
char **dbResult;
int nRow,nColumn;
char *errmsg;


typedef string VerTexType;//������������
typedef int ArcType;//Ȩֵ
typedef struct
{
	VerTexType vexs[MVNum];//�����
	ArcType arcs[MVNum][MVNum];//�ڽӾ���
	int vexnum,arcnum;//��ǰ�������ͱ���
}AMGraph;
bool visited[MVNum];

int LocateVex(AMGraph G,string e)//���صص��λ�ã����������˳���0��ʼ
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
			G.arcs[i][j]=MaxInt;//��ʼ��Ϊ�����
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
		G.arcs[j][i]=G.arcs[i][j]=weight;//���߻�������Ȩֵ
	}
	return OK;
}

void DFS(AMGraph G,int i)
{
	int j;
	cout<<G.vexs[i]<<"  ";
	visited[i]=true;
	//���ο���������Ϊv���������Ƿ���ڱ���δ������
	for(j=0;j<G.vexnum;j++)//�����һ��
		if((G.arcs[i][j]!=0) && (!visited[j]))
			DFS(G,j);//����һ���ͽ�ȥ����������ȱ���
}
void TraverseDFS(AMGraph G)
{
	int i;
	for(i=0;i<G.vexnum;i++)
		visited[i]=false;
	for(i=0;i<G.vexnum;i++)
		if(!visited[i])//�ز�����
		DFS(G,i);

}

void BFS(AMGraph G,int i,Queue &Qh)
{
	int j,k;
	Queue Q;
	InitQueue(Q);
	EnterQueue(Qh,i);
//	cout<<G.vexs[i]<<"  ";//������ĳ�������±�
	visited[i]=true;
	EnterQueue(Q,i);
	while(!EmptyQueue(Q))
	{
		if(DelQueue(Q,k)==true)//�ճ��ӵ�k
		for(j=0;j<G.vexnum;j++)//���μ��i�����е��Ƿ��бߣ��б��Ƿ������      //������е��ȳ���
			if((G.arcs[k][j]!=0) && (!visited[j]))//�ȳ��Ե�k�ȱ�������һ��
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
 * Dijkstra���·����
 * ����ͳ��ͼ(G)��"����vs"������������������·����
 *
 * ����˵����
 *        G -- ͼ
 *       vs -- ��ʼ����(start vertex)��������"����vs"��������������·����
 *     prev[i][j] -- ǰ���������顣����prev[i][j]��j��0��-1֮ǰ��ֵ��"����vs"��"����i"�����·����˳����������ȫ��������±ꡣ
 *     dist[i] -- �������顣����dist[i]��"����vs"��"����i"�����·���ĳ��ȡ�
*/
void dijkstra(AMGraph G, int vs)
{
    int i,j,k,h;
    int min;//�����ϵ���Сֵ,ÿ�ζ�����
    int final[MVNum];      // flag[i]=1��ʾ"����vs"��"����i"�����·���ѳɹ���ȡ��
    
    // ��ʼ��
    for (i = 0; i < G.vexnum; i++)
    {
        final[i] = 0;              // ����i�����·����û��ȡ����
        dist[i] = G.arcs[vs][i];// ��ʼ��ʱ������vs������i�����·��Ϊ"����vs"��"����i"��Ȩ��
		for(j=0;j<G.vexnum;j++)//����vs��ÿ������i=0,1,2,3
			prev[i][j]=-1;//����vs������i��ÿһ������-1
		if(dist[i]<MaxInt)//����ֱ�����ڵĵ�i  vs��i֮��Ĳ���
		{
			prev[i][0]=vs;//����vs������i�ĵ�һ����vs
			prev[i][1]=i;//��Ϊֱ���ٽ�  ���Եڶ������ǵ�i
		}
    }

    // ��"����vs"������г�ʼ��
    final[vs] = 1;//vs��vs�Ѿ��������·����
    dist[vs] = 0;//Ϊ0

    // ����G.vexnum-1(�������������㶼Ҫ��������Ϊ�Ǵ�0��ʼ��0�ǵ�һ�������i���ڼ�������ν��ֻ�Ǹ�������)�Σ�ÿ���ҳ�һ����������·����  
    for (i = 1; i < G.vexnum; i++)
    {
        // Ѱ�ҵ�ǰ��С��·���� �������Ǵ�Դ�㵽�ĸ���Ķ��У�ֻҪһ����С��ֵ��    
        min = MaxInt;
        for (j = 0; j < G.vexnum; j++)
        {
            if ( (!final[j]) && dist[j]<min )// ������δ��ȡ���·���Ķ����У��ҵ���vs����Ķ���(k)��
            {
                min = dist[j];
                k = j;
            }
        }
        // ���"����k"Ϊ�Ѿ���ȡ�����·��
        final[k] = 1;//��Ϊk�Ѿ������ڴ�vs����ֱ������̵�·���ˣ����Կ��Ǵ�vs��k�ٵ�ĳ����j,��vsֱ�ӵ�j�Ƚ����ߵĴ�С
																	//�ܲ��������k���м�㻻��vsֱ�ӵ�j��������vs��j��û��·����

        // ������ǰ���·����ǰ������
        // �������Ѿ�"����k�����·��"֮�󣬸���"δ��ȡ���·���Ķ�������·����ǰ������"��
        for (j = 0; j < G.vexnum; j++)
        {
			//�����ѷ��ֵĴ�Դ�㵽k��·����̵���������Դ�㵽�������·���ܷ����Ⱦ���k�ٵ��Ǹ����·���滻��
            if ( (!final[j]) && (min+G.arcs[k][j] < dist[j]) )//dist[j]�൱��dist[vs][j],���ǰѴ�Դ�㵽ĳ����j�ľ��� �� ��Դ���ȵ���Դ����̵�·���ǵ�k���ٴ��ǵ�k��ĳ����j�ľ����  �Ƚϣ����ĸ���
            {
                dist[j] = min + G.arcs[k][j];//�Ⱦ���k����k��j��ֱ�ӵ�j���̣��Ǿ͸���Դ�㵽j�����·��
				for(h=0; h<G.vexnum; h++)//�޸�p[j]��vs��j�����Ķ������vs��k���������ж����ټ��϶���j  
                {										//��Ϊ��k���Ѿ������·����
                    prev[j][h]=prev[k][h];  //����k����Ϊ������·���Ǿ���k�ģ�����vs��k��ô�ߣ�vs��j��ǰ����Ҳ��ô�ߣ����ߵ�k��
                    if(prev[j][h]==-1)//��p[j][]��һ������-1�ĵط����ɶ���j
                    {  
                        prev[j][h]=j;//��vs��k����һ�����͵�j��
                        break;  //����prev[j][]����vs��j������·�������ĵ�
                    }  //prev[j][h]����˼��  Դ��vs��j��·���еĵڣ�h+1�����ߵ����Ǹ��������
                }//for
            }//if
        }//for
    }//for

	/*
    // ��ӡdijkstra���·���Ľ��
    printf("dijkstra(%s): \n", G.vexs[vs].c_str());
	

    for (i = 0; i < G.vexnum; i++)
	{
        printf("  ���·��(%s, %s)=%d\n", G.vexs[vs].c_str(), G.vexs[i].c_str(), dist[i]);
		cout<< "  ";
		for(j=0;prev[i][j]>-1;j++)
		{
			h=prev[i][j];
			cout<<G.vexs[h]<<" -> ";
		}
		cout<<endl;
		cout<<"  ������ʾΪ:\n  ";
		for(j=0;prev[i][j]>-1;j++)
			cout<<prev[i][j]+1<<" -> ";
		cout<<"\b\b\b\b";
		cout<<endl<<endl;
	}
	*/

}