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

 */

 /**
	@file BFS_texture.cu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#define SIZE 300000
#define SIZE_NEIGHBOURS 1000
#define START_NODE 0
#define SIZE_FILENAME 50

#define STARTTIME \
  cudaEvent_t start,stop;\
  cudaEventCreate(&start);\
  cudaEventCreate(&stop);\
  cudaEventRecord(start,0);\

#define STOPTIME \
  cudaEventRecord(stop,0);\
  cudaEventSynchronize(stop);\
  float elapsed;\
  cudaEventElapsedTime(&elapsed,start,stop);\
  elapsed/=1000.f;\
  cudaEventDestroy(start);\
  cudaEventDestroy(stop);\
  
#define PRINTTIME \
  printf("Kernel elapsed time %fs \n", elapsed);\
//

//Struct Graph
struct Graph {
  int numVertices;
  int max_neighbours;
  int* adjMatrix;
  int* visited;
  int* offset;
  int max_degree;
};

//Creazione del grafo
struct Graph* createGraph(int vertices) {
  struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
  graph->numVertices = vertices;
  int max_neig = 100;
  graph->max_neighbours = max_neig;

  graph->visited = (int *) malloc(vertices * sizeof(int));
  graph->adjMatrix = (int *) malloc(vertices * max_neig * sizeof(int));
  graph->offset = (int *) calloc(vertices, sizeof(int));

  for (int i = 0; i < vertices; i++) {
    graph->visited[i] = 0;
    for (int j = 0; j < max_neig; j++){
      graph->adjMatrix[i*max_neig + j] = -1;
    }
  }
  return graph;
}

//Aggiungi arco
void addEdge(struct Graph* graph, int src, int dest) {
  //Arco da src a dest
  graph->adjMatrix[src * graph->max_neighbours + graph->offset[src]++] = dest;
  //Arco da dest a src
  graph->adjMatrix[dest * graph->max_neighbours + graph->offset[dest]++] = src;
}

void make_csv(float time, int N, int gridsize, int thread_per_block){
  FILE* fp;
  char root_filename[] = "measure/texture";

  char* filename = (char*) malloc(sizeof(char) * (strlen(root_filename) + 10*sizeof(char)));
  sprintf(filename,"%s/%d/BFS_v_%d_b_%d_tpb.csv",root_filename,N,gridsize,thread_per_block);
  
  if ( access( filename, F_OK ) == 0 ) {
     fp = fopen(filename,"a"); 

  } else {
     fp = fopen(filename,"w");
     fprintf(fp, "N_vertices; BlockSize; GridSize; time_sec\n");
  }
  fprintf(fp, "%d; %d; %d; %f\n", N, thread_per_block, gridsize, time);
  fclose(fp);
}

texture<int,1> text_mem;

__global__ void computeNextQueue(int n_vertices, int *distance, int queueSize, int *currentQueue, int *nextQueueSize, int *nextQueue, int level) {
  const int tid = blockIdx.x * blockDim.x + threadIdx.x;  // thread id
  //Creo un index per l'iterazione
  int idx_iter = tid;

	while (idx_iter < queueSize) {
		int current = currentQueue[idx_iter];
		for (int i = 0; i < n_vertices; i++) {
			int v = tex1Dfetch(text_mem,current * n_vertices + i);
			if(v==-1) break;
      if(distance[v] == 0){
        distance[v] = level + 1;
        int position = atomicAdd(nextQueueSize, 1);
        nextQueue[position] = v;
      }
		}
    idx_iter += blockDim.x * gridDim.x;
	}
}

int main(int argc,char ** argv){
  if(argc<4) {
    fprintf(stderr,"ERROR too few arguments of: %s\n",argv[0]);
    exit(1);
  }
  // Concateno il nome parziale dato in ingresso e ottengo due stringe:
  // 1) File dove è presente i dati per il grafo "graph.txt"
  // 2) File dove è presente la bfs di test per verificare il funzionamento "graph_test.txt" 
  char file_graph[SIZE_FILENAME];
  strcpy(file_graph, argv[1]);
  char file_test[SIZE_FILENAME] = ""; 
	strncat(file_test, file_graph, SIZE_FILENAME-1);
  strncat(file_test, "_test.txt", SIZE_FILENAME-1);
  strncat(file_graph, ".txt", SIZE_FILENAME-1);

  /*
  *
  * Inizio fase di lettura da file e creazione grafo
  *
  */
  // Apro il file di input
  FILE * fp = fopen(file_graph, "r");
  if (fp == NULL) exit(EXIT_FAILURE);

  int n_vertices = 0;
  char * line = NULL;
  size_t len = 0;
  if(getline(&line, &len, fp)!= -1){
    n_vertices = atoi(strtok(line, " "));
  }

  #ifdef DEBUG
    printf("Valori: %d\n", n_vertices);
  #endif

  struct Graph* graph = createGraph(n_vertices);
  char* other;
  int src_vertex;

  while (getline(&line, &len, fp) != -1) {
    src_vertex = atoi(strtok(line, " "));
    while(1) {
      other = strtok(NULL, " ");
      if(other == NULL){
        break;
      }
      addEdge(graph, src_vertex, atoi(other));
    }
  }
  fclose(fp);
  /*
  *
  * Fine fase di lettura da file e creazione grafo
  *
  */

  //Assegno il numero dei blocchi
  int blocks = atoi(argv[2]);
  dim3 dimGrid(blocks);

  //Assegno il numero dei thread per blocco
  int th_p_block = atoi(argv[3]);
  dim3 dimBlock(th_p_block);

  //Inizializzazioni variabili CPU
	int currentQueueSize = 1;
	const int NEXT_QUEUE_SIZE = 0;
  const int start_vertex = START_NODE;
	int level = 1;

  //Inizializzaione variabili GPU
  int *d_adjacencyMatrix;
	int *d_firstQueue;
	int *d_secondQueue;
	int *d_nextQueueSize;
  int *d_distance;

  const int size = n_vertices * sizeof(int);
  const int adjacencySize = n_vertices * graph->max_neighbours * sizeof(int);

  //Allocazione su GPU
  cudaMalloc((void **)&d_adjacencyMatrix, adjacencySize);
  cudaMalloc((void **)&d_firstQueue, size);
	cudaMalloc((void **)&d_secondQueue, size);
  cudaMalloc((void **)&d_distance, size);
	cudaMalloc((void **)&d_nextQueueSize, sizeof(int));

  cudaMemcpy(d_adjacencyMatrix, graph->adjMatrix, adjacencySize, cudaMemcpyHostToDevice);
  cudaMemcpy(d_nextQueueSize, &NEXT_QUEUE_SIZE, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_firstQueue, &start_vertex, sizeof(int), cudaMemcpyHostToDevice);

  graph->visited[start_vertex] = 1;
  cudaMemcpy(d_distance, graph->visited, n_vertices * sizeof(int), cudaMemcpyHostToDevice);

  cudaChannelFormatDesc channel = cudaCreateChannelDesc<int>();
  cudaBindTexture(0,text_mem, d_adjacencyMatrix, channel);

  //Avvio il timer
  STARTTIME
  while (currentQueueSize > 0) {
		int *d_currentQueue;
		int *d_nextQueue;
		if (level % 2 != 0) {
			d_currentQueue = d_firstQueue;
			d_nextQueue = d_secondQueue;
		}
		else {
			d_currentQueue = d_secondQueue;
			d_nextQueue = d_firstQueue;
		}
		computeNextQueue<<<blocks, th_p_block>>> (graph->max_neighbours, d_distance, currentQueueSize, d_currentQueue, d_nextQueueSize, d_nextQueue, level);
		cudaDeviceSynchronize();
		level++;
		cudaMemcpy(&currentQueueSize, d_nextQueueSize, sizeof(int), cudaMemcpyDeviceToHost);
		cudaMemcpy(d_nextQueueSize, &NEXT_QUEUE_SIZE, sizeof(int), cudaMemcpyHostToDevice);
	}
  //Salvo il tempo trascorso
  STOPTIME

  cudaUnbindTexture(text_mem);

  cudaMemcpy(graph->visited, d_distance, size, cudaMemcpyDeviceToHost);
  cudaDeviceSynchronize();

  //Free delle variabili su GPU
  cudaFree(d_adjacencyMatrix);
  cudaFree(d_firstQueue);
  cudaFree(d_secondQueue);
  cudaFree(d_distance);

  /*
  *
  * Inizio fase di test per la correttezza della BFS
  *
  */
  FILE * fp_test;
  int work = 0;
  char* vertex;
  int actual_level = 0;

  fp_test = fopen(file_test, "r");
  if (fp_test == NULL) exit(EXIT_FAILURE);

  work = 1;
  actual_level = 0;

  while (getline(&line, &len, fp_test) != -1) {
    actual_level++;
    vertex = strtok(line, " ");
    if(graph->visited[atoi(vertex)] < actual_level){
      work = 0;
      break;
    }
    
    while(1) {
      vertex = strtok(NULL, " ");
      if(vertex == NULL){
        break;
      }
      if(graph->visited[atoi(vertex)] < actual_level){
        work = 0;
        break;
      }
    }

    if(work == 0){
      break;
    }
  }
  fclose(fp_test);

  /*
  *
  * Fine fase di test per la correttezza della BFS
  *
  */
    
  if(work == 1){
    PRINTTIME
    make_csv(elapsed, n_vertices, blocks, th_p_block);
    #ifdef DEBUG
      printf("\nWork!\n");
    #endif
  }
  
  return 0;
}