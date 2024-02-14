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


def get_info_from_longest_chain(block_graph, block, blocks_in_longest_chain):
    #print(len(block_graph[block]))
    if(len(block_graph[block]) == 0):
        return 0

    longest_chain_len = 0
    max_len_block = [-1, -1]
    for i in block_graph[block]:
        temp = get_info_from_longest_chain(block_graph, i[0], blocks_in_longest_chain)
       
        if(longest_chain_len < temp):
            longest_chain_len = temp
            max_len_block[0] = i[0]
            max_len_block[1] = i[1]

    blocks_in_longest_chain.append((max_len_block[0], max_len_block[1]))
    return longest_chain_len + 1

def do_ratio_analysis(bc_info,node_info):
    bc = pd.read_csv(bc_info)
    node = pd.read_csv(node_info)
    
    N = len(bc)
    B = len(bc)

    fast_node_list = list(node.loc[node.is_fast == 1, 'id'])
    high_cpu_node_list = list(node.loc[node.is_high_cpu == 1, 'id'])
    
    block_graph = [[] for i in range(N)]

    for block, block_par, creator in zip(bc.id, bc.pid, bc.creator):
        if(block_par != -1):
            block_graph[block_par].append((block, creator))

    blocks_in_longest_chain = []
    longest_chain_len = get_info_from_longest_chain(block_graph, 0, blocks_in_longest_chain)
    blocks_in_longest_chain.append((0, -1))
    
    
    num_blocks_per_node_in_longest_chain = [0 for i in range(len(node))]
    for i in blocks_in_longest_chain:
        if(i[1] != -1):
            num_blocks_per_node_in_longest_chain[i[1]] += 1;

    
    high_cpu__fast_longest_bc_ratio = []
    high_cpu__slow_longest_bc_ratio = []
    low_cpu__fast_longest_bc_ratio = []
    low_cpu__slow_longest_bc_ratio= []


    high_cpu__fast_overall_bc_ratio = []
    high_cpu__slow_overall_bc_ratio = []
    low_cpu__fast_overall_bc_ratio = []
    low_cpu__slow_overall_bc_ratio= []

    num_blocks_per_node_overall = [0 for i in range(len(node))]
    for block, creator in zip(bc.id, bc.creator):
        num_blocks_per_node_overall[creator] += 1

    for node, is_fast, is_high_cpu in zip(node.id, node.is_fast, node.is_high_cpu):
        if(is_fast and is_high_cpu):
            if(num_blocks_per_node_in_longest_chain[node] > 0):
                high_cpu__fast_longest_bc_ratio.append(num_blocks_per_node_in_longest_chain[node]/B)
            else:
                high_cpu__fast_longest_bc_ratio.append(0)

            if(num_blocks_per_node_overall[node] > 0):
                high_cpu__fast_overall_bc_ratio.append(num_blocks_per_node_overall[node]/B)
            else:
                high_cpu__fast_overall_bc_ratio.append(0)

        elif(is_fast and not is_high_cpu):
            if(num_blocks_per_node_in_longest_chain[node] > 0):
                low_cpu__fast_longest_bc_ratio.append(num_blocks_per_node_in_longest_chain[node]/B)
            else:
                low_cpu__fast_longest_bc_ratio.append(0)
            if(num_blocks_per_node_overall[node] > 0):
                low_cpu__fast_overall_bc_ratio.append(num_blocks_per_node_overall[node]/B)
            else:
                low_cpu__fast_overall_bc_ratio.append(0)

        elif(not is_fast and is_high_cpu):
            if(num_blocks_per_node_in_longest_chain[node] > 0):
                high_cpu__slow_longest_bc_ratio.append(num_blocks_per_node_in_longest_chain[node]/B)
            else:
                high_cpu__slow_longest_bc_ratio.append(0)

            if(num_blocks_per_node_overall[node] > 0):
                high_cpu__slow_overall_bc_ratio.append(num_blocks_per_node_overall[node]/B)
            else:
                high_cpu__slow_overall_bc_ratio.append(0)

        elif(not is_fast and not is_high_cpu):
            if(num_blocks_per_node_in_longest_chain[node] > 0):
                low_cpu__slow_longest_bc_ratio.append(num_blocks_per_node_in_longest_chain[node]/B)
            else:
                low_cpu__slow_longest_bc_ratio.append(0)
            if(num_blocks_per_node_overall[node] > 0):
                low_cpu__slow_overall_bc_ratio.append(num_blocks_per_node_overall[node]/B) 
            else:
                low_cpu__slow_overall_bc_ratio.append(0)
            
    return [
            sum(high_cpu__fast_longest_bc_ratio)/len(high_cpu__fast_longest_bc_ratio) if(len(high_cpu__fast_longest_bc_ratio) != 0) else 0,
                sum(high_cpu__slow_longest_bc_ratio)/len(high_cpu__slow_longest_bc_ratio) if(len(high_cpu__slow_longest_bc_ratio) != 0)else 0,
                sum(low_cpu__fast_longest_bc_ratio)/len(low_cpu__fast_longest_bc_ratio) if(len(low_cpu__fast_longest_bc_ratio) != 0)else 0,
                sum(low_cpu__slow_longest_bc_ratio)/len(low_cpu__slow_longest_bc_ratio) if(len(low_cpu__slow_longest_bc_ratio) != 0)else 0
            ],[
                 sum(high_cpu__fast_overall_bc_ratio)/len(high_cpu__fast_overall_bc_ratio) if(len(high_cpu__fast_overall_bc_ratio) != 0)else 0,
                 sum(high_cpu__slow_overall_bc_ratio)/len(high_cpu__slow_overall_bc_ratio) if(len(high_cpu__slow_overall_bc_ratio) != 0)else 0,
                 sum(low_cpu__fast_overall_bc_ratio)/len(low_cpu__fast_overall_bc_ratio) if(len(low_cpu__fast_overall_bc_ratio) != 0)else 0,
                 sum(low_cpu__slow_overall_bc_ratio)/len(low_cpu__slow_overall_bc_ratio) if(len(low_cpu__slow_overall_bc_ratio) != 0)else 0
            ]


if __name__ == "__main__":
    parse = argparse.ArgumentParser()
    parse.add_argument("--blockchain_tree",action="store_true")
    parse.add_argument("--ratio_analysis",action="store_true")
    parse.add_argument("--node_info", type=str,default=None)
    parse.add_argument("--blockchain_info", type=str,default=None)
    args = parse.parse_args()
    if args.blockchain_tree:
        make_blockchain_tree(args.blockchain_info)

    if args.ratio_analysis:
        do_ratio_analysis(args.blockchain_info,args.node_info)
