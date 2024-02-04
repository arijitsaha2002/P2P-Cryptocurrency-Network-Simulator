#ifndef NODE_H
#define NODE_H

#include "utils.h"
#include "block.h"

#define NODE_FAST	0x1
#define NODE_SLOW	0x2
#define NODE_LOW_CPU	0x3
#define NODE_FAST_CPU	0x4

using namespace std;

class Node {
    int node_id;
	int capabilities;
	long double hashing_power;
    vector<Node*> connected_peers;
    set<Transaction*> trasaction_pool;
    Block* longest_chain_tail;
	unordered_map<int, vector<Block*>> ukn_blocks;
	void update_longest_chain_tail(Block* blk);
	void update_longest_chain_tail_recursively(Block* blk);
	
public:
    Node(int node_id);
	Node(int node_id, int capabilities);
    bool add_transaction(Transaction* txn); //
    bool add_block_to_tree(Block* blk);
	vector<Node*> get_neighbours();
	void add_node(Node* neighbour);
	bool populate_block(Block* blk); //
	Block* get_longest_chain_tail();
	int get_id();
};

#endif
