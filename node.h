#include "block.h"
#include "utils.h"

class Node {
    int node_id;
    vector<Node*> connectedPeers;
    set<Transaction*> trasactionPool;
    Block* longestChainTail;

public:
    Node(int node_id){
        this->node_id = node_id;
    }

    void add_transaction(Transaction* txn);
    void add_node_to_blockchain(Block* blk);
};