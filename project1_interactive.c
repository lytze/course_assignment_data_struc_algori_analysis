#include <time.h>
#include "project1.h"

#ifndef CLK_TCK
#define CLK_TCK 1
#endif
// tester added: if compiling syst do not present clock tick in time.h, define it as 1

clock_t start, stop; /* clock_t is a built-in type for processor time (ticks) */
double duration;  /* records the run time (seconds) of a function */

int main() {

    int N;      // The number of integers
    int K;      // The number of iterations
    int a[12000];

    printf("Please input the number of integers in your list.\n");
    scanf("%d", &N);

    for(int i=0; i<N; i++){    // initiate the array a[], which holds N elements.   
        a[i]=i;
    }

    printf("Please input the number of iterations(K) for iteractive method.\n");
    scanf("%d", &K);

    printf("Output of iteractive method:\n");
    start = clock();     /* records the ticks at the beginning of the function call */

    for(int i=0; i<K; i++){       //repeat the iteractive function calls for K times to obtain a total run time
        IteractiveMethod(a, N);
    }

    stop = clock();    /* records the ticks at the end of the function call */
    duration = ((double)(stop - start))/CLK_TCK/K; 
    printf("\nFor iteractive method, the ticks are %lu ; the total time is %lfs; the duration time is %lf.\n",stop-start,duration*K,duration);
    
    printf("Please input the number of iterations(K) for iteractive method.\n");
    scanf("%d", &K);

    printf("Output of recursive method:\n");
    start = clock();     /* records the ticks at the beginning of the function call */

    for(int i=0; i<K; i++){      ////repeat the recursive function calls for K times to obtain a total run time
        RecursiveMethod(a, 0, N-1);;
    }

    stop = clock();    /* records the ticks at the end of the function call */
    duration = ((double)(stop - start))/CLK_TCK/K; 
    printf("\nFor recursive method, the ticks are %lu ; the total time is %lfs; the duration time is %lf.\n",stop-start,duration*K,duration);

    return 0;
}

