#!/bin/python3
import networkx as nx
import numpy as np 

MIN_DEGREE = 3
MAX_DEGREE = 6

Graph = None

n = int(input("Number of peers : "))
while True:
    degree_secuence = np.random.randint(low=MIN_DEGREE, high=MAX_DEGREE + 1, size= n)
    if nx.is_graphical(degree_secuence):
        Graph = nx.random_degree_sequence_graph(degree_secuence, seed= 44)
        if nx.is_connected(Graph):
            break;

edges = Graph.edges()

for e in edges:
    n1, n2 = e
    print(n1 + 1, n2 + 1)

