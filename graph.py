#!/bin/python3
import networkx as nx
import numpy as np 
import sys

MIN_DEGREE = 3
MAX_DEGREE = 6

Graph = None

if(len(sys.argv) != 2):
    print("./graph.py <no of peer>")
    exit(1)

n = int(sys.argv[1])
while True:
    degree_sequence = np.random.randint(low=MIN_DEGREE, high=MAX_DEGREE + 1, size= n)
    if nx.is_graphical(degree_sequence):
        Graph = nx.random_degree_sequence_graph(degree_sequence)
        if nx.is_connected(Graph):
            break;

edges = Graph.edges()
print(n, len(edges))
for e in edges:
    n1, n2 = e
    print(n1 , n2)

