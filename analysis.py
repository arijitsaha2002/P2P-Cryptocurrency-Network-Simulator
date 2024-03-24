#!/bin/python3
import argparse
import os
import numpy
import pandas as pd
import networkx as nx
from networkx.drawing.nx_agraph import graphviz_layout
import matplotlib.pyplot as plt


def make_blockchain_tree(filename, title):
    T = nx.Graph()
    file = pd.read_csv(filename)

    creator_dict = dict({})
    print(filename)
    print(file.columns)
    for a, c_a, b, c_b in zip(file.id, file.cid, file.pid, file.pcid):
        creator_dict[a] = c_a
        creator_dict[b] = c_b
        if a >= 0 and b >= 0:
            T.add_edge(b, a)

    G = T
    plt.figure(figsize=(20,10))
    pos = graphviz_layout(G, prog='dot', args='-Grankdir="LR"')

    # Iterate through nodes
    for node in G.nodes():
        color = 'red' if creator_dict[node] == 0 or creator_dict[node] == 1 else 'blue'  # Set color based on node id
        nx.draw_networkx_nodes(G, pos, nodelist=[node], node_size=300, node_color=color)  # Draw nodes
        nx.draw_networkx_labels(G, pos, labels={node: creator_dict[node]}, font_color='white', font_size=10, font_weight='bold')

    nx.draw_networkx_edges(G, pos, edgelist=G.edges(), arrows=True, arrowstyle="<-")
    plt.title(f"{title}")
    plt.savefig(filename+".png")
    plt.close()
    return

    T = nx.Graph()
    file = pd.read_csv(filename)

    for a, b in zip(file.id, file.pid):
        if( a >= 0 and b >= 0):
            T.add_edge(b, a)

    G = T 
    plt.figure(figsize=(20,10))
    pos = graphviz_layout(G, prog='dot', args='-Grankdir="LR"')
    nx.draw_networkx_nodes(G, pos, node_size=300)
    nx.draw_networkx_edges(G, pos, edgelist=G.edges(), arrows=True, arrowstyle="<-")
    plt.title(f"{title}")
    plt.savefig(filename+".png")
    plt.close()

def get_title_from_filename_as_dict(filename):
    params = os.path.basename(filename)[:-len(".log.csv")].split("_")[2:]
    return {"num_peers": int(filename.split("_")[1][len("info"):]),
                "mean_interarrival_block_time": params[0],
                "mean_interarrival_transaction_time": params[1], 
                "frac_slow": params[2],
                "g1": params[3], 
                "g2": params[4]
    }
    

def get_title_from_filename(filename):
    params = os.path.basename(filename)[:-len(".log.csv")].split("_")[2:]
    ret = f"({params[0]}, {params[1]}, {params[2]}, {params[3]}, {params[4]})"
    return ret;
    


    


def full_analysis(dirname):
    files = os.listdir(dirname)
    block_info_files = list(filter(lambda x : os.path.isfile(os.path.join(dirname, x)) and str(x).startswith("block_info") , files))
    node_info_files =  list(filter(lambda x : os.path.isfile(os.path.join(dirname, x)) and str(x).startswith("node_info") , files))

    block_suffix = list(map(lambda x : x[len("block_info"):], block_info_files))
    node_suffix = list(map(lambda x : x[len("node_info"):], node_info_files))
    common_suffix = list(set(block_suffix).intersection(set(node_suffix)))
    ret = []
    
    for i in common_suffix:
        ret.append(["block_info"+i, "node_info"+i])

    attacker_info_list_x = dict({})
    attacker_info_list_y = dict({})
    list_of_accepted_block_ratio = []
    titles = []

    for r in ret:
        title = get_title_from_filename(r[0])
        # plt.savefig(os.path.join(os.path.dirname(os.path.join(dirname, r[0])), f'tree_{os.path.basename(os.path.join(dirname, r[0]))[len("block_info_"):-len(".log.csv")]}.png'))
        plt.close()
        titles.append(title)
        
        g=get_title_from_filename_as_dict(r[0])
        g1=g["g1"]
        g2=g["g2"]
        
        attacker_longest_single_chain_ratio, accepted_block_ratio = do_ratio_analysis(os.path.join(dirname,r[0]), os.path.join(dirname,r[1]))
        list_of_accepted_block_ratio.append(accepted_block_ratio)

        if g2 in attacker_info_list_x:
            attacker_info_list_x[g2].append(g1)
            attacker_info_list_y[g2].append(attacker_longest_single_chain_ratio)
        else:
            attacker_info_list_x[g2] = [g1]
            attacker_info_list_y[g2] = [attacker_longest_single_chain_ratio]




    X_axis = numpy.arange(len(titles)) 
    plt.figure(figsize=(15,10))
    plt.bar(X_axis, list_of_accepted_block_ratio)
    plt.xticks(X_axis, titles) 
    plt.ylabel("Accepted block ratio") 
    plt.title("Fraction of Accepted Blocks") 
    plt.xlabel("{mean_interarrival_block_time, mean_interarrival_transaction_time, frac_slow}")
    plt.savefig(os.path.join(dirname, f'accepted_block_ratio.png'))
    plt.close()

    X_axis = numpy.arange(len(titles)) 
    

    for g2 in attacker_info_list_x.keys():
        plt.figure(figsize=(15,10))
        plt.plot(attacker_info_list_x[g2], attacker_info_list_y[g2])
        plt.ylabel("attacker 1 aceptence ratio") 
        plt.title("Full blockchain") 
        plt.xlabel("\u03c4 2")
        plt.savefig(os.path.join(dirname, f'{g2}.png'))
        plt.close()


    # make blockchain tree
    for i in common_suffix:
        # for attacker node
        make_blockchain_tree(os.path.join(dirname,"block_recieve_node0_" + i), "attacker_node_blocktree")
        # for honest node
        make_blockchain_tree(os.path.join(dirname, "block_recieve_node2_" + i), "honest_node_blocktree")
        



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
    
    block_graph = [[] for i in range(N)]

    for block, block_par, creator in zip(bc.id, bc.pid, bc.creator):
        if(block_par != -1):
            block_graph[block_par].append((block, creator))

    blocks_in_longest_chain = []
    longest_chain_len = get_info_from_longest_chain(block_graph, 0, blocks_in_longest_chain)
    blocks_in_longest_chain.append((0, -1))
    
    
    num_blocks_per_node_in_longest_chain = [0, 0]
    for i in blocks_in_longest_chain:
        if(i[1] == 1 or i[1] == 0):
            num_blocks_per_node_in_longest_chain[i[1]] += 1;

    

    num_blocks_per_node_overall = [0, 0]
    for creator in bc.creator:
        if(creator == 0 or creator == 1):
            num_blocks_per_node_overall[creator] += 1


    attacker_block_ratio_in_longest_chain = 0 
    if(num_blocks_per_node_in_longest_chain[0] > 0):
        attacker_block_ratio_in_longest_chain = num_blocks_per_node_in_longest_chain[0]/num_blocks_per_node_overall[0]

    ratio_of_blocks_in_longest_chain = longest_chain_len/N
    return attacker_block_ratio_in_longest_chain, ratio_of_blocks_in_longest_chain

if __name__ == "__main__":
    parse = argparse.ArgumentParser()
    parse.add_argument("--blockchain_tree", action = 'store_true')
    parse.add_argument("--node_info", type=str,default=None)
    parse.add_argument("--blockchain_info", type=str,default=None)
    parse.add_argument("--full_analysis", type=str, default=None)
    args = parse.parse_args()
    if args.full_analysis:
        full_analysis(args.full_analysis)
    if args.blockchain_tree:
        make_blockchain_tree(args.blockchain_info)
        plt.show()
