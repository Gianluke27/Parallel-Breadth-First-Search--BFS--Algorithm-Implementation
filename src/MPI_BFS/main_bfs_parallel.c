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
	@file main_bfs_parallel.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include <time.h>
#include <math.h>
#include <stdbool.h>
#define SIZE 300000
#define SIZE_NEIGHBOURS 1000
#define START_NODE 0
#define SIZE_FILENAME 50
/*
#define DEBUG
//*/

/*
#define PRINT_ALL_TIME
//*/

#define STARTTIME(id)                           \
  clock_t start_time_42_##id, end_time_42_##id; \
  start_time_42_##id = clock();

#define STOPTIME(id)        \
  end_time_42_##id = clock(); \
  double time##id = ((double)(end_time_42_##id - start_time_42_##id)) / CLOCKS_PER_SEC;

#define PRINTTIME(id)\
  printf("%d;%lf;\n",n_vertices,time##id);

//Struct queue
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

//Struct Graph
struct Graph {
  int numVertices;
  int* adjMatrix;
  int* visited;
  int max_degree;
};

//Creazione del grafo
struct Graph* createGraph(int vertices) {
  struct Graph* graph = malloc(sizeof(struct Graph));
  graph->numVertices = vertices;

  graph->visited = malloc(vertices * sizeof(int));

  graph->adjMatrix = calloc(vertices * vertices, sizeof(int));

  for (int i = 0; i < vertices; i++) {
    graph->visited[i] = 0;
  }

  return graph;
}

//Aggiungi arco
void addEdge(struct Graph* graph, int src, int dest) {
  //Arco da src a dest
  graph->adjMatrix[src*graph->numVertices + dest] = 1;
  //Arco da src a dest
  graph->adjMatrix[dest*graph->numVertices + src] = 1;
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

//Aggiungi un elemento in coda se non è già presente
int enqueue_if_not_exist(struct queue* q, int value) {
  if (q->rear != q->size - 1)
  {
    if (q->front == -1){
      q->front = 0;
      q->rear++;
      q->items[q->rear] = value;
    }
    else{
      for(int i=q->front; i<=q->rear; i++){
        if(q->items[i] == value) return 0;
      }
      q->rear++;
      q->items[q->rear] = value;
    }
    return 1;
  }
  return 0;
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

//Stampa la coda
void printQueue(struct queue* q) {
  int i = q->front;

  if (isEmpty(q)) {
    printf("Queue is empty\n");
  } else {
    printf("Queue contains \n");
    for (i = q->front; i < q->rear + 1; i++) {
      printf("%d ", q->items[i]);
    }
    printf("\n");
  }
}

//Resetta la coda
void reset_queue(struct queue* q) {
  q->front = -1;
  q->rear = -1;
}

//Numero elementi in coda
int n_element_queue(struct queue* q) {
  if(q->front == -1){
    return 0;
  }
  return ((q->rear - q->front) + 1);
}

int main(int argc, char** argv){
  int size, rank;

  //Inizio MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int* adjMatrix;
  int n_vertices;

  // Concateno il nome parziale dato in ingresso e ottengo due stringe:
  // 1) File dove è presente i dati per il grafo "graph.txt"
  // 2) File dove è presente la bfs di test per verificare il funzionamento "graph_test.txt" 
  char file_graph[SIZE_FILENAME];
  char file_test[SIZE_FILENAME] = ""; 
  char * line = NULL;
  size_t len = 0;

  if(rank == 0){
    strcpy(file_graph, argv[1]);
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

    n_vertices = 0;
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

    adjMatrix = graph->adjMatrix;

    fclose(fp);

    /*
    *
    * Fine fase di lettura da file e creazione grafo
    *
    */
  }

  //Aspetto che il rank 0 finisca di leggere il file
  MPI_Barrier(MPI_COMM_WORLD);

  //Invio ad ogni processo, il numero di vertici presenti nel grafo
  MPI_Bcast(&n_vertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int num_of_vertices_per_proc;
  int avg_num_of_vertices_per_proc = ceil((float)n_vertices/size);

  //Calcolo il primo vertice da considerare per ogni processo
  int first_vertex = rank * avg_num_of_vertices_per_proc;

  //Calcolo il numero di vertici per ogni processo
  if(rank != size - 1){
    num_of_vertices_per_proc = avg_num_of_vertices_per_proc;
  }
  else{
    num_of_vertices_per_proc = n_vertices - (size - 1) * avg_num_of_vertices_per_proc;
  }  

  //Istanzio la sottomatrice di adiacenza del grafo
  int *subAdjMatrix = (int *) malloc(sizeof(int) * avg_num_of_vertices_per_proc * n_vertices);

  //Suddivido la matrice di adiacenza per ogni processo
  MPI_Scatter(adjMatrix, avg_num_of_vertices_per_proc * n_vertices, MPI_INT, subAdjMatrix, avg_num_of_vertices_per_proc * n_vertices, MPI_INT, 0, MPI_COMM_WORLD);

  //Istanzio per ogni processo, l'array contenente il livello relativo ai vari vertici
  int *level = (int *)malloc(sizeof(int) * avg_num_of_vertices_per_proc);

  //Per ogni processo, controllo se sono il proprietario dello START_NODE
  //Se lo sono, allora lo setto come visitato e con valore pari a 1
  for(int i = 0; i < avg_num_of_vertices_per_proc; i++){
      if(first_vertex + i == START_NODE){
          level[i] = 1;
      }
      else
          level[i] = 0;
  }

  //Creo le due code per ogni processo
  struct queue* F = createQueue(n_vertices);
  struct queue* N = createQueue(n_vertices);

  //Creo un array per poter immagazzinare i vari vicini visitati per ogni processo
  struct queue** map = (struct queue**)malloc(sizeof(struct queue*) * size);
  for(int i = 0; i < size; i++){
    map[i] = createQueue(n_vertices);
  }

  int send_buf_size = 0; // dimesione del send_buf
  int recv_buf_size = 0; // dimesione del recv_buf

  int *send_counts; // numero di vicini che saranno inviati ad ogni processore
  int *send_buf; // contiene i dati che servono per essere inviati ad ogni processore
  int *send_disps; // start index dei dati che devono essere inviati ad ogni processore
  int *recv_counts; // numero di vicini da ricevere da ogni processore
  int *recv_buf; // array contenente i dati ricevuti da ogni processore
  int *recv_disps; // displacement dei dati di ogni processore
  int rel_vertex; // vertice relativo per ogni processo

  send_counts = (int *)malloc(sizeof(int) * size);
  send_disps = (int *)malloc(sizeof(int) * size);
  recv_counts = (int *)malloc(sizeof(int) * size);
  recv_disps = (int *)malloc(sizeof(int) * size);

  //Se ci sono altri vertici da esplorare e se terminare la BFS
  int frontier_has_vertices, continue_search;
  int currentVertex;

  //Attesa di tutti i processori prima di iniziare
  MPI_Barrier(MPI_COMM_WORLD);

  //Avvio il timer
  STARTTIME(2)
  //Caso peggiore per ogni vertice
  for(int l = 1; l < n_vertices; l++){
      //Sincronizzo i processori
      MPI_Barrier(MPI_COMM_WORLD);

      //Resetto le code
      reset_queue(F);
      reset_queue(N);
      
      //Metto in coda i vertici di proprietà del processore attuale
      for(int i = 0; i < num_of_vertices_per_proc; i++){
        if(level[i] == l){
          enqueue(F, first_vertex + i);
        }
      }

      //Controllo se il processore ha almeno un vertice da esplorare
      if(isEmpty(F))
          frontier_has_vertices = 0;
      else
          frontier_has_vertices = 1;

      //Raccolgo le informazioni da tutti i processori
      MPI_Allreduce(&frontier_has_vertices, &continue_search, 1, MPI_INT, MPI_LOR, MPI_COMM_WORLD);
      
      //Termina il ciclo se non ci sono vertici da espolare in tutti i processori
      if(!continue_search){
          break;
      }

      //Trova i vicini di ogni vertice del livello attuale
      while(!isEmpty(F)){
        currentVertex = dequeue(F);
        rel_vertex = (currentVertex % avg_num_of_vertices_per_proc);
        for(int i = 0; i < n_vertices; i++){
          if(subAdjMatrix[rel_vertex * n_vertices + i] != 0){
            if(i >= first_vertex && i < first_vertex + avg_num_of_vertices_per_proc){
              if(level[i%avg_num_of_vertices_per_proc] == 0){
                enqueue_if_not_exist(N, i);
              }
            }
            else{
              enqueue_if_not_exist(N, i);
            }
          }
        }
      }

      //Resetto map
      for(int i = 0; i < size; i++){
        reset_queue(map[i]);
      }

      //Inserisco i vertici trovati nell'indice del processore proprietario del vertice
      while(!isEmpty(N)){
        currentVertex = dequeue(N);
        enqueue(map[currentVertex / avg_num_of_vertices_per_proc], currentVertex);
      }

      //Calcolo il displacement e le dimensioni
      for(int i = 0; i < size; i++){
          send_disps[i] = send_buf_size;
          send_buf_size += n_element_queue(map[i]);
          send_counts[i] = n_element_queue(map[i]);
      }

      //Creo il buffer per l'invio
      int k = 0;
      send_buf = (int *)malloc(sizeof(int) * send_buf_size);
      for(int i = 0; i < size; i++){
        for(int j = 0; j < send_counts[i] ; j++){
            send_buf[k] = dequeue(map[i]);
            k++;
        }
      }

      //Invio ad ogni processore il numero di vertici complessivi trovati
      MPI_Alltoall(send_counts, 1, MPI_INT, recv_counts, 1, MPI_INT, MPI_COMM_WORLD);
      
      //Calcolo la dimensione e il displacement dell'array ricevuto
      for(int i = 0; i < size; i++){
          recv_disps[i] = recv_buf_size;
          recv_buf_size += recv_counts[i];
      }

      //Invio ad ogni processore tutti i vertici visitati che sono di loro proprietà
      recv_buf = (int *)malloc(sizeof(int) * recv_buf_size);
      MPI_Alltoallv(send_buf, send_counts, send_disps, MPI_INT,
              recv_buf, recv_counts, recv_disps, MPI_INT, MPI_COMM_WORLD);

      //Smisto i vertici trovati ad ogni processore
      for(int i = 0; i < recv_buf_size; i++){
          rel_vertex = recv_buf[i] % avg_num_of_vertices_per_proc;
          if(level[rel_vertex] == 0)
              level[rel_vertex] = l + 1;
      }

      free(send_buf);
      free(recv_buf);
      send_buf_size = 0;
      recv_buf_size = 0;
  } 
  
  //Salvo il tempo trascorso
  STOPTIME(2)

  //Attendo tutti i processori
  MPI_Barrier(MPI_COMM_WORLD);

  /*
  *
  * Inizio fase di test per la correttezza della BFS
  *
  */
  //Raccolgo tutti i vertici esplorati con i vari livelli da ogni processore
  int* all_level;
  if(rank == 0){
    all_level = (int *) malloc(sizeof(int) * avg_num_of_vertices_per_proc * size);
  }

  //Attendo l'allocazione
  MPI_Barrier(MPI_COMM_WORLD);

  //Raccolgo i dati
  MPI_Gather(level, avg_num_of_vertices_per_proc, MPI_INT, all_level, avg_num_of_vertices_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

  FILE * fp_test;
  int work = 0;
  if(rank == 0)
  {
    char* vertex;
    int actual_level = 0;

    fp_test = fopen(file_test, "r");
    if (fp_test == NULL) exit(EXIT_FAILURE);

    work = 1;
    actual_level = 0;

    while (getline(&line, &len, fp_test) != -1) {
      actual_level++;
      vertex = strtok(line, " ");
      if(all_level[atoi(vertex)] < actual_level){
        #ifdef DEBUG
          printf("Not Work!\n");
        #endif
        work = 0;
        break;
      }
      while(1) {
        vertex = strtok(NULL, " ");
        if(vertex == NULL){
          break;
        }
        if(all_level[atoi(vertex)] < actual_level){
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

    fclose(fp_test);

    if(work == 1){
      PRINTTIME(2)
      #ifdef DEBUG
        printf("\nWork!\n");
      #endif
    }
  }

  //Attendo
  MPI_Barrier(MPI_COMM_WORLD);

  /*
  *
  * Fine fase di test per la correttezza della BFS
  *
  */

  MPI_Finalize();

}