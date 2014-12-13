#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

/*************************************************
* Discription: Shortest Path Graph
* 
**************************************************/
#define INFINITY 1000
#define LEN 1000
#define TRUE 1
#define FALSE 0

// A structure to represent an adjacency list node
typedef struct AdjListNode
{
    char data;
	int distance; 
    struct AdjListNode* next;
} AdjListNode;
 
// A structure to represent an adjacency list
typedef struct AdjList
{
	char data;
	int visited;
	int weight;
	char previous;
    struct AdjListNode* head;  // pointer to head node of list
} AdjList;
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be the number of vertices in graph.
typedef struct Graph
{
    int NumberOfNodes;
    struct AdjList* array;
} Graph;
 

char queue[10];
char stack[10];
int bIndex, eIndex, qSize, sIndex, sSize;
/*
* Initialize a graph.   
* 
* NodesStr describes the node list in the graph. 
* For example, if NodesStr is "ABCDE", this means that 
* there are 5 nodes in the graph and nodes id is A,
* B, C, D, E respectively.   
*/ 
void InitGraph(Graph* G, char* NodesStr);

/*
* The format of the string is 1st Character = starting node name 2nd
* Character = ending node name Integer = edge length
* 
* For example, the String "AB5" should create 2 nodes, each labeled "A" and
* "B" respectively with a distance between them of 5 units.
*/ 
void AddEdge(Graph* G, char* edgeStr);

/*
* Each character in the String represents an edge traversal over which the
* sum of the edge lengths is to be accumulated.
* 
* The route is defined by providing a sequence of characters that represent
* the nodes of the path.
* 
* For example, if a graph had been created as "AB5", "BC2" then the return
* value for various paths would be:
* 
* "AB" --> 5 "BC" --> 2 "ABC" --> 7 
*/
int CalculateRouteDistance(Graph* G, char* routeNodesStr);

/*
* The format of the fromTo input string is 1st Character = starting node
* name 2nd Character = ending node name
* 
* Any single shortest path from the starting node name to the ending node
* dame. If the graph had been created as "AB5", "BC2", "AD2", "DC5", and
* this method was called with "AC" then both paths "ABC7" and "ADC7" are of
* length 7 and either would be a valid return value.
* 
* The return value should be the node names followed by the length of the
* path.
* 
* Hint: Students can use the Dijkstra algorithm to implement the shortest 
* route on the graph, 
*/
char* CalculateShortestRoute(Graph* G, char* fromTo);

/*
* Destroy a graph.    
*/ 
void DestroyGraph(Graph* G);

void InitQueue(void);
void Enqueue(char i);
void Dequeue(void);
int findWeight(Graph *G, char c);
void setWeight(Graph *G, char c, int weight);
void setVisited(Graph* G, char c);
int isVisited(Graph* G, char c);
void setPrev(Graph* G, char c, char word);
void InitStack(void);
void push(char c);
char pop(void);

int main()
{
	Graph G;
	char path[LEN];
	char fromTo[LEN];

	InitGraph(&G, "ABCDE");
	/*
	AddEdge(&G, "AB5");
	AddEdge(&G, "BC4");
	AddEdge(&G, "CD8");
	AddEdge(&G, "DC8");
	AddEdge(&G, "DE6");
	AddEdge(&G, "AD5");
	AddEdge(&G, "CE2");
	AddEdge(&G, "EB3");
	AddEdge(&G, "AE7");
	*/
	
	AddEdge(&G, "AB5");
	AddEdge(&G, "AD10");
	AddEdge(&G, "BC2");
	AddEdge(&G, "BD3");
	AddEdge(&G, "BE9");
	AddEdge(&G, "CA7");
	AddEdge(&G, "CE6");
	AddEdge(&G, "DE1");
	AddEdge(&G, "DB2");
	


	// Test 1 -- 9
	strcpy(path, "ABC");
	printf("The route distance of %s is %d. \n", path, CalculateRouteDistance(&G, path));

	// Test 2 -- 5
	strcpy(path, "AD");
	printf("The route distance of %s is %d. \n", path, CalculateRouteDistance(&G, path));
	/*
	// Test 3 -- 13
	strcpy(path, "ADC");
	printf("The route distance of %s is %d. \n", path, CalculateRouteDistance(&G, path));
	
	// Test 4 -- 22
	strcpy(path, "AEBCD");
	printf("The route distance of %s is %d. \n", path, CalculateRouteDistance(&G, path));
	*/
	// Test 5 -- "ABC9"
	strcpy(fromTo, "AC");
	printf("The shortest path of %s is %s. \n", fromTo, CalculateShortestRoute(&G, fromTo));
	/*
	// Test 6 -- "BCEB9"
	strcpy(fromTo, "BB");
	printf("The shortest path of %s is %s. \n", fromTo, CalculateShortestRoute(&G, fromTo));
	*/
	
	strcpy(fromTo, "AA");
	printf("The shortest path of %s is %s. \n", fromTo, CalculateShortestRoute(&G, fromTo));

	strcpy(fromTo, "AE");
	printf("The shortest path of %s is %s. \n", fromTo, CalculateShortestRoute(&G, fromTo));
	
	DestroyGraph(&G);
} 


/*
* Initialize a graph.   
* 
* NodesStr describes the node list in the graph. 
* For example, if NodesStr is "ABCDE", this means that 
* there are 5 nodes in the graph and nodes id is A,
* B, C, D, E respectively.   
*/ 
void InitGraph(Graph* G, char* NodesStr)
{
	int i;
	int length;
	length = strlen(NodesStr); //finding lenght of string
	G->array = (AdjList*)malloc(sizeof(AdjList)*length); //allocating memory to array
	for(i = 0; i < length; i++)
	{
		G->array[i].data = NodesStr[i]; //initializing array
		G->array[i].head = (AdjListNode*)calloc(1, sizeof(AdjListNode));
	}
	G->NumberOfNodes = length; //initializing size
	return;
}


/*
* The format of the string is 1st Character = starting node name 2nd
* Character = ending node name Integer = edge length
* 
* For example, the String "AB5" should create 2 nodes, each labeled "A" and
* "B" respectively with a distance between them of 5 units.
*/ 
void AddEdge(Graph* G, char* edgeStr)
{
	int i, stri;
	char buffer[3];
	char cToStr[2];
	AdjListNode *temp, *newnode;
	temp = (AdjListNode*)malloc(sizeof(AdjListNode)*1);
	buffer[0] = '\0';
	cToStr[1] = '\0';
	for(i = 0; i < G->NumberOfNodes; i++)
	{
		if(edgeStr[0] == G->array[i].data) //traversing array to find edgeStr[0]
		{
			if(G->array[i].head->data == NULL)	// for empty list
			{
				G->array[i].head->data = edgeStr[1];
				for(stri = 2; stri < strlen(edgeStr); stri++)
				{
					cToStr[0] = edgeStr[stri];
					strcat(buffer, cToStr);
				}
				//G->array[i].head->distance = edgeStr[2] - 48; //ascii conversion
				G->array[i].head->distance = atoi(buffer);
				break;
			}
			else
			{
				temp = G->array[i].head;	//reference point
				while(temp->next != NULL)	//traverse to last node of list
				{
					temp = temp->next;
				}
				newnode = (AdjListNode*)calloc(1, sizeof(AdjListNode));
				newnode->data = edgeStr[1];
				for(stri = 2; stri < strlen(edgeStr); stri++)
				{
					cToStr[0] = edgeStr[stri];
					strcat(buffer, cToStr);
				}
				//newnode->distance = edgeStr[2] - 48; //ascii conversion
				newnode->distance = atoi(buffer);
				temp->next = newnode;
				break;
			}
		}
	}
}


/*
* Each character in the String represents an edge traversal over which the
* sum of the edge lengths is to be accumulated.
* 
* The route is defined by providing a sequence of characters that represent
* the nodes of the path.
* 
* For example, if a graph had been created as "AB5", "BC2" then the return
* value for various paths would be:
* 
* "AB" --> 5 "BC" --> 2 "ABC" --> 7 
*/
int CalculateRouteDistance(Graph* G, char* routeNodesStr)
{
	int i, j, length, dist = 0;
	AdjListNode *temp;
	temp = (AdjListNode*)malloc(sizeof(AdjListNode)*1);

	length = strlen(routeNodesStr); //finding how many times we need to iterate through the array list

	for(j = 0; j < length-1; j++)
	{
		for(i = 0; i < G->NumberOfNodes; i++)
		{
			if(routeNodesStr[j] == G->array[i].data) //traversing array to find edgeStr[0]
			{
				temp = G->array[i].head;	//reference point
				while(temp->data != routeNodesStr[j+1])	//traverse to find the node with the next letter in routeNodeStr
				{
					temp = temp->next; //traversal
				}
				dist = temp->distance + dist; //adding distance to each node
				break;
			}
		}
	}
	return dist;
}




char* CalculateShortestRoute(Graph* G, char* fromTo)
{
	AdjListNode *temp;
	int i, j, alt, flag = 0;
	char cToStr[2];
	char weight[5];
	char refdest;
	char *output = "";
	char source = fromTo[0];
	char destination = fromTo[1];
	cToStr[1] = '\0';
	InitQueue();
	for(i = 0; i < G->NumberOfNodes; i++)		//creating modified data structure
	{
		G->array[i].previous = NULL;
		G->array[i].weight = INFINITY;
		G->array[i].visited = FALSE;
	}
	Enqueue(source);

	while(qSize > 0)
	{
		for(i = 0; i < G->NumberOfNodes; i++)
		{
			if(queue[bIndex] == G->array[i].data) // found char in adjacency list
			{
				temp = G->array[i].head;
				while(temp->next != NULL)
				{
					alt = G->array[i].weight + temp->distance;
					if(G->array[i].weight == INFINITY)
					{
						alt = temp->distance;
					}
					if(alt < findWeight(G, temp->data))
					{
						setWeight(G, temp->data, alt);
						//cToStr[0] = queue[bIndex];
						setPrev(G, temp->data, queue[bIndex]);
						if(!isVisited(G, temp->data))
							Enqueue(temp->data);
					}
					temp = temp->next;
				}
				alt = G->array[i].weight + temp->distance;		//once more for last element of linked list
				if(G->array[i].weight == INFINITY)
				{
					alt = temp->distance;
				}
				if(alt < findWeight(G, temp->data))
				{
					setWeight(G, temp->data, alt);
					//cToStr[0] = queue[bIndex];
					setPrev(G, temp->data, queue[bIndex]);
					if(!isVisited(G, temp->data))
						Enqueue(temp->data);
				}
				setVisited(G, queue[bIndex]);
				Dequeue();
				break;
			}
		}
	}
	InitStack();
	output[0] = '\0'; // clear output
	push(destination);
	refdest = destination;
	for(j = 0; j < G->NumberOfNodes; j++)
	{
		if(G->array[j].data == refdest)
		{
			if((refdest == source) && flag)
			{
				//push(G->array[j].previous);
				break;
			}
			push(G->array[j].previous);
			refdest = G->array[j].previous;
			j = -1;
			flag = 1;		//defines first iteration
		}
	}
	while(sSize > 0) //backtracks
	{
		cToStr[0] = pop();
		strcat(output, cToStr);
	}
	for(j = 0; j < G->NumberOfNodes; j++)
	{
		if(G->array[j].data == destination)
		{
			sprintf(weight, "%d", G->array[j].weight);
			break;
		}
	}
	strcat(output, weight);
	return output;
}

void InitStack(void)
{
	sIndex = 0;
	sSize = 0;
	return;
}

void push(char c)
{
	stack[sIndex] = c;
	sIndex++;
	sSize++;
}

char pop(void)
{
	sIndex--;
	sSize--;
	return stack[sIndex];
}
void setPrev(Graph* G, char c, char word)
{
	int i;
	for(i = 0; i < G->NumberOfNodes; i++)
	{
		if(c == G->array[i].data)
		{
			G->array[i].previous = word;
			break;
		}
	}
}
void setVisited(Graph* G, char c)
{
	int i;
	for(i = 0; i < G->NumberOfNodes; i++)
	{
		if(c == G->array[i].data)
		{
			G->array[i].visited = TRUE;
			break;
		}
	}
}

int isVisited(Graph* G, char c)
{
	int i;
	for(i = 0; i < G->NumberOfNodes; i++)
	{
		if(c == G->array[i].data)
		{
			return G->array[i].visited;
			break;
		}
	}
}
int findWeight(Graph *G, char c)
{
	int i;
	for(i = 0; i < G->NumberOfNodes; i++)
	{
		if(c == G->array[i].data)
		{
			return G->array[i].weight;
			break;
		}
	}
}

void setWeight(Graph *G, char c, int weight)
{
	int i;
	for(i = 0; i < G->NumberOfNodes; i++)
	{
		if(c == G->array[i].data)
		{
			G->array[i].weight = weight;
			break;
		}
	}
}

void InitQueue(void)
{
	bIndex = 0;
	eIndex = 0;
	qSize = 0;
	return;
}
void Enqueue(char i)
{
	qSize++;
	queue[eIndex] = i;
	eIndex++;
	return;
}

void Dequeue(void)
{
	qSize--;
	bIndex++;
}




/*
* Destroy a graph.    
*/ 
void DestroyGraph(Graph* G)
{
	int i;
	free(G->array);
	G->NumberOfNodes = 0;
	return;
}



