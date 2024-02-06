#include "node.h"
extern long double CURRENT_TIME;

// NOT DEFINED
//-----------------------
extern long double Z1;
//-----------------------


Node::Node(int node_id){
	this->node_id = node_id;
	this->capabilities = 0;
}

Node::Node(int node_id, int capabilities, Block* genesis_block){
	this->node_id = node_id;
	this->capabilities = capabilities;

// using [Z1*h + (1-Z1)*(10*h) = 1] for finding the hashing power
	if(capabilities & NODE_LOW_CPU){
		this->hashing_power = 1/(10 - 9*Z1);
	}
	else{
		this->hashing_power = 10/(10- 9*Z1);
	}
	this->longest_chain_tail = genesis_block;
}

void Node::update_longest_chain_tail(Block* blk){
	if(this->longest_chain_tail->get_length_of_chain() < blk->get_length_of_chain()){
		longest_chain_tail = blk;
	}
}

void Node::update_longest_chain_tail_recursively(Block* blk){
	auto itr = ukn_blocks.find(blk->blk_id);
	if(itr == ukn_blocks.end()){
		update_longest_chain_tail(blk);
		return;
	}
	for(auto child: itr->second){
		update_longest_chain_tail_recursively(child);
	}
	// erase updated blocks from ukn_blocks
	ukn_blocks.erase(ukn_blocks.find(blk->blk_id));
}

bool Node::add_block_to_tree(Block* blk){

	if(blk->users_recv_time[node_id] != -1){
		return false;
	}
	blk->users_recv_time[node_id] = CURRENT_TIME;


	// HANDLING PARENT
	//
		//
		// if parent already recieved and parent not in ukn_blocks
	if(blk->prev_blk->users_recv_time[node_id] != -1 && ukn_blocks.find(blk->prev_blk->prev_blk_id) == ukn_blocks.end()){

		// ****just think a little bit about first block(coinbase)****
			update_longest_chain_tail(blk);
	}
	else{
		//
		// if parent not present or parent in ukn_blocks then add current to ukn_blocks
		if(ukn_blocks.find(blk->prev_blk_id) == ukn_blocks.end()){
			ukn_blocks[blk->prev_blk_id] = vector<Block*>({blk});
		}
		else{
			ukn_blocks[blk->prev_blk_id].push_back(blk);
		}

	}

	
	// HANDLING CHILDREN
	//
	//
	// if current blk goes into ukn_blocks ==> do nothing
	//
		// If current blk didn't in ukn_blocks
		// if no waiting children ==> already updated the length while handling children
		//
			//
			// if there are waiting children
	if(ukn_blocks.find(blk->blk_id) != ukn_blocks.end()){
		update_longest_chain_tail_recursively(blk);
	}

	return true;
}

//void Node::add_pending_child_blocks(Block* blk){
//	int blk_id = blk->blk_id;
//	if(ukn_blocks.find(blk_id) != ukn_blocks.end()){
//		vector<Block*> child_blks = ukn_blocks[blk_id];
//		for(unsigned int i=0; i<child_blks.size(); i++){
//			add_pending_child_blocks(child_blks[i]);
//		}
//		ukn_blocks.erase(blk_id);
//	}
//}

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

int Node::get_capability(){
	return this->capabilities;
}
