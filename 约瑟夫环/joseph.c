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