#include "block.h"

extern int INITIAL_AMOUNT;
extern RandomNumber rng;
/*
 * Transaction size
 */
int Transaction::get_size() {
	return sz;
}

int Transaction::next_tid = 0;
blk_t Block::next_blk_id = 0;
// Transaction::Transaction(int sender, int receiver, int amount) {
// 	this->sender = sender;
// 	this->receiver = receiver;
// 	this->amount = amount;
// 	this->tid = next_tid++;
// 	this -> sz = TRANSACTION_SIZE;
// }
 
/*
 * Uniformly randomly generates a reciever
 * amount of transaction generated according to distribution and balance of sender
 */
Transaction::Transaction(int sender) {
	this->sender = sender;
	this->tid = next_tid++;
	
	int receiver = sender;
	while(receiver == sender)	receiver = rng.uniformNumber(0, MAX_USERS-1);
	this -> receiver = receiver;

	this -> amount = TransactionAmount(sender);

	this -> sz = TRANSACTION_SIZE;
	this -> timestamp = CURRENT_TIME;

	this -> user_recv_time = new long double[MAX_USERS];

	for(int i=0; i<MAX_USERS; i++){
		this->user_recv_time[i] = -1;
	}
}

Transaction::Transaction(int sender, int receiver, int amount){
	
}

Transaction::~Transaction() {
	delete user_recv_time;
}

/*
 * Special transaction created by miner
 * sender = -1
 * receiver = miner
 * amount = 50
 */
CoinbaseTransaction::CoinbaseTransaction(int miner) : Transaction(-1, miner, COINBASE_AMOUNT) {
	sz = COINBASE_TRANSACTION_SIZE;
}


/*
 * Creates a block with coinbase transaction
 */

Block::Block(int miner, Block* prev) {
	this->miner = miner;
	this->prev_blk_id = prev -> blk_id;
	this->blk_id = next_blk_id++;
	this->prev_blk = prev;
	this->sz = COINBASE_TRANSACTION_SIZE;
	this->coinbase = new CoinbaseTransaction(miner);
	this->user_balance = new int[MAX_USERS];
	this->length_of_chain = prev->length_of_chain + 1;
	this -> timestamp = CURRENT_TIME;
	for(int i=0;i<MAX_USERS;i++) {
		user_balance[i] = prev->user_balance[i];
	}
	user_balance[miner] += COINBASE_AMOUNT;

	this -> users_recv_time = new long double[MAX_USERS];
	for(int i=0; i<MAX_USERS; i++){
		users_recv_time[i] = -1;
	}
}

Block::Block(){
	this->miner = -1;
	this->prev_blk = nullptr;
	this->prev_blk_id = -1;
	this->blk_id = next_blk_id++;
	this->sz = COINBASE_TRANSACTION_SIZE;
	this->coinbase = nullptr;
	this->user_balance = new int[MAX_USERS];
	this->length_of_chain = 1;
	this->timestamp = CURRENT_TIME;
	for(int i=0; i<MAX_USERS; i++){
		user_balance[i] = INITIAL_AMOUNT;
	}

	this->users_recv_time = new long double[MAX_USERS];
	
	for(int i=0; i<MAX_USERS; i++){
		users_recv_time[i] = CURRENT_TIME;
	}

}
/*
 * Destructor
 */
Block::~Block() {
	delete coinbase;
	delete user_balance;
	delete users_recv_time;
}

/*
 * Adds a transaction to the block
 */
bool Block::add_transaction(Transaction* txn) {
	if(txn->get_amount() > user_balance[txn->get_sender()]) {
		return false;
	}
	if ( sz + txn->get_size() > MAX_BLOCK_SIZE ) {
		return false;
	}
	transactions.push_back(txn);
	sz += txn->get_size();
	user_balance[txn->get_sender()] -= txn->get_amount();
	user_balance[txn->get_receiver()] += txn->get_amount();
	return true;
}

/*
 * Block size is sum of transaction sizes + 1 KB for coinbase
 */
int Block::get_size() {
	return sz;
};


int Block::get_length_of_chain(){
	return length_of_chain;
}
