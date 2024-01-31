#ifndef EVENTS_H
#define EVENTS_H
#include "block.h"
#include "transaction.h"

/**
 * @file events.h
 * @brief Contains the class definitions for various events
 *
 * Contains the class definitions
 */

/**
 * Enum for event types
 * EVENT_TRANSACTION_GENERATE: Event leading to a new transaction being generated
 * EVENT_TRANSACTION_RECIEVE: Event leading to a transaction being recieved by a node
 * EVENT_BLOCK_GENERATE: Event leading to a new block being generated
 * EVENT_BLOCK_RECIEVE: Event leading to a block being recieved by a node
 */
enum event_type
{
	EVENT_TRANSACTION_GENERATE,
	EVENT_TRANSACTION_RECIEVE,
	EVENT_BLOCK_GENERATE,
	EVENT_BLOCK_RECIEVE
};

/**
 * @class Event
 *
 * Class for events
 */
class Event
{
	long double timestamp;
	event_type type;

	public:

	/**
	 * @brief Constructor for Event
	 *
	 * @param time The time at which the event occurs
	 * @param type The type of the event
	 */
	Event(long double time, int type);
	bool operator<(const Event& rhs) const;
	/**
	 * @brief Function to simulate the event
	 *
	 * Pure virtual function to simulate the event
	 */
	virtual void simulate_event() = 0;
};

/**
 * @class GenerateTransaction
 *
 * Class for event leading to a new transaction being generated
 */
class GenerateTransaction: public Event
{
	Node* creator_node;
	void create_events_for_recvrs(Transaction* new_transaction);
	void create_event_for_next_transaction();
	public:

	/**
	 * @brief Constructor for GenerateTransaction
	 *
	 * @param time The time at which the event occurs
	 * @param creator_node The node which creates the transaction
	 */
	GenerateTransaction(long double time, Node* creator_node);
};

/**
 * @class TransactionRecieved
 *
 * Class for event leading to a transaction being recieved by a node
 */
class TransactionRecieved: public Event
{
	Transaction* transaction;
	Node* reciever_node;
	Node* sender_node;

	public:

	/**
	 * @brief Constructor for TransactionRecieved
	 *
	 * @param time The time at which the event occurs
	 * @param transaction The transaction being recieved
	 * @param reciever_node The node which recieves the transaction
	 * @param sender_node The node which sends the transaction
	 */
	TransactionRecieved(long double time, Transaction* transaction, Node* reciever_node,Node* sender_node);
};

/**
 * @class GenerateBlock
 *
 * Class for event leading to a new block being generated
 */
class GenerateBlock: public Event
{
	Node* creator_node;
	Block* parent_block;

	void create_events_for_recvrs(Block* new_block);

	public:

	/**
	 * @brief Constructor for GenerateBlock
	 *
	 * @param time The time at which the event occurs
	 * @param creator_node The node which creates the block
	 * @param parent_block The parent block of the block being created
	 */
	GenerateBlock(long double time, Node* creator_node, Node* parent_block);
};

/**
 * @brief Function to create an event for new block generation
 *
 * @param node The node which creates the block
 */
void create_event_for_next_block(Node* node);

/**
 * @class BlockRecieved
 *
 * Class for event leading to a block being recieved by a node
 */
class BlockRecieved: public Event
{
	Block* block;
	Node* reciever_node;
	Node* sender_node;

	public:

	/**
	 * @brief Constructor for BlockRecieved
	 *
	 * @param time The time at which the event occurs
	 * @param block The block being recieved
	 * @param reciever_node The node which recieves the block
	 * @param sender_node The node which sends the block
	 */
	BlockRecieved(long double time, Block* block, Node* reciever_node, Node* sender_node);
};

#endif