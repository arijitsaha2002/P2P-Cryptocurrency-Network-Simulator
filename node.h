#ifndef NODE_H
#define NODE_H

#include "utils.h"
#include "block.h"

using namespace std;

class Node {
    int node_id;
    vector<Node*> connected_peers;
    set<Transaction*> trasaction_pool;
    Block* longest_chain_tail;
	unordered_map<int, vector<Block*>> ukn_blocks;
	void add_pending_child_blocks(Block* blk);
	
public:
    Node(int node_id);
    bool add_transaction(Transaction* txn); //
    bool add_block_to_tree(Block* blk);
	vector<Node*> get_neighbours();
	void add_node(Node* neighbour);
	bool populate_block(Block* blk); //
	Block* get_longest_chain_tail();
	int get_id();
};

#endif
