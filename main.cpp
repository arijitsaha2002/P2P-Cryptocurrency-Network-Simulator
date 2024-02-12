#include "events.h"
#include <algorithm>
#include <string>

struct EventCMP {
	 bool operator()(Event * event1, Event * event2) const {
		 return (*event1) < (*event2);
	 }
 };

#define EVENT_SET set<Event *, EventCMP>
RandomNumber rng;
vector<Node *> LIST_OF_NODES;
unordered_map<int, Transaction *> mempool;
map<pair<int, int>, double> rho;
EVENT_SET LIST_OF_EVENTS;
vector<Block*> LIST_OF_BLOCKS;
long double CURRENT_TIME = 0, MEAN_TRANSACTION_INTER_ARRIVAL_TIME = 0.75;
int MAX_USERS;
float Z0 = 0.4, Z1 = 0.3;
int MAX_BLOCKS = 250;
int INITIAL_AMOUNT = 50;
long double AVG_INTERARRIVAL_BLOCK_TIME = 10;
int MAX_TRANSACTIONS = -1;

pair<int, int> getSortedPair(int a, int b) {
    return make_pair(min(a, b), max(a, b));
}

void add_transaction_to_mempool(Transaction* new_transaction){
	mempool[new_transaction->get_tid()] = new_transaction;
}

int TransactionAmount(uid_t sender){
    int low = 0;
    int high = 50;
    return rng.uniformNumber(low, high);
}

unordered_map<int, Transaction*> get_mem_pool(){
	return mempool;
}

long double RandomNumber::get_latency_between_nodes(Node* n1,Node* n2, int size /* in KB */){
	long double pij, cij, dij;
	auto p = getSortedPair(n1->get_id(), n2->get_id());
	if(rho.find(p) == rho.end()) rho[p] = uniformNumber(10,500);
	size = size * 8;
	pij = rho[p] * 1e-3;
	cij = (n1->get_capability() & n2->get_capability() & NODE_FAST) ?1000*100 : 1000*5;
	dij = expDistributionNumber(cij/ 96);
	return pij + size / cij + dij;
}

long double RandomNumber::get_next_block_time(Node *n){
	long double T_k = rng.expDistributionNumber(n->get_hashing_power()/AVG_INTERARRIVAL_BLOCK_TIME);
	return T_k;
}

void add_event_to_queue(Event * e) {
    LIST_OF_EVENTS.insert(e);
}


void init(string file_name){
    ifstream input(file_name);
    if(!input.is_open()){
        cout << "File is not readable" << endl;
        exit(1);
    }
    
    Block * GENESIS_BLOCK = new Block();
   
    int numberOfPeers, numberOfEdges;
    input >> numberOfPeers >> numberOfEdges;
	
	MAX_USERS = numberOfPeers;

	vector<int> Z0_distribution(Z0*numberOfPeers, NODE_SLOW);
	vector<int> Z1_distribution(Z1*numberOfPeers, NODE_LOW_CPU);

	while((int) Z0_distribution.size() != numberOfPeers) Z0_distribution.push_back(NODE_FAST);
	while((int) Z1_distribution.size() != numberOfPeers) Z1_distribution.push_back(NODE_FAST_CPU);
 
	shuffle(Z0_distribution.begin(), Z0_distribution.end(), rng.gen);
	shuffle(Z1_distribution.begin(), Z1_distribution.end(), rng.gen);

    for(int i = 0; i < numberOfPeers; i ++){
        LIST_OF_NODES.push_back(new Node(i, Z0_distribution[i] | Z1_distribution[i], GENESIS_BLOCK));
    }
    
    for(int i = 0; i < numberOfEdges; i ++){
        int n1, n2;
        input >> n1 >> n2;
        LIST_OF_NODES[n1]->add_node(LIST_OF_NODES[n2]);
        LIST_OF_NODES[n2]->add_node(LIST_OF_NODES[n1]);
    }

    input.close();
}

void create_initial_events(){

	for(int i=0; i<MAX_USERS; i++){
		Event* to_add = new GenerateTransaction(rng.expDistributionNumber(1.0/MEAN_TRANSACTION_INTER_ARRIVAL_TIME), LIST_OF_NODES[i]);
		LIST_OF_EVENTS.insert(to_add);
	}
	for(int i = 0; i<MAX_USERS; i++){
		LIST_OF_EVENTS.insert(new GenerateBlock(rng.get_next_block_time(LIST_OF_NODES[i]), LIST_OF_NODES[i], LIST_OF_NODES[i]->get_longest_chain_tail()));
	}

}

void run_loop(){
    CURRENT_TIME = 0;
    while(!LIST_OF_EVENTS.empty()) {
		int x = Block::get_number_of_blocks();
		if(x > MAX_BLOCKS) break;
		if(MAX_TRANSACTIONS != -1 and Transaction::get_number_of_transactions() >  MAX_TRANSACTIONS) break;
        auto top = LIST_OF_EVENTS.begin();
		assert(CURRENT_TIME <= (*top)->timestamp);
        CURRENT_TIME = (*top)->timestamp;
        (*top)->simulate_event();
        LIST_OF_EVENTS.erase(top);
    }
}

void log_data(string suffix){
	FILE* f = fopen(("block_info"+suffix).c_str(), "w");
	fprintf(f, "id,pid,create_time,creator\n");

	for(auto curr_blk: LIST_OF_BLOCKS){
		fprintf(f, "%d,%d,%Lf,%d\n", curr_blk->blk_id, curr_blk->prev_blk_id, curr_blk->users_recv_time[curr_blk->miner], curr_blk->miner);
	}
	fclose(f);

	f = fopen(("node_info"+suffix).c_str(),"w");
	fprintf(f,"id,is_fast,is_high_cpu\n");
	for(auto curr_node: LIST_OF_NODES)
	{
		fprintf(f,"%d,%d,%d\n",curr_node -> get_id(),(bool)(curr_node->get_capability() & NODE_FAST),(bool)(curr_node -> get_capability() & NODE_FAST_CPU));
	}
	fclose(f);

	for(int i = 0;i< MAX_USERS;i++)
	{
		f = fopen(("block_recieve_node"+to_string(i)+suffix).c_str(),"w");
		fprintf(f,"id,timestamp\n");
		for(auto curr_blk: LIST_OF_BLOCKS)
		{
			if(curr_blk->users_recv_time[i] != -1)
				fprintf(f,"%d,%Lf\n",curr_blk->blk_id,curr_blk->users_recv_time[i]);
		}
	}

}

int main(int argc, char * argv[]){
	if(argc != 10){
		cerr << "Use blockSimWrapper.py\n";
		return 1;
	}
	string file_name(argv[3]);
	INITIAL_AMOUNT = atoi(argv[1]);
	MAX_BLOCKS = atoi(argv[2]);
	AVG_INTERARRIVAL_BLOCK_TIME = atof(argv[4]);
	Z1 = atof(argv[5]);
	rng.set_seed(atoi(argv[6]));
	MEAN_TRANSACTION_INTER_ARRIVAL_TIME = atof(argv[7]);
	MAX_TRANSACTIONS = atoi(argv[8]);
	Z0 = atof(argv[9]);
	string suffix(argv[10]);
	init(file_name);
	CURRENT_TIME = 0;
	create_initial_events();
	run_loop();
	log_data(suffix);
	return 0;
}


