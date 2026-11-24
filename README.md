# CCDSALG S40B MCO2 - Group 5
Overview: The GitHub repository link for S40B Group 5's MCO2 in "Data Structures and Algorithms" (CCDSALG).

Group 5 Members - S40B:
Dwayne Cipriano
Elijah Rexson Mayor
Mark Stephen Nucum

Description: This constructs a Slay the Spire map generator, traversal system, and graph representation in the C programming language over an 8-column by 12-row grid with hindered node placement, non-crossing forward edges, and weighted event probabilities. Across its whole scope, it depicts the generated map through the Node Position & Type Table, Adjacency Matrix, and Adjacency List, utilizes DFS and BFS algorithms to evaluate target routes (most treasure, most mystery, most dangerous, and safest), and compares their execution runtimes.

Expected Output: 

1. Map Generation & Node Position/Type Table:
   - Shows active nodes with their Node ID, Row, Column, and Node Type (Enemy, Mystery, Treasure, Rest, or Merchant).
   - Implements 3–5 Enemy nodes on the first row, 3–5 Rest nodes on the last row, weighted type probabilities (40% Enemy, 25% Mystery, 15% Treasure, 15% Rest, 5% Merchant), non-repetitive neighboring Merchant/Rest/Treasure nodes, and non-crossing forward edges.

2. Graph Representations (Adjacency Matrix & Adjacency List):
   - Adjacency Matrix: Displays a matrix where 1 suggests a directed edge from a source node ID to a destination node ID, while 0 suggests no direct edge.
   - Adjacency List: Shows each active node that has outgoing edges together with its linked destination node IDs, followed by memory deallocation.

3. DFS & BFS Traversal Results:
   - States the resulting paths and node counts for each target objective:
     Most Treasure nodes
     Most Mystery nodes
     Safest route (fewest Enemy nodes)
     Most Dangerous route (most Enemy nodes)

4. Comparative Runtime Summary:
   - Displays side-by-side execution iterations comparing DFS and BFS across the generated map and traversal queries.
