#include<mpi.h>
#include<stdio.h>
/**
 * @author cristian.chilipirea
 * Run: mpirun -np 12 ./a.out
 */

int graph[][2] = { { 0, 1 }, { 1, 2 }, { 2, 3 }, 
                  { 3, 4 }, { 4, 11 }, { 11, 5 }, 
                  { 5, 6 }, { 6, 7 }, { 7, 8 },
                  { 8, 9 }, { 9, 10 }, { 10, 9 },
                  { 9, 8 }, { 8, 7 }, { 7, 6 },
                  { 6, 5 }, { 5, 11 }, { 11, 4 },
                  { 4, 3 }, { 3, 2 }, { 2, 1 },
                  { 1, 0 }, { 9, 5 }, { 5, 9 },
                  { 5, 3 }, { 3, 5 }, { 0, 2 },
                  { 2, 0 }, { 9, 7 }, { 7, 9 } };
int cont = 0;
int* getNeighbours(int node) {
 	int *neighbours, no = 0;
 	cont = 0;
 	for (int i = 0; i < 30; i ++) {
 		if (graph[i][0] == node)// || graph[i][1] == node)
 			no++;
 	}
 	//no /= 2;
 	neighbours = (int *)malloc(no * sizeof(int));
 	for (int i = 0; i < 30; i ++) {
 		if (graph[i][0] == node)
 			neighbours[cont++] = graph[i][1];
 	}
 	return neighbours;
}

int* getBFS(int root) {
	int *visited = (int *)malloc(12 * sizeof(int));
	int *queue = (int *)malloc(12 * 12 * sizeof(int));
	int start = 0, finish;
	for (int i = 0; i < 12; i ++) {
		visited[i] = -1;
	}
	int *neigh = getNeighbours(root);
	for (int i = 0; i < cont; i ++) {
		visited[neigh[i]] = neigh[i];
		queue[i] = neigh[i];
	}
	visited[root] = root;
	finish = cont;
	while (start <= finish) {
		int *neigh = getNeighbours(queue[start ++]);
		for (int i = 0; i < cont; i ++) {
			if (visited[neigh[i]] == -1) {
				visited[neigh[i]] = visited[queue[start - 1]];
				queue[finish ++] = neigh[i];
			}
		}
	}
	return visited;
}


int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;


	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);
	int *neighbours = getNeighbours(rank);
	for (int i = 0; i < cont; i ++)
		printf("%i is neigh with %i\n", rank, neighbours[i]);
	neighbours = getBFS(rank);
	for (int i = 0; i < 12; i ++)
		printf("Fastest route from %i to %i starts in %i\n", rank, i, neighbours[i]);
	
	printf("Bye from %i/%i\n", rank, nProcesses);
	MPI_Finalize();
	return 0;
}