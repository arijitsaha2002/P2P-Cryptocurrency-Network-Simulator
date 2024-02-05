#include "utils.h"
#include "node.h"
#include "events.h"
#include <algorithm>

vector<Node *> LIST_OF_NODES;
EVENT_SET LIST_OF_EVENTS;
long double CURRENT_TIME, MEAN_TRANSACTION_INTER_ARRIVAL_TIME;
int MAX_USERS;
int Z0, Z1, MAX_BLOCKS, END_TIME, MAX_TRANSACTIONS;
RandomNumber rng;


void init(string file_name){
    ifstream input(file_name);
    if(!input.is_open()){
        cout << "File is not readable" << endl;
        exit(1);
    }
    
    Block * GENESIS_BLOCK = new Block(); // not implemented
   
    int numberOfPeers, numberOfEdges;
    input >> numberOfPeers >> numberOfEdges;
	
	MAX_USERS = numberOfPeers;

	vector<int> Z0_distribution(Z0*numberOfPeers, NODE_SLOW);
	vector<int> Z1_distribution(Z1*numberOfPeers, NODE_LOW_CPU);

	while(Z0_distribution.size() != numberOfPeers) Z0_distribution.push_back(NODE_FAST);
	while(Z1_distribution.size() != numberOfPeers) Z1_distribution.push_back(NODE_FAST_CPU);

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
		LIST_OF_EVENTS.insert(new GenerateTransaction(0, &LIST_OF_NODES[i]));
	}
	for(int i = 0; i<MAX_USERS; i++){
		LIST_OF_EVENTS.insert(new GenerateBlock(0, &LIST_OF_NODES[i]), LIST_OF_NODES[i]->get_longest_chain_tail());
	}

}

void run_loop(){
    CURRENT_TIME = 0;
    while(!LIST_OF_EVENTS.empty()) {
		if(CURRENT_TIME > END_TIME) break; 
        auto top = LIST_OF_EVENTS.begin();
        top->simulate_event();
        CURRENT_TIME = top->timestamp;
        LIST_OF_EVENTS.erase(top);
    }
}

int main(int argc, char * argv[]){
    if(argc != 2){
        cout << "./main <file generated by grpah.py>" << endl;
        return 1;
    }
    string file_name(argv[1]);
    init(file_name);
	create_initial_events();
    run_loop();
    return 0;
}


