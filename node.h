#ifndef NODE_H
#define NODE_H

#include "block.h"
#include "utils.h"

class Node {
    int node_id;
    vector<Node*> connected_peers;
    set<Transaction*> trasaction_pool;
    Block* longest_chain_tail;
	unordered_map<int, vector<Block*>> ukn_blocks;
	
public:
    Node(int node_id);
    bool add_transaction(Transaction* txn); //
    void add_block_to_tree(Block* blk);
	vector<Node*> get_neighbours();
	void add_node(Node* neighbour);
	void populate_block(Block* blk); //
	Block* get_longest_chain_tail();
};

#endif
