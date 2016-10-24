#include <stdio.h>
#include<stdlib.h>
#include<limits.h>
int V;
int minDistance(int dist[], int sptSet[])
{
   int min = 10000, min_index,v;
   for (v = 0; v < V; v++)
     if (sptSet[v] == 0 && dist[v] <= min)
         min = dist[v], min_index = v;
 
   return min_index;
}
 
int printSolution(int dist[], int n)
{
   printf("Vertex   Distance from Source\n");
   int i;
   for ( i = 0; i < V; i++)
      printf("%d \t\t %d\n", i, dist[i]);
}
 
void dijkstra(int graph[][20], int src)
{
     int dist[V],i;     
     int sptSet[V];  
     for (i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = 0;
 
     dist[src] = 0;
 	int count;
     for ( count = 0; count < V-1; count++)
     {
       int u = minDistance(dist, sptSet);
       sptSet[u] = 1;
       int v;
       for ( v = 0; v < V; v++)
          if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX 
                                       && dist[u]+graph[u][v] < dist[v])
            dist[v] = dist[u] + graph[u][v];
     }
     printSolution(dist, V);
}
 
int main()
{
 	printf("enter the no of nodes ");
	scanf("%d",&V);
	int graph[20][20],i,j;
	for(i=0;i<V;i++)
		for( j=0;j<V;j++)
			scanf("%d", &graph[i][j]);
	dijkstra(graph, 0);
     return 0;
} 
