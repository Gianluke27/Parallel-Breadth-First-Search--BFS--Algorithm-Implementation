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

 PS1='\W: '

 HOW TO USE 

	graph bfs test: ./graph_bfs_test "file_graph" 

 */

 /**
	@file graph_bfs_test.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 400000
#define SIZE_NEIGHBOURS 1000
#define START_NODE 0
#define SIZE_FILENAME 50

//#define DEBUG

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

//Creating a node
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

// Creating a graph
struct Graph* createGraph(int vertices) {
  struct Graph* graph = malloc(sizeof(struct Graph));
  graph->numVertices = vertices;

  graph->adjLists = malloc(vertices * sizeof(struct node*));
  graph->visited = malloc(vertices * sizeof(int));

  int i;
  for (i = 0; i < vertices; i++) {
    graph->adjLists[i] = NULL;
    graph->visited[i] = 0;
  }

  return graph;
}

// Add edge
void addEdge(struct Graph* graph, int src, int dest) {
  struct node* srcNode = graph->adjLists[src];
  struct node* destNode = graph->adjLists[dest];

  //printf("Arco: %d %d\n", src, dest);

  if(srcNode == NULL){
    //printf("Creo nodo: %d\n", src);
    srcNode = createNode(src);
    graph->adjLists[src] = srcNode;
  }
  if(destNode == NULL){
    //printf("Creo nodo: %d\n", dest);
    destNode = createNode(dest);
    graph->adjLists[dest] = destNode;
  }

  
  // Add edge from src to dest
  srcNode->neighbours[srcNode->n_neighbours] = graph->adjLists[dest];
  srcNode->n_neighbours++;

  // Add edge from dest to src
  destNode->neighbours[destNode->n_neighbours] = graph->adjLists[src];
  destNode->n_neighbours++;

  //printf("src: %d, dest: %d, vicini: %d\n", graph->adjLists[src]->vertex, graph->adjLists[dest]->vertex, graph->adjLists[src]->n_neighbours);

}

// Create a queue
struct queue* createQueue(int size) {
  struct queue* q = malloc(sizeof(struct queue));
  q->front = -1;
  q->rear = -1;
  q->size = size;
  q->items = malloc(sizeof(int) * size);
  return q;
}

// Check if the queue is empty
int isEmpty(struct queue* q) {
  if (q->rear == -1)
    return 1;
  else
    return 0;
}

// Adding elements into queue
void enqueue(struct queue* q, int value) {
  if (q->rear == q->size - 1)
  {
    //printf("\nQueue is Full!!");
  }
  else {
    if (q->front == -1)
      q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
  }
}

// Removing elements from queue
int dequeue(struct queue* q) {
  int item;
  if (isEmpty(q)) {
    //printf("Queue is empty");
    item = -1;
  } else {
    item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
      //printf("Resetting queue ");
      q->front = q->rear = -1;
    }
  }
  return item;
}

// Print the queue
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

int main(int argc, char** argv)
{
  char file_graph[SIZE_FILENAME];
  strcpy(file_graph, argv[1]);
  char file_test[SIZE_FILENAME] = ""; 
	strncat(file_test, file_graph, SIZE_FILENAME-1);
  strncat(file_test, "_test.txt", SIZE_FILENAME-1);
  strncat(file_graph, ".txt", SIZE_FILENAME-1);

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

  FILE * fp_test;
  char * str = NULL;

  fp_test = fopen(file_test, "w");
  if (fp_test == NULL) exit(EXIT_FAILURE);

  int currentVertex;
  //struct node* v;
  int next_queue, current_queue = 0;
  int l[2];
  l[0] = 1;
  int first = 1;
  int q[2][SIZE];

  graph->visited[START_NODE] = 1;
  q[current_queue][START_NODE] = START_NODE;
  fprintf(fp_test,"%d\n",0);

  while(1){
    first = 1;
    next_queue = (current_queue + 1)%2;
    l[next_queue] = 0;
    for(int i=0; i<l[current_queue]; i++){
      currentVertex = q[current_queue][i];
      struct node* v = graph->adjLists[currentVertex];
      //printf("vicini di %d: %d\n", currentVertex, v->n_neighbours);
      for(int j=0; j<v->n_neighbours; j++){
        if (graph->visited[v->neighbours[j]->vertex] == 0) {
          graph->visited[v->neighbours[j]->vertex] = 1;
          q[next_queue][l[next_queue]++] = v->neighbours[j]->vertex;
          if(first == 1){
            fprintf(fp_test,"%d",v->neighbours[j]->vertex);
            first = 0;
          }
          else{
            fprintf(fp_test," %d",v->neighbours[j]->vertex);
          }
        }    
      }
    }
    if(l[next_queue] == 0){
      //printf("l vuoto\n");
      break;
    }
    else{
      //printf("l NON vuoto\n");
      fprintf(fp_test,"\n");
      current_queue=next_queue;
    }
    //printf("\n\n\n");
  }

  fclose(fp_test);
  fclose(fp);
  if (line)
      free(line);
  exit(EXIT_SUCCESS);
}