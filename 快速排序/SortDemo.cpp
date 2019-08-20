#include <stdio.h>

int* hqsort(int* arr, int start, int end);
void swap(int& i, int& j);//交换两个数
void printArr(int* arr,int n);


int main(void)
{
	//int arr[] = { 4,8,9,5,0,1,3,7,2,6 };
	//int arr[] = {3, 3, 3, 7, 9, 122344, 4656, 34, 34, 4656, 5, 6, 7, 8, 9, 343, 57765, 23, 12321};
	//int arr[] = {5,1,8,3,0,6,2,9,2,7,8,4,2,1,9,5,3};
	int arr[] = { 5,1,4,2,8,5,3,2 };
	
	int n = sizeof(arr) / sizeof(arr[0]);
	printf("数组大小：%d\n", n);


	//测试交换两数
	/*int i = 2, j = 3;
	swap(i, j);
	printf("i:%d   j:%d\n", i, j);*/

	//测试*q > markNum
	/*int* h = arr + (9 - 0);
	if (*h > 3)
		printf("√");*/

	//测试q--
	/*int* h = arr + 9;
	h--;
	printf("%d",*h);*/


	hqsort(arr,0,n-1);

	printf("排序后:\n");
	printArr(arr,n);


	printf("\n\n\n\n\n");
}

int* hqsort(int* arr, int start, int end)//数组头指针，开始坐标，结束坐标
{
	int* q;
	int markNum = arr[start];
	int i=start, j=end;
	int num = 0;
	//q = arr + (end-start); //并非之差，而是永远都是移到最后。
	q = arr + end;


	while (i < j)
	{
		//num++;//比放后面少了一个++ 并没有。。
		if (num % 2 == 0)
		{
			while (i<j && *q >= markNum)
			{
				j--;
				q--;
			}
			if (i < j)//当两个移到一处时，就不需处理
			{
				//交换*q与arr[i]的值
				swap(*q, arr[i]);
				//转移指针
				q = &arr[i];
			}
		}
		else
		{
			while (i < j && *q < markNum)
			{
				i++;
				q++;
			}
			if (i < j)
			{
				swap(*q, arr[j]);
				q = &arr[j];
			}
		}
		num++; //num需初始化为0
	}
	//printf("num:%d\n", num);
	//printArr(arr,n);//查看每次递归排序的结果


	//判断是否有序，最终
	
	//假设不是最终，排左，排右
	//i-1是基准值所在坐标左一点，排左(或右)的前提是，至少要有两个值才能排，所以必须要i-start>1。前闭后闭，
	if(i - start > 1)
		hqsort(arr, start, i-1);
	//j+1是基准值所在坐标右一点
	if(end - j > 1)
		hqsort(arr, j + 1, end);


	
	return arr;

}

void swap(int& i, int& j)
{
	//一个数异或同一个数两次等于它本身
	i = i ^ j;
	j = i ^ j;//i^j^j = i
	i = i ^ j;//i^j^i = j

	/*int t;
	t = i;
	i = j;
	j = t;*/
}

void printArr(int* arr,int n)
{
	
	for (int i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
	printf("\n");
}