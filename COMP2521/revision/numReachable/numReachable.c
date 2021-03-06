
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

#define INF 0xFFFFF

//A helper function to fetch an unexplored vertex with the smallest dist
//that is adjacent to the src vertex
int unexploredSet(Graph g, int *hasVisited, int *dist) {
	int nV = GraphNumVertices(g);
	int currDist = INF;
	int v = 0;
	for (int i = 0; i < nV; i +=1) {
		if (dist[i] < currDist && !hasVisited[i]) {
			v = i;
			currDist = dist[i];
		}
	}
	return v;
}

int numReachable(Graph g, int src) {
	int nV = GraphNumVertices(g);
	
	int *hasVisited = (int *)calloc(nV, sizeof(int));
	int *dist = (int *)calloc(nV, sizeof(int));
	for (int i = 0; i < nV; i +=1) {
		dist[i] = INF;
	}
	//Set the src distance to zero
	dist[src] = 0;
	
	for (int i = 0; i < nV; i +=1) {
		int curr = unexploredSet(g, hasVisited, dist);
		hasVisited[curr] = 1;
		
		for (int w = 0; w < nV; w +=1) {
			int newDist = dist[curr] + GraphIsAdjacent(g, curr, w);
			if (newDist < dist[w] && GraphIsAdjacent(g, curr, w) 
			    && !hasVisited[w]) {
		    	dist[w] = newDist;
			}
			
		}
		
	}
	
	//Count the number of vertices that are reachable
	int reachable = 0;
	for (int i = 0; i < nV; i +=1) {
		if (dist[i] != INF) {
			reachable +=1;
		}
	}
	free(dist);
	free(hasVisited);
	
	return reachable;
}




















