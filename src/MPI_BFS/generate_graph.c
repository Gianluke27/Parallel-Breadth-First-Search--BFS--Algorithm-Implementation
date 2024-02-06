/* 
 * Course: High Performance Computing 2021/2022
 * 
 * Lecturer: Francesco Moscato	fmoscato@unisa.it
 *
 * Student:           
 * Canzolino	Gianluca  	06227001806  	g.canzolino3@studenti.unisa.it 
 *
 * Copyright (C) 2021 - All Rights Reserved
 *
 * This file is part of CommonAssignment1.
 *
 * CommonAssignment1 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CommonAssignment1 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CommonAssignment1.  If not, see <http://www.gnu.org/licenses/>.

 HOW TO USE 

	generate graph: ./generate_graph "file_graph" "n_of_vertex"

 */

 /**
	@file generate_graph.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define SIZE_FILENAME 50

int main(int argc, char** argv)
{
    int num;
    FILE *fptr;

    char file_graph[SIZE_FILENAME];
    strcpy(file_graph, argv[1]);
    strncat(file_graph, ".txt", SIZE_FILENAME-1);

    fptr = fopen(file_graph,"w");

    if(fptr == NULL)
    {
        printf("Error!");   
        exit(1);             
    }
    
    //numero di vertici
    num = atoi(argv[2]);
    
    srand(time(NULL));
    
    int count = 0;
    int max_per_vertex = (int) ((float)num * 0.01);
    if(max_per_vertex > 100){
        max_per_vertex = 100;
    }
    printf("max: %d\n", max_per_vertex);
    int perc;

    fprintf(fptr,"%d\n",num);
    
    for(int i=0; i < num - 2; i++){
        fprintf(fptr,"%d",i);
        count = 0;
        perc = ((num - i)/ max_per_vertex) + (((num - i) % max_per_vertex) != 0);
        //printf("perc: %d\n", perc);
        for(int j=i+1; j < num; j++){
            if((rand() % perc) == 0){
                fprintf(fptr," %d", j);
                count++;
                //printf("matrix[%d][%d] = 0\n", i, j);
            }
            if(count == max_per_vertex){
                break;
            }
        }
        fprintf(fptr,"\n");
    }
    
    fclose(fptr);

    return 0;
}