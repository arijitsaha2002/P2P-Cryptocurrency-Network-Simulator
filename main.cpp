#include "events.h"
#include "node.h"
#include <algorithm>
#include <sstream>
#include <string>
#include "parser.h"


struct EventCMP {
	 bool operator()(Event * event1, Event * event2) const {
		 return (*event1) < (*event2);
	 }
 };

#define EVENT_SET set<Event *, EventCMP>

/**
 * @brief The random number api to genarate all sorts of required random values
 */
RandomNumber rng;

/**
 * @brief The list of all the nodes in the network
 */
vector<Node *> LIST_OF_NODES;

/**
 * @brief The mempool which contains all the transactions which are not yet included in the block
 */
unordered_map<int, Transaction *> mempool;

/**
 * @brief The rho(link speed) value for each pair of nodes
 */
map<pair<int, int>, double> rho;

/**
 * @brief The set of all the events in the simulation
 */
EVENT_SET LIST_OF_EVENTS;

/**
 * @brief The list of all the blocks in the network
 */
vector<Block*> LIST_OF_BLOCKS;

/**
 * @brief Simulation Parameters
 */
long double CURRENT_TIME = 0, MEAN_TRANSACTION_INTER_ARRIVAL_TIME = 0.75;
int MAX_USERS;
float Z0, Z1;
int MAX_BLOCKS;
int INITIAL_AMOUNT;
long double AVG_INTERARRIVAL_BLOCK_TIME = 10;
int MAX_TRANSACTIONS;


pair<int, int> getSortedPair(int a, int b) {
    return make_pair(min(a, b), max(a, b));
}

/**
 * @brief The function to add a transaction to the mempool
 * @param new_transaction The transaction to be added to the mempool
 */
void add_transaction_to_mempool(Transaction* new_transaction){
	mempool[new_transaction->get_tid()] = new_transaction;
}

int TransactionAmount(){
    int low = 0;
    int high = 50;
    return rng.uniformNumber(low, high);
}

unordered_map<int, Transaction*> get_mem_pool(){
	return mempool;
}

/**
 * function to get latency between two nodes in the network
 */
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

/**
 * function to get the next block time which is exponentially distributed
 */
long double RandomNumber::get_next_block_time(Node *n){
	long double T_k = rng.expDistributionNumber(n->get_hashing_power()/AVG_INTERARRIVAL_BLOCK_TIME);
	return T_k;
}

/** function to add an event to the event queue */
void add_event_to_queue(Event * e) {
    LIST_OF_EVENTS.insert(e);
}


void init(string file_name){
    ifstream input(file_name);
    if(!input.is_open()){
        cout << "File is not readable" << endl;
        exit(1);
    }
    
	// Create the genesis block
    Block * GENESIS_BLOCK = new Block();
   
    int numberOfPeers, numberOfEdges;
    input >> numberOfPeers >> numberOfEdges;
	
	MAX_USERS = numberOfPeers;

	int numberOfHonestPeers = numberOfPeers - 2;
	// Initializing the capabilities of all the nodes in the network
	vector<int> Z0_distribution(Z0*numberOfHonestPeers, NODE_SLOW);
	vector<int> Z1_distribution(Z1*numberOfPeers, NODE_LOW_CPU);

	while((int) Z0_distribution.size() != numberOfPeers) Z0_distribution.push_back(NODE_FAST);
	
	Z0_distribution[numberOfPeers - 1] = NODE_SELFISH;
	Z0_distribution[numberOfPeers - 2] = NODE_SELFISH;

	while((int) Z1_distribution.size() != numberOfPeers) Z1_distribution.push_back(NODE_FAST_CPU);
 
	shuffle(Z0_distribution.begin(), Z0_distribution.end(), rng.gen);
	shuffle(Z1_distribution.begin(), Z1_distribution.end(), rng.gen);

	// create the nodes in the network
    for(int i = 0; i < numberOfPeers; i ++){
		if(Z0_distribution[i] != NODE_SELFISH){
	        LIST_OF_NODES.push_back(new Node(i, Z0_distribution[i] | Z1_distribution[i], GENESIS_BLOCK));
		}
		else{
	        LIST_OF_NODES.push_back(new Node(i, NODE_FAST | Z1_distribution[i], GENESIS_BLOCK, true));
		}
    }
    
    for(int i = 0; i < numberOfEdges; i ++){
        int n1, n2;
        input >> n1 >> n2;
        LIST_OF_NODES[n1]->add_node(LIST_OF_NODES[n2]);
        LIST_OF_NODES[n2]->add_node(LIST_OF_NODES[n1]);
    }

    input.close();
}

/**
 * @brief The function to create the initial events in the simulation
 */
void create_initial_events(){

	for(int i=0; i<MAX_USERS; i++){
		Event* to_add = new GenerateTransaction(rng.expDistributionNumber(1.0/MEAN_TRANSACTION_INTER_ARRIVAL_TIME), LIST_OF_NODES[i]);
		LIST_OF_EVENTS.insert(to_add);
	}
	for(int i = 0; i<MAX_USERS; i++){
		LIST_OF_EVENTS.insert(new GenerateBlock(rng.get_next_block_time(LIST_OF_NODES[i]), LIST_OF_NODES[i], LIST_OF_NODES[i]->get_longest_chain_tail()));
	}

}

/**
 * @brief The function to run the simulation loop
 */
void run_loop(){
    CURRENT_TIME = 0;
    while(!LIST_OF_EVENTS.empty()) {
		int x = Block::get_number_of_blocks();

		/** simulation end condition */
		if(x > MAX_BLOCKS) break;
		if(MAX_TRANSACTIONS != -1 and Transaction::get_number_of_transactions() >  MAX_TRANSACTIONS) break;

		/** picking the event based on the timestamp */
        auto top = LIST_OF_EVENTS.begin();
		assert(CURRENT_TIME <= (*top)->timestamp);
        CURRENT_TIME = (*top)->timestamp;
        (*top)->simulate_event();
        LIST_OF_EVENTS.erase(top);
		delete *top;
    }
}

/**
 * @brief The function to log the data of the simulation
 */
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

/**
 * @brief The main function to run the simulation
 */
int main(int argc, char * argv[]){

	struct arguments args = { 0.1, 10, 100, 50, 0.4, 0.4, 44, -1, (char *)"graph"};
    struct argp argp = {options, parse_opt, 0, 0, 0, 0, 0};

	string file_name;
	string suffix;
    if (argp_parse(&argp, argc, argv, 0, 0, &args) == 0) {
		file_name = args.graph;
		INITIAL_AMOUNT = args.initial_amt;
		MAX_BLOCKS = args.max_blocks;
		AVG_INTERARRIVAL_BLOCK_TIME = args.interarrival_block_time;
		Z1 = args.frac_low_cpu;
		rng.set_seed(args.seed);
		MEAN_TRANSACTION_INTER_ARRIVAL_TIME = args.interarrival_transaction_time;
		MAX_TRANSACTIONS = args.max_transactions;

    } else {
        std::cerr << "Failed to parse arguments" << std::endl;
        return 1;
    }

	init(file_name);
	stringstream ss;
	ss << MAX_USERS << "_" << args.interarrival_block_time << "_" << args.frac_low_cpu 
		<< "_" << args.interarrival_transaction_time << "_" << args.frac_slow << ".log.csv";
	suffix = ss.str();
	CURRENT_TIME = 0;
	create_initial_events();
	run_loop();
	log_data(suffix);
	return 0;
}


