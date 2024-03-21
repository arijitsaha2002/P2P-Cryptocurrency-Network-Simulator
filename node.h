#pragma once
#ifndef NODE_H
#define NODE_H

#include "utils.h"
#include "block.h"

/**
 * @file node.h
 * @brief Contains the class definition for a node
 *
 * Contains the class definition
 */

#define NODE_FAST	0x1
#define NODE_SLOW	0x2
#define NODE_LOW_CPU	0x4
#define NODE_FAST_CPU	0x8
#define NODE_SELFISH	0x10

using namespace std;

/**
 * @class Node
 *
 * Class for a node
 */
class Node {
private:
    int node_id;
	bool selfish;
	int capabilities;
	long double hashing_power;
    vector<Node*> connected_peers;
    set<Transaction*> trasaction_pool;
    Block* longest_chain_tail;
	unordered_map<int, vector<Block*>> ukn_blocks;
	/**
	 * @brief Function to update the longest chain tail
	 *
	 * Function to update the longest chain tail
	 */
	void update_longest_chain_tail(Block* blk);
	void update_longest_chain_tail_recursively(Block* blk);
	
public:
	queue<Block*>* private_chain;
    Node(int node_id);
	/**
	 * @brief Constructor for Node
	 *
	 * @param node_id The id of the node
	 * @param capabilities The capabilities of the node
	 * @param genesis_block The genesis block of the blockchain
	 */
	Node(int node_id, int capabilities, Block* genesis_block, bool selfish, float hashing_power);

	/** update transaction in the transaction pool */
    bool add_transaction(Transaction* txn);

	/**add block to this nodes block tree */
    bool add_block_to_tree(Block* blk);

	vector<Node*> get_neighbours();

	/** update neighbours */
	void add_node(Node* neighbour);

	/** populate the block which is being constructed by this node */
	void populate_block(Block* blk, long double start_time);

	/** get the longest chain tail */
	Block* get_longest_chain_tail();

	/** functions to get some information about this node*/
	int get_id();
	int get_capability();
	long double get_hashing_power();

	/** add blocks whose parent has arrived*/
	void add_pending_child_blocks(Block* blk);

	/** check whether the miner is selfish or not*/
	bool is_selfish();

	void set_longest_chain_tail(Block* new_longest_chain_tail);
};

#endif
