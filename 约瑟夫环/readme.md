## 约瑟夫环问题

> 一个经典的循环链表问题。

## 题意

已知 n 个人（以编号1，2，3，…，n分别表示）围坐在一张圆桌周围，从编号为 k 的人开始顺时针报数，数到 m 的那个人出列；他的下一个人又从 1 还是顺时针开始报数，数到 m 的那个人又出列；依次重复下去，要求找到最后出列的那个人。

## 实例

> 例如有 5 个人，要求从编号为 3 的人开始，数到 2 的那个人出列：

![img](http://data.biancheng.net/uploads/allimg/170718/2-1FGQ54403413.png)

出列顺序依次为：

编号为 3 的人开始数 1，然后 4 数 2，所以 4 先出列；
4 出列后，从 5 开始数 1，1 数 2，所以 1 出列；
1 出列后，从 2 开始数 1，3 数 2，所以 3 出列；
3 出列后，从 5 开始数 1，2 数 2，所以 2 出列；
最后只剩下 5 自己，所以 5 出列。  



## 解题思路

循环链表：将普通链表的尾部和首部连接即为循环链表。

> end->next = first

将人封装为节点：num数字 , next指针。

将这些人做成循环链表连接起来。并初始化为每个节点的num赋值：1，2，3，4，5...

循环到最后一个人，即循环条件：

```c
while(p->next!=p)
{

}
```

读到m的人退出，即

```c
for(i=0;i<m;i++)
{
    pre = p;
    p=p->next;
}//经过循环后，p已经是要删除的节点了。
pre->next = p->next;//将要删除的节点前一个跟后一个相连。
free(p);//退出，销毁，释放空间
```



## 代码

```c
#include <stdio.h>
#include <stdlib.h>

//人  节点
typedef struct node {
	int number;
	struct node* next;
}person;

//初始化节点，初始化循环链表；返回首节点
person* initLink(int n) 
{
	//首节点
	person* head = (person*)malloc(sizeof(person));
	head->number = 1;
	head->next = NULL;
	//头节点 cyclic
	person* cyclic = head;
	//后面的空间
	for (int i = 2; i <= n; i++) {
		//赋值
		person* body = (person*)malloc(sizeof(person));
		body->number = i;
		body->next = NULL;
		//连接
		cyclic->next = body;
		cyclic = cyclic->next;
	}
	//循环后cyclic是最后一个，将最后一个的next和首节点相连
	cyclic->next = head;//首尾相连
	return head;
}
void findAndKillK(person* head, int k, int m) {
	//临时节点tail
	person* tail = head;
	//找到链表第一个结点的上一个结点，为删除操作做准备
	while (tail->next != head) {
		tail = tail->next;//tail为第二个
	}
	person* p = head;
	//找到编号为k的人
	while (p->number != k) {
		tail = p;
		p = p->next;
	}//tail为开始执行节点的上一个

	//从编号为k的人开始，此时p即为坐标为k的人。
	while (p->next != p) {
		//找到从p报数1开始，报m的人，并且还要知道数m-1的人的位置tail，方便做删除操作。
		for (int i = 1; i < m; i++) {
			tail = p;
			p = p->next;//p就是要删除的节点，tail为上一个
		}
		tail->next = p->next;//从链表上将p结点摘下来
		printf("出列人的编号为:%d\n", p->number);
		free(p);
		p = tail->next;//继续使用p指针指向出列编号的下一个编号，游戏继续
	}
	printf("出列人的编号为:%d\n", p->number);
	free(p);
}
int main(void) {
	printf("输入圆桌上的人数n:");
	int n;
	scanf("%d", &n);
	person* head = initLink(n);
	printf("初始化完成。");
	printf("从第k人开始报数(k>1且k<%d)：", n);
	int k;
	scanf("%d", &k);
	printf("数到m的人出列：");
	int m;
	scanf("%d", &m);
	printf("开始执行：");
	findAndKillK(head, k, m);
	return 0;
}
```





