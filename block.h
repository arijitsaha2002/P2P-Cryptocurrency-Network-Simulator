#pragma once
#ifndef BLOCK_H
#define BLOCK_H

#include <cstdint>
#include "utils.h"
#define MAX_BLOCK_SIZE 1024 // in KB
#define COINBASE_TRANSACTION_SIZE 1 // in KB
#define TRANSACTION_SIZE 1 // in KB
#define COINBASE_AMOUNT 50

typedef int tid_t;
typedef int blk_t;
extern int MAX_USERS;

class Transaction {
	protected:
		int sender; // ID_x
		int receiver; // ID_y
		int amount; // C coins
		int tid; // Txn ID
		Transaction(int sender, int receiver, int amount);
		static int next_tid;
		int sz;
		long double timestamp;
	public:
		Transaction(int sender);
		Transaction(Node* creator_node);
		// static void reset(){next_tid = 0;}
		int get_size(); // in KB
		int get_sender() {return sender;}
		int get_receiver() {return receiver;}
		int get_amount() {return amount;}
		int get_tid() {return tid;}
		long double *user_recv_time;
		~Transaction();
		static int get_number_of_transactions();
};

class CoinbaseTransaction : public Transaction {
	/*
	 * Special transaction created by miner
	 * sender = -1
	 * receiver = miner
	 * amount = COINBASE_AMOUNT
	 */
	public:
		CoinbaseTransaction(int miner);
};

class Block {
	private:
		CoinbaseTransaction* coinbase;
		int sz; // Size in KB
		int length_of_chain;
		static blk_t next_blk_id;
		int *user_balance;
		long double timestamp;
		// add time for the user who created the block when it is accepted to be sent in the blochain
	public:
		int miner;
		blk_t prev_blk_id; // For the chain
		std::vector<Transaction*> transactions;
		Block* prev_blk;
		Block();
		Block(int miner, Block* prev);
		~Block();
		bool add_transaction(Transaction* txn);
		int get_size();
		int get_length_of_chain();
		long double *users_recv_time;
		blk_t blk_id;
		static int get_number_of_blocks();
};
#endif
