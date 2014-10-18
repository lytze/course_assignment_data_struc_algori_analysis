#include <stdlib.h>
#include <stdio.h>

//a list of N integers are delivered to subfunctions by array a[];

void IteractiveMethod(int a[], int N){     //define the iteractive method for performance measurement 
    
    for(int i=0; i<N; i++) {        //print the integers one by one through a for-loop
        printf("%d ",a[i]);
    }
}; 

void RecursiveMethod(int a[], int left, int right) {     //define the recursive method for performance measurement

    int center = (right + left)/2;

    if (1 == right - left) {                 //The first base case: There are two elements in the subarray. 
        printf("%d ", a[left]);           //Sequentially output the two elements to console. 
        printf("%d ", a[right]);
    }
    else if (1 == (right - left)%2) {        //When there are even number of elements in the subarray, there is no middle element.
        RecursiveMethod(a,left,center);   //Recursively print the first part, and then the second part.
        RecursiveMethod(a,center+1,right);
    }
    else if (0 == right - left) {            //The second base case: There are one elements in the subarray.
        printf("%d ", a[left]);
    }
    else if (0 == (right - left)%2) {          //When there are even number of elements in the subarray, there is no middle element.
        RecursiveMethod(a, left, center - 1);    //Recursively print the first part, followed by printing the integer in the middle, and finally the second part.
        printf("%d ", a[center]);
        RecursiveMethod(a, center+1, right);
        
    }

};  
