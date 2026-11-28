#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 12
#define COLS 8
#define TOTAL 96

char map[ROWS][COLS];
int edges[TOTAL][TOTAL];
int hasNode[ROWS][COLS];

// Generate random number from min to max
int randomNumber(int min, int max) 
{
    return min + rand() % (max - min + 1);
}

// Check if a column already has a node
int columnUsed(int row, int col) 
{
    return hasNode[row][col];
}

// Generate the number of nodes per row
void generatePositions() 
{
    for (int r = 0; r < ROWS; r++) 
	{

        int count;

        if (r == 0 || r == ROWS - 1) 
            count = randomNumber(3, 5);
            
        else
            count = randomNumber(1, COLS);

        while (1) 
		{
            int placed = 0;

            for (int c = 0; c < COLS; c++) 
                hasNode[r][c] = 0;
            

            while (placed < count) 
			{
                int col = randomNumber(0, COLS - 1);

                if (!columnUsed(r, col)) 
				{
                    hasNode[r][col] = 1;
                    placed++;
                }
            }

            // Check that every node has a possible
            // connection to the next row.
            int valid = 1;

            if (r > 0) 
			{
                for (int c = 0; c < COLS; c++) 
				{
                    if (hasNode[r - 1][c]) 
					{
                        int found = 0;

                        for (int next = 0; next < COLS; next++) 
						{
                            if (hasNode[r][next] && abs(c - next) <= 2) 
                                found = 1;
                        }

                        if (!found) 
						{
                            valid = 0;
                            break;
                        }
                    }
                }
            }

            if (valid) 
			{
                break;
            }
        }
    }
}

// Check whether a special node type is allowed
int validType(int row, int col, char type) 
{

    if (type == 'R' || type == 'T' || type == '$') 
	{

        // Check nodes in the previous row
        if (row > 0) 
		{
            for (int c = 0; c < COLS; c++) 
			{
                if (hasNode[row - 1][c] && abs(col - c) <= 2 && map[row - 1][c] == type) 
                    return 0;
            }
        }

        // Check the last row when generating row 10
        if (row == ROWS - 2 && type == 'R') 
		{
            for (int c = 0; c < COLS; c++) 
			{
                if (hasNode[ROWS - 1][c] && abs(col - c) <= 2) 
                    return 0;
            }
        }
    }

    return 1;
}

// Generate random node type based on percentages
char generateNodeType(int row, int col) 
{

    if (row == 0) 
        return 'E';

    if (row == ROWS - 1) 
        return 'R';

    for (int attempt = 0; attempt < 100; attempt++) 
	{

        int chance = randomNumber(1, 100);
        char type;

        if (chance <= 40) 
            type = 'E';
            
        else if (chance <= 65) 
            type = '?';
            
        else if (chance <= 80) 
            type = 'T';
            
        else if (chance <= 95) 
            type = 'R';
            
        else 
            type = '$';

        if (validType(row, col, type)) 
            return type;
    }

    // Default type if no valid type is selected
    return 'E';
}

// Generate all node types
void generateMap() 
{
    generatePositions();

    for (int r = 0; r < ROWS; r++) 
	{
        for (int c = 0; c < COLS; c++) 
		{

            if (hasNode[r][c])
                map[r][c] = generateNodeType(r, c);
            
			else 
                map[r][c] = ' ';
        }
    }
}

// Generate directed connections
void generateEdges() 
{
	int maximumRowEdges = 2 * COLS;
	
    for (int r = 0; r < ROWS - 1; r++) 
	{
		int rowSourceCols[maximumRowEdges];
        int rowDestCols[maximumRowEdges];
        int rowEdgeCtr = 0;
		
        for (int c = 0; c < COLS; c++) 
		{

            if (hasNode[r][c]) 
			{
				int targetCtr = 0;
				int targets[COLS];

                for (int next = 0; next < COLS; next++) 
				{

                    if (hasNode[r + 1][next]) 
					{
                        int difference;
                        
                        if (c > next)
                        	difference = c - next;
                        	
                        else
                        	difference = next - c;

                        if (difference <= 2) 
                            targets[targetCtr++] = next;
                    }
                }
                
                if (targetCtr > 0)
                {
                	int maximumEdges;
                	
                	if (targetCtr > 2)
                		maximumEdges = 2;
                		
                	else 
                		maximumEdges = targetCtr;
                		
                	int edgeCtr = (rand() % maximumEdges) + 1;
                	int nodeEdges = 0;
                	int usedTarget[COLS] = {0};
                	
                	for (int i = 0; i < edgeCtr; i++)
                	{
                		int cross = 0;
                		int tries = 0;
                		int col = -1;
                		
                		do
                		{
                			cross = 0;
                			col = targets[rand() % targetCtr];
                			
                			for (int j = 0; j < rowEdgeCtr; j++)
                			{
                				if ((rowDestCols[j] > col && rowSourceCols[j] < c) || (rowDestCols[j] < col && rowSourceCols[j] > c))                				
                					cross = 1;
							}
							
							tries++;
						} while ((usedTarget[col] || cross) && tries < 15);
						
						if (nodeEdges < COLS && !cross && !usedTarget[col] && rowEdgeCtr < maximumRowEdges)
						{
							int from = COLS * r + c;
							int to = COLS * (r + 1) + col;
							
							usedTarget[col] = 1;
							rowSourceCols[rowEdgeCtr] = c;
							rowDestCols[rowEdgeCtr] = col;
							rowEdgeCtr++;
							nodeEdges++;
							
							edges[from][to] = 1;
						}
					}	
				}
            }
        }
    }
}

// Display the generated map
void displayMap() 
{ 
    printf("\n========== GENERATED MAP ==========\n\n");

    printf("       ");

    for (int c = 0; c < COLS; c++) 
        printf("%d   ", c);

    printf("\n");

    for (int r = 0; r < ROWS; r++) 
	{
        printf("Row %2d ", r);

        for (int c = 0; c < COLS; c++) 
		{

            if (hasNode[r][c]) 
                printf("[%c] ", map[r][c]);
                
            else 
                printf(" .  ");
        }

        printf("\n");
    }

    printf("\nNode Legend:\n");
    printf("E = Enemy\n");
    printf("? = Mystery\n");
    printf("T = Treasure\n");
    printf("R = Rest\n");
    printf("$ = Merchant\n");
}

// Display adjacency list
void displayAdjacencyList() 
{
    printf("\n========== ADJACENCY LIST ==========\n");

    for (int r = 0; r < ROWS; r++) 
	{
        for (int c = 0; c < COLS; c++) 
		{

            if (hasNode[r][c]) 
			{

                int from = r * COLS + c;

                printf("\nNode %d (%c) -> ", from, map[r][c]);

                for (int next = 0; next < TOTAL; next++) 
				{
                    if (edges[from][next] == 1) 
                        printf("%d ", next);
                }
            }
        }
    }

    printf("\n");
}

// Display adjacency matrix using active nodes only
void displayAdjacencyMatrix() 
{
	int n = 15;

    printf("\n========== ADJACENCY MATRIX ==========\n");

    printf("     ");

    for (int i = 0; i < n; i++) 
        printf("%3d ", i);

    printf("\n");

    for (int j = 0; j < n; j++) 
	{

        printf("%3d  ", j);

        for (int k = 0; k < n; k++) 
            printf("%3d ", edges[j][k]);

        printf("\n");
    }
}

int main() 
{

    srand(time(NULL));

    generateMap();
    generateEdges();

    displayMap();
    displayAdjacencyList();
    displayAdjacencyMatrix();

    return 0;
}
