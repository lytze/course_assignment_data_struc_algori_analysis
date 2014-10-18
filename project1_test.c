#include <time.h>
#include "project1_functions.h"

#ifndef CLK_TCK
#define CLK_TCK 1
#endif
/* For systems lack the define of CLK_TCK, let it = 1, for they count ticks in ms */

int main() {
    FILE *file_p;
    file_p = fopen("./project1_runtime.csv", "wt");
        /* load the outputs into a .csv file */
    
    fprintf(file_p, 
    "lenght, iter.rep, iter.tck, iter.ttime, iter.time, recr.rep, recr.tck, recr.ttime, recr.timr\n");
        /* write the header of the .csv file */
    
    /* ================================================================ */
    
    int list_lengths[] = {100, 500, 1000, 2000, 4000, 6000, 8000, 10000};
    int list_len_index = 0;
        /* lengths of lists */
    int rep_time = 10;
        /* let procedures run 10 times */
    
    int current_list[10000 + 100];
        /* current list for manupulating */
    int through = 0;
        /* filling current list index */
    
    clock_t start, stop;
    
    while (list_len_index - 8) {
        
        while (through - list_lengths[list_len_index]) {
            current_list[through] = through + 1;
            through++;
        }
            /* filling the current list */
        
        fprintf(file_p, "%d, ", list_lengths[list_len_index]);
        
        start = clock();
        for(int i = 0; i < rep_time; i++) 
            IteractiveMethod(current_list, list_lengths[list_len_index]);
        stop = clock();
            /* iteractive method  time counted */
        fprintf(file_p, "%d, %lu, %f, %f, ", rep_time, stop - start, 
            (float)(stop - start) / CLK_TCK, (float)(stop - start) / CLK_TCK / rep_time);
            /* iteractive method data filled */
        
        start = clock();
        for(int i = 0; i < rep_time; i++)
            RecursiveMethod(current_list, 0, list_lengths[list_len_index] - 1);
        stop = clock();
            /* recursive method time counted */
        fprintf(file_p, "%d, %lu, %f, %f\n",rep_time, stop - start, 
            (float)(stop - start) / CLK_TCK, (float)(stop - start) / CLK_TCK / rep_time);
            /* recursive method data filled */
        
        list_len_index++;
    }   /* go through all lengths */
    
    /* ================================================================ */
    
    fclose(file_p);
    return 0;
}