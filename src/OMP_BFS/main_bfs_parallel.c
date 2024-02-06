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

	main_bfs: ./main_bfs_parallel "file_graph" n_thread n_measure

 */

 /**
	@file main_bfs_parallel.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#define SIZE 300000
#define SIZE_NEIGHBOURS 1000
#define START_NODE 0
#define SIZE_FILENAME 50
/*
#define DEBUG
//*/

#define STARTTIME(ID)\
	double start42##ID;\
	double stop42##ID;\
	start42##ID = omp_get_wtime();

#define STOPTIME(ID)\
	stop42##ID = omp_get_wtime();\
	double time##ID;\
	time##ID= stop42##ID-start42##ID;\

#define PRINTTIME(ID)\
	printf("%d;%lf;\n",n_valori,time##ID);\

struct queue {
  int* items;
  int front;
  int rear;
  int size;
};

struct queue* createQueue();
void enqueue(struct queue* q, int);
int dequeue(struct queue* q);
void display(struct queue* q);
int isEmpty(struct queue* q);
void printQueue(struct queue* q);

//Struct node
struct node {
  int vertex;
  struct node* neighbours[SIZE_NEIGHBOURS];
  int n_neighbours;
};

//Creazione di un nodo
struct node* createNode(int v) {
  struct node* newNode = malloc(sizeof(struct node));
  newNode->vertex = v;
  newNode->n_neighbours = 0;
  return newNode;
}

//Struct Graph
struct Graph {
  int numVertices;
  struct node** adjLists;
  int* visited;
  int max_degree;
};

//Creazione del grafo
struct Graph* createGraph(int vertices) {
  struct Graph* graph = malloc(sizeof(struct Graph));
  graph->numVertices = vertices;

  graph->adjLists = malloc(vertices * sizeof(struct node*));
  graph->visited = malloc(vertices * sizeof(int));

  for (int i = 0; i < vertices; i++) {
    graph->adjLists[i] = NULL;
    graph->visited[i] = 0;
  }

  return graph;
}

//Aggiungi arco
void addEdge(struct Graph* graph, int src, int dest) {
  struct node* srcNode = graph->adjLists[src];
  struct node* destNode = graph->adjLists[dest];

  if(srcNode == NULL){
    srcNode = createNode(src);
    graph->adjLists[src] = srcNode;
  }
  if(destNode == NULL){
    destNode = createNode(dest);
    graph->adjLists[dest] = destNode;
  }

  //Arco da src a dest
  srcNode->neighbours[srcNode->n_neighbours] = graph->adjLists[dest];
  srcNode->n_neighbours++;

  //Arco da dest a src
  destNode->neighbours[destNode->n_neighbours] = graph->adjLists[src];
  destNode->n_neighbours++;
}

//Crea una coda
struct queue* createQueue(int size) {
  struct queue* q = malloc(sizeof(struct queue));
  q->front = -1;
  q->rear = -1;
  q->size = size;
  q->items = malloc(sizeof(int) * size);
  return q;
}

//Controlla se una coda è vuota
int isEmpty(struct queue* q) {
  if (q->rear == -1)
    return 1;
  else
    return 0;
}

//Aggiungi un elemento in coda
void enqueue(struct queue* q, int value) {
  if (q->rear != q->size - 1)
  {
    if (q->front == -1)
      q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
  }
}

//Rimuovi un elemento dalla coda
int dequeue(struct queue* q) {
  int item;
  if (isEmpty(q)) {
    item = -1;
  } else {
    item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
      q->front = q->rear = -1;
    }
  }
  return item;
}

//Stampa la codacd
void printQueue(struct queue* q) {
  int i = q->front;

  if (isEmpty(q)) {
    printf("Queue is empty");
  } else {
    printf("\nQueue contains \n");
    for (i = q->front; i < q->rear + 1; i++) {
      printf("%d ", q->items[i]);
    }
  }
}

void bfs_parallel(struct Graph* graph) {
  int next_queue, current_queue = 0;
  int l[2];
  l[0] = 1;
  int q[2][SIZE];
  int level = 2;

  graph->visited[START_NODE] = 1;
  q[current_queue][START_NODE] = START_NODE;

  while(1){
    next_queue = (current_queue + 1) % 2;
    l[next_queue] = 0;
    #pragma omp parallel shared(graph, q, l)
    {
      int currentVertex;
      struct node* v;
      #pragma omp for
      for(int i=0; i<l[current_queue]; i++){
        currentVertex = q[current_queue][i];
        v = graph->adjLists[currentVertex];
        for(int j=0; j<v->n_neighbours; j++){
          if (graph->visited[v->neighbours[j]->vertex] == 0) {
            #pragma omp critical
            {
              graph->visited[v->neighbours[j]->vertex] = level;
              q[next_queue][l[next_queue]++] = v->neighbours[j]->vertex;
            }
          }    
        }
      }
    }
    if(l[next_queue] == 0){
      break;
    }
    else{
      current_queue=next_queue;
      level++;
    }
  }
}

int main(int argc, char** argv)
{
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

  int n_valori = 0;
  char * line = NULL;
  size_t len = 0;
  if(getline(&line, &len, fp)!= -1){
    n_valori = atoi(strtok(line, " "));
  }

  #ifdef DEBUG
    printf("Valori: %d\n", n_valori);
  #endif

  struct Graph* graph = createGraph(n_valori);
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

  int n_threads = atoi(argv[2]);
  int work = 1;
  char* vertex;
  int actual_level = 0;
  FILE * fp_test;

  //Setto il numero di thread
  omp_set_num_threads(n_threads);

  /*
  *
  * Itero diverse volte la BFS
  * Importante iterare per evitare la ricostruzione del grafo
  *
  */
  for(int z=0; z<atoi(argv[3]); z++){
    #pragma omp barrier
    //Avvio il timer
    STARTTIME(2)
    //Avvio la BFS in parallelo
    bfs_parallel(graph);
    //Salvo il tempo trascorso
    STOPTIME(2)

    /*
    *
    * Inizio fase di test per la correttezza della BFS
    *
    */
    fp_test = fopen(file_test, "r");
    if (fp_test == NULL) exit(EXIT_FAILURE);

    work = 1;
    actual_level = 0;

    while (getline(&line, &len, fp_test) != -1) {
      actual_level++;
      vertex = strtok(line, " ");
      if(graph->visited[atoi(vertex)] < actual_level){
        #ifdef DEBUG
          printf("Not Work!\n");
        #endif
        work = 0;
        break;
      }
      
      while(1) {
        vertex = strtok(NULL, " ");
        if(vertex == NULL) break;

        if(graph->visited[atoi(vertex)] < actual_level){
          #ifdef DEBUG
            printf("Not Work!\n");
          #endif
          work = 0;
          break;
        }
      }

      if(work == 0){
        break;
      }
    }

    if(work == 1){
      PRINTTIME(2)
      #ifdef DEBUG
        printf("\nWork!\n");
      #endif
    }
    /*
    *
    * Fine fase di test per la correttezza della BFS
    *
    */
    
    fclose(fp_test);

    //Resetto i vertici visitati del grafo
    for(int x=0; x<n_valori; x++){
      graph->visited[x] = 0;
    }
  }
    
  if (line)
      free(line);
  exit(EXIT_SUCCESS);
}