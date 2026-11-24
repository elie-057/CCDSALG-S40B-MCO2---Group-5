#include <stdio.h>
#include <stdlib.h>

#define ROWS 12
#define COLUMNS 8
#define SUM_NODES 96 //ROWS * COLUMNS

typedef enum
{
	EMPTY = -1,
	ENEMY = 0,
	MERCHANT = 4,
	MYSTERY = 1,
	REST = 3,
	TREASURE = 2
} nodeType;

typedef struct
{
	int column, id, outgo[COLUMNS], outgoCtr, row;
	nodeType nt;
} node;

typedef struct
{
	int isNode[ROWS][COLUMNS];
	node N[ROWS][COLUMNS];
} map;

typedef struct adjacencyListNode
{
	int destinationID;
	struct adjacencyListNode* next;
} aln;

typedef struct
{
	aln* head;
} adjList;

nodeType obtainArbitraryNodeType()
{
	int pick = rand() % 100;
	
	if (pick < 40)
		return ENEMY;
		
	else if (pick < 65)
		return MYSTERY;
		
	else if (pick < 80)
		return TREASURE;
		
	else if (pick < 95)
		return REST;
		
	else
		return MERCHANT;
}

const char* obtainNodeTypeName(nodeType nt)
{
	switch (nt)
	{
		case ENEMY: return "Enemy";
		case MERCHANT: return "Merchant";
		case MYSTERY: return "Mystery";
		case REST: return "Rest";
		case TREASURE: return "Treasure";
		default: return "Empty";
	}
}

void initiateMap(map* m)
{
	int r, c, ctrID = 0;
	
	for (r = 0; r < ROWS; r++)
	{
		for (c = 0; c < COLUMNS; c++)
		{
			m->isNode[r][c] = 0;
			m->N[r][c].id = ctrID++;
            m->N[r][c].row = r;
            m->N[r][c].column = c;
            m->N[r][c].nt = EMPTY;
            m->N[r][c].outgoCtr = 0;
		}
	}
}

void createMap(map* m)
{
	int nodesFirstRow = (rand() % 3) + 3;
	int usedColumns[COLUMNS] = {0};
	
	for (int i = 0; i < nodesFirstRow; i++) 
	{
		int c;
		
		do
		{
			c = rand() % COLUMNS;
		} while (usedColumns[c]);

        usedColumns[c] = 1;
        m->isNode[0][c] = 1;
        m->N[0][c].nt = ENEMY;
    }
    
    for (int r = 1; r < ROWS - 1; r++) 
	{
        int nodes = (rand() % COLUMNS) + 1;
        
        for (int c = 0; c < COLUMNS; c++) 
			usedColumns[c] = 0;

        for (int i = 0; i < COLUMNS && i < nodes; i++) 
		{
            int c;
            
            do 
			{ 
				c = rand() % COLUMNS; 
			} while (usedColumns[c]);
			
            usedColumns[c] = 1;

            m->isNode[r][c] = 1;

            nodeType nt;
            int allowed = 0, tries = 0;

            do 
			{
                nt = obtainArbitraryNodeType();
                
                if (nt == MERCHANT || nt == REST || nt == TREASURE) 
				{
                    int straightFound = 0;
                    
                    if (c > 0 && m->isNode[r][c - 1] && m->N[r][c - 1].nt == nt) 
						straightFound = 1;
						
                    if (r > 0 && m->isNode[r - 1][c] && m->N[r - 1][c].nt == nt) 
						straightFound = 1;

                    if (tries > 10 || !straightFound) 
						allowed = 1;
                } 
				
				else 
                    allowed = 1;
            
                tries++;
            } while (!allowed);

            m->N[r][c].nt = nt;
        }
    }
    
    int nodesLastRow = (rand() % 3) + 3;
    
    for (int c = 0; c < COLUMNS; c++) 
		usedColumns[c] = 0;

    for (int i = 0; i < nodesLastRow; i++) 
	{
        int c;
        
        do 
		{ 
			c = rand() % COLUMNS; 
		} while (usedColumns[c]);
		
        usedColumns[c] = 1;

        m->isNode[ROWS - 1][c] = 1;
        m->N[ROWS - 1][c].nt = REST;
    }
}

void makeSides(map* m) 
{
	int maximumRowEdges = COLUMNS * 2;
	
    for (int r = 0; r < ROWS - 1; r++) 
	{
		int rowSourceColumns[COLUMNS * 2];
		int rowDestinationColumns[COLUMNS * 2];
		int rowEdgeCtr = 0;
		
        for (int c = 0; c < COLUMNS; c++) 
		{
            if (m->isNode[r][c]) 
			{
				int potentialTargets[COLUMNS];
                int nextRow = r + 1;
                int targetCtr = 0;

                for (int i = 0; i < COLUMNS; i++) 
				{
                    if (m->isNode[nextRow][i]) 
					{
                        int columnDifference;
                        
                        if (c > i)
                        	columnDifference = c - i;
                        	
                        else
                        	columnDifference = i - c;
                        
                        if (columnDifference <= 2)  
                            potentialTargets[targetCtr++] = i;
                    }
                }

                if (targetCtr > 0) 
				{
                    int maximumEdges;
                    
                    if (targetCtr > 2) 
                        maximumEdges = 2;
                        
                    else 
                        maximumEdges = targetCtr;
                    
                    int makeEdges = (rand() % maximumEdges) + 1;
                    int chosenTargets[COLUMNS] = {0};

                    for (int e = 0; e < makeEdges; e++) 
					{
                        int chosenIndex, tries = 0, crosses = 0;
                        
                        do 
						{
							crosses = 0;
                            chosenIndex = potentialTargets[rand() % targetCtr];
                            
                            for (int f = 0; f < rowEdgeCtr; f++)
                            {
                            	if ((rowSourceColumns[f] > c && rowDestinationColumns[f] < chosenIndex) || (rowSourceColumns[f] < c && rowDestinationColumns[f] > chosenIndex))
                            		crosses = 1;
							}
                            
                            tries++;
                        } while (tries < 15 && (crosses || chosenTargets[chosenIndex]));

                        if (rowEdgeCtr < maximumRowEdges && !crosses && m->N[r][c].outgoCtr < COLUMNS && !chosenTargets[chosenIndex]) 
						{
                            chosenTargets[chosenIndex] = 1;
                            rowSourceColumns[rowEdgeCtr] = c;
                            rowDestinationColumns[rowEdgeCtr] = chosenIndex;
                            rowEdgeCtr++;
                            
                            int destinationID = m->N[nextRow][chosenIndex].id;
                            m->N[r][c].outgo[m->N[r][c].outgoCtr++] = destinationID;
                        }
                	}
				}
         	}
		}
	}
}

void displayNodeTable(map* m) 
{
    printf("----- Node Position & Type Table -----\n");
    printf("%s | %s | %s | %s\n", "Node ID", "Row", "Col", "Type");

    for (int r = 0; r < ROWS; r++) 
	{
        for (int c = 0; c < COLUMNS; c++) 
		{
            if (m->isNode[r][c]) 
			{
                node n = m->N[r][c];
                printf("ID: %02d | R: %02d  | C: %02d  | %s\n", n.id, n.row + 1, n.column + 1, obtainNodeTypeName(n.nt));
            }
        }
    }
    
    printf("\n");
}

void makeAdjacencyMatrix(map* m, int am[SUM_NODES][SUM_NODES]) 
{
    for (int i = 0; i < SUM_NODES; i++) 
	{
        for (int j = 0; j < SUM_NODES; j++) 
            am[i][j] = 0;
    }

    for (int r = 0; r < ROWS; r++) 
	{
        for (int c = 0; c < COLUMNS; c++) 
		{
            if (m->isNode[r][c]) 
			{
                node n = m->N[r][c];
                int sourceID = n.id;
                
                for (int i = 0; i < n.outgoCtr; i++) 
				{
                    int destinationID = n.outgo[i];
                    am[sourceID][destinationID] = 1;
                }
            }
        }
    }
}

void createAdjacencyList(map* m, adjList al[SUM_NODES]) 
{
    for (int i = 0; i < SUM_NODES; i++) 
        al[i].head = NULL;

    for (int r = 0; r < ROWS; r++) 
	{
        for (int c = 0; c < COLUMNS; c++) 
		{
            if (m->isNode[r][c]) 
			{
                node n = m->N[r][c];
                int sourceID = n.id;

                for (int i = 0; i < n.outgoCtr; i++) 
				{
                    int destinationID = n.outgo[i];

                    struct adjacencyListNode* newNode = (struct adjacencyListNode*)malloc(sizeof(struct adjacencyListNode));
                    if (newNode != NULL)
                    {
                    	newNode->destinationID = destinationID;
                    	newNode->next = al[sourceID].head;
                    	al[sourceID].head = newNode;
					}
                }
            }
        }
    }
}

void showAdjacencyMatrix(int M[SUM_NODES][SUM_NODES]) 
{
    printf("----- Adjacency Matrix (Sample 15x15) -----\n");
    printf("Note: Displaying the first 15 nodes for precise console result\n");

    int max = 15;
    
    for (int i = 0; i < max; i++) 
		printf("%3d ", i);

    printf("\n");

    for (int i = 0; i < max; i++) 
	{
        printf("%2d |", i);
        
        for (int j = 0; j < max; j++) 
            printf("%3d ", M[i][j]);
            
        printf("\n");
    }
    
    printf("\n");
}

void showAdjacencyList(adjList al[SUM_NODES]) 
{
    printf("----- Adjacency List -----\n");

    for (int i = 0; i < SUM_NODES; i++) 
	{
        if (al[i].head != NULL) 
		{
            printf("Node %2d -> ", i);
            struct adjacencyListNode* temp = al[i].head;
            
            while (temp != NULL) 
			{
                printf("[ID: %d] ", temp->destinationID);
                temp = temp->next;
            }
            
            printf("\n");
        }
    }
    
    printf("\n");
}

void discardAdjacencyList(adjList al[SUM_NODES]) 
{
    for (int i = 0; i < SUM_NODES; i++) 
	{
        struct adjacencyListNode* now = al[i].head;
        
        while (now != NULL) 
		{
            struct adjacencyListNode* nextNode = now->next;
            free(now);
            now = nextNode;
        }
    }
}

int main()
{
	map m;
	
	printf("--------------------\n");
	printf("<<< Slay the Spire >>>\n");
	printf("--------------------\n\n");
	
	initiateMap(&m);
	createMap(&m);
	makeSides(&m);
	
	displayNodeTable(&m);
	
	int adjacencyMatrix[SUM_NODES][SUM_NODES];
	makeAdjacencyMatrix(&m, adjacencyMatrix);
	showAdjacencyMatrix(adjacencyMatrix);
	
	adjList adjacencyList[SUM_NODES];
	createAdjacencyList(&m, adjacencyList);
	showAdjacencyList(adjacencyList);
	discardAdjacencyList(adjacencyList);
	
	return 0;
}
