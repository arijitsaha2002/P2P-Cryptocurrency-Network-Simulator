#include "events.h"


extern vector<Node*> LIST_OF_NODES;
extern RandomNumber rng;
extern long double MEAN_TRANSACTION_INTER_ARRIVAL_TIME;

Event::Event(long double time, event_type type)
{
	this->timestamp = time;
	this->type = type;
}

bool Event::operator<(const Event& rhs) const
{
	return this->timestamp < rhs.timestamp;
}

bool Event::operator>(const Event& rhs) const
{
	return this->timestamp > rhs.timestamp;
}

GenerateTransaction::GenerateTransaction(long double time, Node* creator_node): Event(time, EVENT_TRANSACTION_GENERATE)
{
	this->creator_node = creator_node;
}

void GenerateTransaction::create_events_for_recvrs(Transaction* new_transaction)
{
	vector<Node*> recvrs = creator_node->get_neighbours();

	for(unsigned int i=0; i < recvrs.size(); i++) {
		long double time = this->timestamp + rng.get_latency_between_nodes(creator_node, recvrs[i], new_transaction->get_size());
		Event* new_event = new TransactionRecieved(time, new_transaction, recvrs[i],creator_node);
		add_event_to_queue(new_event);
	}
	return;
}

void GenerateTransaction::create_event_for_next_transaction()
{
	long double time = this->timestamp + rng.expDistributionNumber(1.0/MEAN_TRANSACTION_INTER_ARRIVAL_TIME);
	Event* new_event = new GenerateTransaction(time, creator_node);
	add_event_to_queue(new_event);
	return;
}

void GenerateTransaction::simulate_event()
{
	Transaction *new_transaction = new Transaction(creator_node->get_id());

	add_transaction_to_mempool(new_transaction);
	assert(creator_node -> add_transaction(new_transaction));
	create_events_for_recvrs(new_transaction);
	create_event_for_next_transaction();

}

TransactionRecieved::TransactionRecieved(long double time, Transaction* transaction, Node* reciever_node, Node* sender_node): Event(time, EVENT_TRANSACTION_RECIEVE)
{
	this->transaction = transaction;
	this->reciever_node = reciever_node;
	this->sender_node = sender_node;
}

void TransactionRecieved::simulate_event()
{
	/* Check if transaction is not already recieved */
	if(!(reciever_node -> add_transaction(transaction))) {
		return;
	}


	/* Send transaction to peers */
	vector<Node*> nodes = reciever_node->get_neighbours();

	for(unsigned int i=0; i < nodes.size();i++) {
		if(nodes[i] != sender_node) {
			long double time = this->timestamp + rng.get_latency_between_nodes(reciever_node, nodes[i], transaction->get_size());
			Event* new_event = new TransactionRecieved(time, transaction, nodes[i], reciever_node);
			add_event_to_queue(new_event);
		}
	}
}

GenerateBlock::GenerateBlock(long double time, Node* creator_node,Block* parent_block): Event(time, EVENT_BLOCK_GENERATE)
{
	this->creator_node = creator_node;
	this->parent_block = parent_block;
}

void GenerateBlock::create_events_for_recvrs(Block* new_block)
{
	vector<Node*> recvrs = creator_node->get_neighbours();

	for(unsigned int i =0 ; i < recvrs.size(); i++) {
		long double time = this->timestamp + rng.get_latency_between_nodes(creator_node, recvrs[i], new_block->get_size());
		Event* new_event = new BlockRecieved(time, new_block, recvrs[i], creator_node);
		add_event_to_queue(new_event);
	}
}

void create_event_for_next_block(Node* node,long double timestamp)
{
	long double time = timestamp + rng.get_next_block_time(node);
	Event* new_event = new GenerateBlock(time, node, node->get_longest_chain_tail());
	add_event_to_queue(new_event);
}

void GenerateBlock::simulate_event()
{
	if(parent_block != creator_node->get_longest_chain_tail()) {
		return;
	}
	Block* new_block = new Block(creator_node->get_id(), parent_block);
	creator_node -> populate_block(new_block);

	create_events_for_recvrs(new_block);
	create_event_for_next_block(creator_node, this->timestamp);
}

BlockRecieved::BlockRecieved(long double time, Block* block, Node* reciever_node, Node* sender_node): Event(time, EVENT_BLOCK_RECIEVE)
{
	this->block = block;
	this->reciever_node = reciever_node;
	this -> sender_node = sender_node;
}

void BlockRecieved::simulate_event()
{
	/* Check if block is not already recieved */
	if(!(reciever_node -> add_block_to_tree(block))) {
		return;
	}

	/* Send block to peers */
	vector<Node*> nodes = reciever_node->get_neighbours();

	for(unsigned int i=0; i < nodes.size();i++) {
		if(nodes[i] != sender_node) {
			long double time = this->timestamp + rng.get_latency_between_nodes(reciever_node, nodes[i], block->get_size());
			Event* new_event = new BlockRecieved(time, block, nodes[i], reciever_node);
			add_event_to_queue(new_event);
		}
	}

	/* Check if this block leads to new longest chain */
	if(reciever_node -> get_longest_chain_tail() != block)
		return ;

	/* 
	 * Longest chain changed
	 * Remove generate block event from queue
	 */

	create_event_for_next_block(reciever_node,this->timestamp);
}

