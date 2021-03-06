// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include "Graph.h"
#include "GraphRead.h"

#define LOWPRIORITY 0x7FFFFFFF

/////////////////////////////////////////////////////////////////////////
///Function prototypes:
int reachableNodes (ShortestPaths sp, int v);
int sumOfAllDist (ShortestPaths sp, int v);
int addPredNode (ShortestPaths sp, PQ pq, int v);
int numPred (PredNode *head);
int isPred (PredNode *head, int v);
int numShortPathST (ShortestPaths sp, int s, int t, int **hasVisitedST);
int numShortPathDFS (ShortestPaths sp, int s, int t);
int numShortPathVia (ShortestPaths sp, int s, int t, int via);


/////////////////////////////////////////////////////////////////////////

//O(n^3)
NodeValues closenessCentrality(Graph g) {
	NodeValues nvs;
    nvs.numNodes = GraphNumVertices(g);
    nvs.values = malloc(nvs.numNodes * sizeof(double));

    //set array of values to 0.0 for safety
    for (int i = 0; i < nvs.numNodes; i +=1) {
        nvs.values[i] = 0.0;
    }

    //Find the centrality value for all nodes
    int v = 0;
    
    //This loop is O(n)
    while (v < nvs.numNodes) {
        ShortestPaths sp = dijkstra(g, v); //O(n^2)
        //Fetch the number of vertices reachable from v
        //and the sum of all dist from v

        int reachable = reachableNodes(sp, v); //O(n)
        int sum = sumOfAllDist(sp, v); //O(n)

        //Check if reachable or sum is zero
        //Ensure N-1 is not zero!
        if (sum && reachable && (nvs.numNodes - 1)) {
            double x = 1.0 * reachable / (nvs.numNodes - 1);
            double y = 1.0 * reachable / sum;
            nvs.values[v] = x*y;
        }
        //If N-1 == 0, there is only one vertex
        //If n-1 == 0, no reachable nodes
        else {
            nvs.values[v] = 0.0;
        }
        //Free sp
        freeShortestPaths(sp);
        v +=1;
    }

	return nvs;
}

//This function has a complexity of O(n^5),
//because there are three loops that index the graph with n vertices
//resulting in a complexity of O(n^3) and
//the helper functions have a complexity of O(n^2).
//Ergo, this function is O(n^5)
NodeValues betweennessCentrality(Graph g) {
    NodeValues nvs;
    nvs.numNodes = GraphNumVertices(g);
    nvs.values = malloc(nvs.numNodes * sizeof(double));
    //st = source target 2d array
    int **hasVisitedST = (int**) calloc(nvs.numNodes, sizeof(int*));

    for (int i = 0; i < nvs.numNodes; i +=1) {
        hasVisitedST[i] = calloc(nvs.numNodes, sizeof(int));
    }

    //set array of values to 0.0 for safety
    for (int i = 0; i < nvs.numNodes; i +=1) {
        nvs.values[i] = 0.0;
    }
    int s = 0; 
    
    //This loop is O(n)
    while (s < nvs.numNodes) {
        ShortestPaths sp = dijkstra(g, s);
        //Loop through the pred list,

        int t = 0;
        //This loop is O(n)
        while (t < nvs.numNodes) {

            //Ensure target is not isolated and is not the source
            if (sp.dist[t] && t != s) {
                //FETCH number of shortest paths for each target node,
                int *hasVisitedV = calloc(nvs.numNodes, sizeof(int));
                PQ pq = PQNew();
                addPredNode(sp, pq, t);
                //OBTAIN the number of paths that pass through v
                //from node s to t 
                //By backtracking from t and returning to s.
                //Use BFS to backtrack through to s
                //and add edges on the shortest path
                //Don't double count paths..

                int numSP = numShortPathST(sp, s, t, hasVisitedST);//O(n^2)

                //This loop is O(n) at worst
                //if the graph is incredibly dense aka connected
                while (!PQIsEmpty(pq)) {
                    int v = PQDequeue(pq);
                    if (v != s && v != t && !hasVisitedV[v]) {
                        
                        //O(n^2);
                        int numPaths = numShortPathVia(sp, s, t, v);
                        //calculate centrality for each node on the path
                        //from s to t
                        nvs.values[v] += 1.0 * numPaths / numSP;
                        addPredNode(sp, pq, v);
                        hasVisitedST[s][t] = 1;
                        hasVisitedV[v] = 1;
                    }
                }
                PQFree(pq);
                free(hasVisitedV);
            }
            t +=1;
        }
        freeShortestPaths(sp);
        s +=1;
    }

    for (int i = 0; i < nvs.numNodes; i +=1) {
        free(hasVisitedST[i]);
    }
    free(hasVisitedST);
	return nvs;
}

//O(n^5)
NodeValues betweennessCentralityNormalised(Graph g) {
	NodeValues nvs = betweennessCentrality(g);//O(n^5)
    int v = 0;
    while (v < nvs.numNodes) {
        ShortestPaths sp = dijkstra(g, v);
        int n = nvs.numNodes;
        nvs.values[v] *= 1.0/(n - 1) * 1.0/(n - 2);
        freeShortestPaths(sp);
        v +=1;
    }

	return nvs;
}

//Print out the NodeValues struct
void showNodeValues(NodeValues nvs) {
    for (int i = 0; i < nvs.numNodes; i +=1) {
        printf("%d: %lf\n", i, nvs.values[i]);
    }
}

void freeNodeValues(NodeValues nvs) {
    free(nvs.values);
}



/////////////////////////////////////////////////////////////////////////
///Helper functions

//A function that returns the number of nodes reachable from v in a graph
//Excluding the current vertex (n)
int reachableNodes (ShortestPaths sp, int v) {
    int i = 0;
    int reachable = 0;
    while (i < sp.numNodes) {
        //Don't add vertices which are unreachable
        //or the src vertex
        if (i != v && sp.dist[i]) {
            reachable += 1;
        }
        i +=1;
    }
    return reachable;
}

//A function that sums all the distance reachable from v in a graph

//Isolated vertices and unreachable have been covered,
//they are marked as zero
int sumOfAllDist (ShortestPaths sp, int v) {
    int sum = 0;
    for (int i = 0; i < sp.numNodes; i +=1) {
        sum += sp.dist[i];
    }
    return sum;
}

//A function which enqueues predecessor vertices
int addPredNode (ShortestPaths sp, PQ pq, int v) {
    PredNode *curr = sp.pred[v];
    int i = 0;
    while (curr != NULL) {
        PQInsert(pq, curr->v, LOWPRIORITY);
        curr = curr->next;
        i +=1;
    }
    return i;
}

//A function that returns the number of predecessors to a node
int numPred (PredNode *head) {
    PredNode *curr = head;
    int sum = 0;
    while (curr != NULL) {
        sum +=1;
        curr = curr->next;
    }
    return sum;
}

//A function that checks if node v is a predecessor of node t
//returns 1 if true, false otherwise
int isPred (PredNode *head, int v) {
    PredNode *curr = head;
    while (curr != NULL) {
        if (curr->v == v) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}


//A function that returns the sum of all the shortest paths
//By backtracking from t to s using BFS
//Number of Shortest Path Source Target (numShortPathST)
//O(n^2)
int numShortPathST (ShortestPaths sp, int s, int t, int **hasVisitedST) {
    int count = 1;
    PQ pq = PQNew();
    addPredNode(sp, pq, t);
    int vPrev = t;
    while (!PQIsEmpty(pq)) {
        int v = PQDequeue(pq);
        //Add the number of possible paths by checking the predecessor node
        int backTrack = numPred(sp.pred[vPrev]) - 1;
        //v or vPrev?
        if (backTrack > 0 && (!hasVisitedST[vPrev][t] 
                          || !hasVisitedST[s][t] ) 
                          ) {
            count +=backTrack;
        }
        vPrev = v;
        addPredNode(sp, pq, v);
    }
    PQFree(pq);
    return count;
}
//A function that returns the sum of all the shortest paths
//By backtracking from via to s using DFS
//O(n^2)
int numShortPathDFS (ShortestPaths sp, int s, int t) {
    int count = 0;
    PredNode *currTar = sp.pred[t];
    
    //base case:
    //Successful alternate path
    if (t == s) {
        return 1;
    }

    //Recursive case:
    //Continue searching
    while (currTar != NULL) {
        count += numShortPathDFS(sp, s, currTar->v);
        currTar = currTar->next;
    }
    return count;
}

//Return the number of paths that go through the node via from target to src.
//Use a pincer movement, (v to s) * (s to v) will give us the total
//number of paths
//O(n^2)
int numShortPathVia (ShortestPaths sp, int s, int t, int via) {
    return numShortPathDFS(sp, s, via) * numShortPathDFS(sp, via, t);
}
