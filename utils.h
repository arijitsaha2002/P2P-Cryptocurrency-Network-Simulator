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

using namespace std;

#define MIN_DEGREE 3
#define MAX_DEGREE 6
#define newline '\n'

extern int MAX_USERS;

class RandomNumber{
    random_device rd;
public:
    mt19937 gen;
    RandomNumber() {
        gen = mt19937(rd());
    }

    int uniformNumber(int low, int high) {
        uniform_int_distribution<int> dist(low, high);
        return dist(gen);
    }

    vector<int> uniformNumberArrayDistinct(int low, int high, unsigned int size) {
        set<int> S;
        while (S.size() != size) {
            S.insert(uniformNumber(low, high));
        }
        return vector<int>(S.begin(), S.end());
    }

    double expDistributionNumber(double lambda){
        std::exponential_distribution<double> expDist(lambda);
        return expDist(gen);
    }

    vector<double> expDistributionArray(double lambda, unsigned int size){
        vector<double> S;
        std::exponential_distribution<double> expDist(lambda);
        for(uint i = 0; i<size; i++){
            S.push_back(expDist(gen));
        }

        return S;
    }

	/* Pending Implementation */
	long double get_latency_between_nodes(Node* n1,Node* n2, int size /* in KB */);
	long double get_next_block_time(Node* n);
};

extern RandomNumber rng;
extern vector<Node *> LIST_OF_NODES;
extern set<Event> LIST_OF_EVENTS;



pair<int, int> getSortedPair(int, int);
int TransactionAmount(uid_t sender);
void add_event_to_queue(Event e);

/* Pending Implementation */
void add_transaction_to_mempool(Transaction* new_transaction);
void add_event_to_queue(Event* new_event);

#endif
