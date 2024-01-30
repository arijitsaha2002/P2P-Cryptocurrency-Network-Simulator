#include "node.h"

void Node::add_node_to_blockchain(Block* blk){
       if(blk->get_length_of_chain() > longestChainTail->get_length_of_chain()){
        longestChainTail = blk;
       }
}

void Node::add_transaction(Transaction* txn){
    trasactionPool.insert(txn);
}