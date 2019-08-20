package hx.insist.sort;

/**
 * @ClassName demo
 * @Description //TODO
 * @Author 123
 * @Date 2019/8/16 15:25
 * @Version 1.0
 * 快速排序
 * 我自己想的版本
 **/
public class demo2 {
    public static void main(String[] args) {
        //int arr[] = new int[]{5,3,8,1,9,0,3,6,1,3};
        //int arr[] = { 4,8,9,5,0,1,3,7,2,6 };
        int arr[] = {3, 3, 3, 7, 9, 122344, 4656, 34, 34, 4656, 5, 6, 7, 8, 9, 343, 57765, 23, 12321};
        //int arr[] = {5,1,8,3,0,6,2,9,2,7,8,4,2,1,9,5,3};

        int n = arr.length;
        System.out.println("长度是："+n);

        //排序
        hqsort(arr,0,n-1);


        for (int i : arr) {
            System.out.print(i+" ");
        }
        System.out.println();
    }

    private static int[] hqsort(int arr[],int start,int end){
        int i = start;
        int j = end;
        int markNum = arr[start];
        int p = arr[end];
        int sign = 0;

        while (i<j){
            if(sign%2==0){
                while(i<j && p>=markNum){
                    j--;
                    p = arr[j];
                }
                if(i<j){
                    //交换两数
                    arr[i]=p;//p赋值给arr[i]

                    //p移动过去 p已经“移动”过去了,p现在已经是那个“较小”的值了
                }
            }else{
                while(i<j && p<markNum){
                    i++;
                    p = arr[i];//★
                }
                if(i<j){
                    arr[j] = p;//★
                }
            }
            sign++;
        }
        //循环结束后，将基准值填入。
        arr[i] = markNum;//i = j

        if(i-1>start)
            hqsort(arr,start,i-1);
        if(j+1<end)
            hqsort(arr,j+1,end);

        return arr;
    }


}
