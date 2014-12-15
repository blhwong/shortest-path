shortest-path
=============
Created by: Brandon Wong
Creation Date: 12/2014

implements directed acylic graphs

computes the distance between each node through function CalculateRouteDistance

computes shortest path using Dijkstra's algorithm through function CalculateShortestRoute

Input Specifications:
Passes Graph and Node keyword parameters into the InitGraph function. 
Add the edge specifying which keywords and edge weight using AddEdge function.
Pass in the node path to calculate the distance in the CalculateRouteDistance function.
Pass in the two nodes you want to calculate shortest path in the CalculateShortestPath function.

Output specifications:
CalculateRouteDistance passes back the int value of the accumulated weight.
CalculateShortestDistance passes back the String of the shortest path. String includes Path and distance.

Concepts used: adjacency lists, stacks, queues, Dijkstra's algorithm