#include <stdio.h>
#include <stdlib.h>

//��  �ڵ�
typedef struct node {
	int number;
	struct node* next;
}person;

//��ʼ���ڵ㣬��ʼ��ѭ�����������׽ڵ�
person* initLink(int n) 
{
	//�׽ڵ�
	person* head = (person*)malloc(sizeof(person));
	head->number = 1;
	head->next = NULL;
	//ͷ�ڵ� cyclic
	person* cyclic = head;
	//����Ŀռ�
	for (int i = 2; i <= n; i++) {
		//��ֵ
		person* body = (person*)malloc(sizeof(person));
		body->number = i;
		body->next = NULL;
		//����
		cyclic->next = body;
		cyclic = cyclic->next;
	}
	//ѭ����cyclic�����һ���������һ����next���׽ڵ�����
	cyclic->next = head;//��β����
	return head;
}
void findAndKillK(person* head, int k, int m) {
	//��ʱ�ڵ�tail
	person* tail = head;
	//�ҵ������һ��������һ����㣬Ϊɾ��������׼��
	while (tail->next != head) {
		tail = tail->next;//tailΪ�ڶ���
	}
	person* p = head;
	//�ҵ����Ϊk����
	while (p->number != k) {
		tail = p;
		p = p->next;
	}//tailΪ��ʼִ�нڵ����һ��

	//�ӱ��Ϊk���˿�ʼ����ʱp��Ϊ����Ϊk���ˡ�
	while (p->next != p) {
		//�ҵ���p����1��ʼ����m���ˣ����һ�Ҫ֪����m-1���˵�λ��tail��������ɾ��������
		for (int i = 1; i < m; i++) {
			tail = p;
			p = p->next;//p����Ҫɾ���Ľڵ㣬tailΪ��һ��
		}
		tail->next = p->next;//�������Ͻ�p���ժ����
		printf("�����˵ı��Ϊ:%d\n", p->number);
		free(p);
		p = tail->next;//����ʹ��pָ��ָ����б�ŵ���һ����ţ���Ϸ����
	}
	printf("�����˵ı��Ϊ:%d\n", p->number);
	free(p);
}
int main(void) {
	printf("����Բ���ϵ�����n:");
	int n;
	scanf("%d", &n);
	person* head = initLink(n);
	printf("��ʼ����ɡ�");
	printf("�ӵ�k�˿�ʼ����(k>1��k<%d)��", n);
	int k;
	scanf("%d", &k);
	printf("����m���˳��У�");
	int m;
	scanf("%d", &m);
	printf("��ʼִ�У�");
	findAndKillK(head, k, m);
	return 0;
}