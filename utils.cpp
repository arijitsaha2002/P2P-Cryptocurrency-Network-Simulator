#include "utils.h"

RandomNumber rng;
vector<Node *> LIST_OF_NODES;


set<Event*, EventCMP> LIST_OF_EVENTS;

pair<int, int> getSortedPair(int a, int b) {
    return make_pair(min(a, b), max(a, b));
}


int TransactionAmount(uid_t sender){
    int low = 0;
    int high = 50;
    return rng.uniformNumber(low, high);
}

void add_event_to_queue(Event * e) {
    LIST_OF_EVENTS.insert(e);
}

