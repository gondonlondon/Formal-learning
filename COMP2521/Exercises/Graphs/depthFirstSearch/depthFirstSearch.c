//A program that utilises depth first search with recursion
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Graph.h"
//My utility function
void DFS(Graph g, int src, int *track) {
	int vertices = GraphNumVertices(g); //O(1)
	track[src] = 1;
	printf("%d ", src);
	int i = 0;
	while (i < vertices) {
		if (!track[i] && GraphIsAdjacent(g, src, i)) {
			DFS(g, i, track);
		}
		i +=1;
	}
}


void depthFirstSearch(Graph g, int src) {
	int vertices = GraphNumVertices(g);
	int *track = malloc(sizeof(int) * vertices);

	//set track to zero
	for (int i = 0; i < vertices; i +=1) {
		track[i] = 0;
	}
	//An array that tracks each node;
	DFS(g, src, track);
	free(track);
}
