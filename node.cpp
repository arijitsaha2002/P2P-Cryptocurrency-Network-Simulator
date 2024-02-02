#include "node.h"
extern long double CURRENT_TIME;

Node::Node(int node_id){
	this->node_id = node_id;
}

bool Node::add_block_to_tree(Block* blk){
	if(blk -> users_recv_time[node_id] != -1){
		return false;
	}

	Block* prev_blk = blk->prev_blk;

	blk->users_recv_time[node_id] = CURRENT_TIME;
	if(prev_blk->users_recv_time[node_id] == -1){
		// Parent block not yet recieved
		if(ukn_blocks.find(prev_blk->blk_id) == ukn_blocks.end()){
			ukn_blocks[prev_blk->blk_id] = vector<Block*>({blk});
		}
		else{
			ukn_blocks[prev_blk->blk_id].push_back(blk);
		}

		// int curr_id = blk->blk_id;
		// while(ukn_blocks.find(curr_id) != ukn_blocks.end()){
		// 	Block* blk_child = ukn_blocks.find(curr_id)->second;
		// 	add_block_to_tree(blk_child);
		// }
	
		// if(blk->get_length_of_chain() > longest_chain_tail->get_length_of_chain()){
		// 	longest_chain_tail = blk;
		// }
	}else{
		add_pending_child_blocks(blk);
	}

	return true;
}

void Node::add_pending_child_blocks(Block* blk){
	int blk_id = blk->blk_id;
	if(ukn_blocks.find(blk_id) != ukn_blocks.end()){
		vector<Block*> child_blks = ukn_blocks[blk_id];
		for(unsigned int i=0; i<child_blks.size(); i++){
			add_pending_child_blocks(child_blks[i]);
		}
		ukn_blocks.erase(blk_id);
	}
}

// bool Node::add_transaction(Transaction* txn){
// 	// pending
// 	return false;
// }

vector<Node*> Node::get_neighbours(){
	return connected_peers;
}

Block* Node::get_longest_chain_tail(){
	return this->longest_chain_tail;
}

void Node::add_node(Node* neighbour){
	connected_peers.push_back(neighbour);
}

// bool Block::populate_block(Block* blk){
// 	// pending implmentation
// }
