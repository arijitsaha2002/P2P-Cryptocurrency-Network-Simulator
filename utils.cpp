#include "utils.h"
#include "events.h"

RandomNumber rng;
extern vector<Node *> LIST_OF_NODES;
extern set<Event> LIST_OF_EVENTS;
unordered_map<pair<int, int>, double> rho;

pair<int, int> getSortedPair(int a, int b) {
    return make_pair(min(a, b), max(a, b));
}


int TransactionAmount(uid_t sender){
    int low = 0;
    int high = 50;
    return rng.uniformNumber(low, high);
}

void add_event_to_queue(Event * e) {
    LIST_OF_EVENTS.insert(*e);
}

long double RandomNumber::get_latency_between_nodes(Node* n1,Node* n2, int size /* in KB */){
	long double pij, cij, dij;
	auto p = getSortedPair(n1->get_id(), n2->get_id());
	if(rho.find() == rho.end()) rho[p] = uniformNumber(10,500);
	size = size * 8;
	pij = rho[p];
	cij = (n1->isFast && n2->isFast)?1000000*100 : 1000000*5;
	dij = expDistributionNumber(cij/ 96) * 0.001;
	return pij + size / cij + dij;
}

