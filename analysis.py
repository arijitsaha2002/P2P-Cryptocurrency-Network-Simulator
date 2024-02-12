#!/bin/python3
import argparse
import os
import pandas as pd
import networkx as nx
from networkx.drawing.nx_agraph import graphviz_layout
import matplotlib.pyplot as plt


def make_blockchain_tree(filename):
    T = nx.Graph()
    file = pd.read_csv(filename)

    for a, b in zip(file.id, file.pid):
        if( a < 150 and b < 150 and a >= 0 and b >= 0):
            T.add_edge(b, a)

    G = T 
    pos = graphviz_layout(G, prog='dot', args='-Grankdir="LR"')
    nx.draw_networkx_nodes(G, pos, node_size=300)
    nx.draw_networkx_edges(G, pos, edgelist=G.edges(), arrows=True, arrowstyle="<-")
    # nx.draw_networkx_labels(G, pos, font_size=7, font_color="white")
    plt.show()

if __name__ == "__main__":
    parse = argparse.ArgumentParser()
    parse.add_argument("--blockchain_tree", type=str, default=None)
    args = parse.parse_args()
    if args.blockchain_tree:
        make_blockchain_tree(args.blockchain_tree)

