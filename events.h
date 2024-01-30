#ifndef EVENTS_H
#define EVENTS_H
#include "block.h"
#include "transaction.h"

enum event_type
{
	EVENT_TRANSACTION_GENERATE,
	EVENT_TRANSACTION_RECIEVE,
	EVENT_BLOCK_GENERATE,
	EVENT_BLOCK_RECIEVE
};

class Event
{
	long double timestamp;
	event_type type;

	public:

	Event(long double time, int type);
	bool operator<(const Event& rhs) const;
	virtual void simulate_event() = 0;
};

class GenerateTransaction: public Event
{
	Node* creator_node;
	void create_events_for_recvrs(Transaction* new_transaction);
	void create_event_for_next_transaction();
	public:

	GenerateTransaction(long double time, Node* creator_node);
};

class TransactionRecieved: public Event
{
	Transaction* transaction;
	Node* reciever_node;
	Node* sender_node;

	public:

	TransactionRecieved(long double time, Transaction* transaction, Node* reciever_node,Node* sender_node);
};

class GenerateBlock: public Event
{
	Node* creator_node;
	Block* parent_block;

	void create_events_for_recvrs(Block* new_block);

	public:

	GenerateBlock(long double time, Node* creator_node, Node* parent_block);
};

void create_event_for_next_block(Node* node);
class BlockRecieved: public Event
{
	Block* block;
	Node* reciever_node;
	Node* sender_node;

	public:

	BlockRecieved(long double time, Block* block, Node* reciever_node, Node* sender_node);
};

#endif
