#include "node.h"
extern long double CURRENT_TIME;

// NOT DEFINED
//-----------------------
extern long double Z1;
//-----------------------

int Node::get_id(){
	return this->node_id;
}

Node::Node(int node_id){
	this->node_id = node_id;
	this->capabilities = 0;
}

/**
 * @brief Constructor for Node
 *
 * @param node_id The id of the node
 * @param capabilities The capabilities of the node
 * @param genesis_block The genesis block of the blockchain
 */
Node::Node(int node_id, int capabilities, Block* genesis_block, bool selfish, float hashing_power){

	this->node_id = node_id;
	this->capabilities = capabilities;
	this->hashing_power = hashing_power;
	this->longest_chain_tail = genesis_block;

	if(selfish) {
		this->selfish = true;
		this->private_chain = new queue<Block*>();
	}
	else{
		this->selfish = false;
		this->private_chain = NULL;
	}
}

/**
 * @brief Function to update the longest chain tail
 */
void Node::update_longest_chain_tail(Block* blk){
	if(this->longest_chain_tail->get_length_of_chain() < blk->get_length_of_chain()){
		longest_chain_tail = blk;
	}
}

/**
 * @brief Function to update the longest chain tail recursively for all the pending child blocks
 */
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


/**
 * @brief Function to add a block to the this block tree
 *
 * @param txn The block  to be added
 *
 * @return 1 if the block is added successfully, 0 if the transaction is already present
 */
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

/** update transaction in this node's transaction pool */
bool Node::add_transaction(Transaction* txn){
	 if(txn->user_recv_time[node_id] != -1){
		return false;
	 }
	 txn->user_recv_time[node_id] = CURRENT_TIME;
	 return true;
 }


vector<Node*> Node::get_neighbours(){
	return connected_peers;
}

Block* Node::get_longest_chain_tail(){
	return this->longest_chain_tail;
}

long double Node::get_hashing_power(){
	return this->hashing_power;
}

void Node::add_node(Node* neighbour){
	connected_peers.push_back(neighbour);
}

/** @brief function to construct a new block at this node
 * @param blk The block to be constructed
 * @param start_time The time at which the block construction starts
 */
void Node::populate_block(Block* blk, long double start_time){

	unordered_map<int, Transaction*> mem_pool_copy = get_mem_pool();
	Block* end_blk = blk->prev_blk;
	
	/* sanity check */
	if(this->selfish and this->private_chain->size()){
		assert(this->private_chain->back() == end_blk);
	}
	else{
		assert(this->longest_chain_tail == end_blk);
	}

	while(end_blk->blk_id != 0){
		for(auto txn: end_blk->transactions){
			if(mem_pool_copy.find(txn->get_tid()) != mem_pool_copy.end()){
				mem_pool_copy.erase(txn->get_tid());
			}

		}

		end_blk = end_blk->prev_blk;
	}

	for(auto txn : mem_pool_copy){
		if(txn.second->user_recv_time[node_id] == -1 || start_time < txn.second->user_recv_time[node_id]) 
			continue;
		if(blk->add_transaction(txn.second) == 2){
			break;
		}
	}

}

int Node::get_capability(){
	return this->capabilities;
}

bool Node::is_selfish(){
	return selfish;
}

void Node::set_longest_chain_tail(Block* new_longest_chain_tail){
	this->longest_chain_tail = new_longest_chain_tail;
}

