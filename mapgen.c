#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 12
#define COLS 8
#define TOTAL 96

char map[ROWS][COLS];
int edges[TOTAL][TOTAL];
int hasNode[ROWS][COLS];

// Compares distance of both columns
int columnDistance(int cA, int cB) {
    if (cA > cB)
		return cA - cB;
	
	return cB - cA;
}

// Generate random number from min to max
int randomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Check if a column already has a node
int columnUsed(int row, int col) {
    return hasNode[row][col];
}

// Generate the number of nodes per row
void generatePositions() 
{
    int c, r, col, allowed, next, ctr, tries, placed, found;
	
	for (r = 0; r < ROWS; r++) 
	{
        while (1) 
		{
			if (r == 0 || r == ROWS - 1) 
            	ctr = randomNumber(3, 5);
        	
			else 
            	ctr = randomNumber(1, COLS);
            
            allowed = 0;
            tries = 0;

			while (tries < 200)
			{
				placed = 0;
				
				for (c = 0; c < COLS; c++) 
                	hasNode[r][c] = 0;
            	
            	while (placed < ctr) 
				{
	                col = randomNumber(0, COLS - 1);
	
	                if (!columnUsed(r, col)) 
					{
	                    hasNode[r][col] = 1;
	                    placed++;
	                }
            	}
            	
            // Check that every node has a possible
            // connection to the next row.
            allowed = 1;

            if (r > 0) 
			{
                for (c = 0; c < COLS; c++) 
				{
                    if (hasNode[r - 1][c]) 
					{
                        found = 0;

                        for (next = 0; next < COLS; next++) 
						{
                            if (hasNode[r][next] &&
                                columnDistance(c, next) <= 2) 
                                found = 1;
                        }

                        if (!found) 
						{
                            allowed = 0;
                            break;
                        }
                    }
                }
            }

		        if (allowed)
		            break;
	                
	        	tries++;
        	}
        	
        	if (allowed)
                break;
        }
    }
}

// Check whether a special node type is allowed
int validType(int row, int col, char type) {

	int i, j;

    if (type == 'R' || type == 'T' || type == '$') 
	{

        // Check nodes in the previous row
        if (row > 0) 
		{
            for (i = 0; i < COLS; i++) 
			{
                if (hasNode[row - 1][i] &&
                    columnDistance(col, i) <= 2 &&
                    map[row - 1][i] == type) 
                    return 0;
            }
        }

        // Check the last row when generating row 10
        if (row == ROWS - 2 && type == 'R') 
		{
            for (j = 0; j < COLS; j++) {
                if (hasNode[ROWS - 1][j] &&
                    columnDistance(col, j) <= 2) 
                    return 0;
            }
        }
    }

    return 1;
}

// Generate random node type based on percentages
char generateNodeType(int row, int col) {
    char type;
	int attempt, chance; 
	
	if (row == 0) 
        return 'E';

    if (row == ROWS - 1) 
        return 'R';

    for (attempt = 0; attempt < 100; attempt++) 
	{
        chance = randomNumber(1, 100);

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
void generateMap() {
	int r, c;
	
    generatePositions();

    for (r = 0; r < ROWS; r++) 
	{
        for (c = 0; c < COLS; c++) 
		{

            if (hasNode[r][c]) 
                map[r][c] = generateNodeType(r, c);
            
			else 
                map[r][c] = ' ';
        }
    }
}

// Generate directed connections
void generateEdges() {
	int r, c, rowEdgeCtr, targetCtr, columnDifference, 
	next, targets[COLS], usedTarget[COLS], maximumEdges,
	edgeCtr, i, chosenColumn, attempts, crosses, j, 
	rowSourceColumns[COLS * 2], rowDestColumns[COLS * 2], 
	maximumRowEdges = COLS * 2, from, to, k;


    for (r = 0; r < ROWS - 1; r++) 
	{
		rowEdgeCtr = 0;
		
        for (c = 0; c < COLS; c++) 
		{
            if (hasNode[r][c]) 
			{
				targetCtr = 0;

                for (next = 0; next < COLS; next++) 
				{
                    if (hasNode[r + 1][next]) 
					{

                    	columnDifference = columnDistance(c, next);

                        if (columnDifference <= 2) 
						{
                            targets[targetCtr] = next;
                            targetCtr++;
                        }
                    }
                }

                if (targetCtr > 0) 
				{
					for (k = 0; k < COLS; k++)
						usedTarget[k] = 0;
					
                    if (targetCtr > 2)
                    	maximumEdges = 2;
                    	
                    else
                    	maximumEdges = targetCtr;
                    	
                    edgeCtr = randomNumber(1, maximumEdges);
                    
                    for (i = 0; i < edgeCtr; i++)
                    {
                    	chosenColumn = -1; 
						attempts = 0;
						crosses = 1;
						
						while (attempts < 20 && (crosses || chosenColumn == -1 || usedTarget[chosenColumn]))
                        {
                            chosenColumn = targets[rand() % targetCtr];
                            crosses = 0;
                            
                            for (j = 0; j < rowEdgeCtr; j++)
                            {
                                if ((rowSourceColumns[j] < c &&
                                     rowDestColumns[j] > chosenColumn) ||
                                    (rowSourceColumns[j] > c &&
                                     rowDestColumns[j] < chosenColumn))
                                    crosses = 1;
                            }

                            attempts++;
                        }
                        
                        if (!crosses && chosenColumn != -1 &&
                            rowEdgeCtr < maximumRowEdges && !usedTarget[chosenColumn])
                        {
                            from = COLS * r + c;
                            to = COLS * (r + 1) + chosenColumn;

                            edges[from][to] = 1;
                            usedTarget[chosenColumn] = 1;
                            rowSourceColumns[rowEdgeCtr] = c;
                            rowDestColumns[rowEdgeCtr] = chosenColumn;
                            rowEdgeCtr++;
                        }
					}
                }
            }
        }
    }
}

// Display the generated map
void displayMap() {
	int r, c;

    printf("\n========== GENERATED MAP ==========\n\n");
    printf("       ");

    for (c = 0; c < COLS; c++) 
        printf("%d   ", c);

    printf("\n");

    for (r = 0; r < ROWS; r++) 
	{
        printf("Row %2d ", r);

        for (c = 0; c < COLS; c++) 
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
void displayAdjacencyList() {
	int r, c, from, next;
	
    printf("\n========== ADJACENCY LIST ==========\n");

    for (r = 0; r < ROWS; r++) 
	{
        for (c = 0; c < COLS; c++) 
		{

            if (hasNode[r][c]) 
			{
				from = r * COLS + c;

                printf("\nNode %d (%c) -> ", from, map[r][c]);

                for (next = 0; next < TOTAL; next++) 
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
void displayAdjacencyMatrix() {
    int activeNodes[TOTAL];
    int count = 0;
    int r, c, i, j, k;

    for (r = 0; r < ROWS; r++) 
	{
        for (c = 0; c < COLS; c++) 
		{

            if (hasNode[r][c]) 
			{
                activeNodes[count] = r * COLS + c;
                count++;
            }
        }
    }

    printf("\n========== ADJACENCY MATRIX ==========\n");
    printf("     ");

    for (i = 0; i < count; i++) 
        printf("%3d ", activeNodes[i]);

    printf("\n");

    for (j = 0; j < count; j++) 
	{
        printf("%3d  ", activeNodes[j]);

        for (k = 0; k < count; k++) 
            printf("%3d ", edges[activeNodes[j]][activeNodes[k]]);

        printf("\n");
    }
}

int main() {

    srand((unsigned int)time(NULL));

    generateMap();
    generateEdges();

    displayMap();
    displayAdjacencyList();
    displayAdjacencyMatrix();

    return 0;
}
