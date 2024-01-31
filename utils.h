#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <vector>
#include <set>
#include <ctime>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <queue>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <unordered_map>
#include "event.h"

#include <map>
using namespace std;

#define MIN_DEGREE 3
#define MAX_DEGREE 6
#define newline '\n'

class RandomNumber;
class Event;
class Node;
class Transaction;
class TransactionRecieved;

struct EventCMP {
    bool operator()(Event * event1, Event * event2) const {
        return (*event1) < (*event2);
    }
};

extern int MAX_USERS;
extern RandomNumber rng;
extern vector<Node *> LIST_OF_NODES;
extern set<Event *, EventCMP> LIST_OF_EVENTS;
extern long double CURRENT_TIME;

pair<int, int> getSortedPair(int, int);
int TransactionAmount(uid_t sender);
void add_event_to_queue(Event *e);
void add_transaction_to_mempool(Transaction* new_transaction);
void add_event_to_queue(Event* new_event);

#endif
