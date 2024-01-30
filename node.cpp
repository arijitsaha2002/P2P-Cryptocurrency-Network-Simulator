#include "node.h"
extern long double CURRENT_TIME;

Node::Node(){
	this->node_id = node_id;
}

bool Node::add_block_to_tree(Block* blk){
	Block* prev_blk = blk->prev_blk;

	if(prev_blk->users_recv_time[node_id] == -1){
		if(ukn_blocks.find(prev_blk->blk_id) == ukn_blocks.end()){
			ukn_blocks[prev_blk->blk_id] = vector<Block*>({blk});
		}
		else{
			ukn_blocks[prev_blk->blk_id].append(blk);
		}

		int curr_id = blk->blk_id;
		while(ukn_blocks.find(curr_id) != ukn_blocks.end()){
			Block* blk_child = ukn_blocks.find(cur_id)->second;
			add_block_to_tree(blk_child);
		}
	
		if(blk->get_length_of_chain() > longest_chain_tail->get_length_of_chain()){
			longest_chain_tail = blk;
		}
		return true;
	}

	else return false;
}

void Node::add_transaction(Transaction* txn){
	// pending
}

vector<Node*> Node::get_neighbours(){
	return connected_peers;
}

Block* get_longest_chain_tail(){
	return longest_chain_tail;
}

void Node::add_node(Node* neighbour){
	connected_peers.push_back(neighbour);
}

bool populate_block(Block* blk){
	// pending implmentation
}
