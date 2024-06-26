#pragma once
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
#include <unordered_map>
#include <map>
#include <queue>
#include <cassert>


using namespace std;

#define MIN_DEGREE 3
#define MAX_DEGREE 6
#define newline '\n'
#define EPSILON 0.01

class RandomNumber;
class Event;
class Node;
class Transaction;
class TransactionRecieved;

extern int MAX_USERS;
extern vector<Node *> LIST_OF_NODES;
extern long double CURRENT_TIME;

pair<int, int> getSortedPair(int, int);
int TransactionAmount();
void add_event_to_queue(Event *e);
void add_transaction_to_mempool(Transaction* new_transaction);
void add_event_to_queue(Event* new_event);
unordered_map<int, Transaction*> get_mem_pool();
class RandomNumber{
    random_device rd;
public:
    mt19937 gen;
    RandomNumber() {
        gen = mt19937(rd());
    }
	
	void set_seed(int seed){
		gen = mt19937(seed);
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

    long double expDistributionNumber(long double lambda){
        std::exponential_distribution<long double> expDist(lambda);
		long double ret = expDist(gen);
        return ret;
    }

    vector<double> expDistributionArray(long double lambda, unsigned int size){
        vector<double> S;
        std::exponential_distribution<long double> expDist(lambda);
        for(uint i = 0; i<size; i++){
            S.push_back(expDist(gen));
        }

        return S;
    }

	/* Pending Implementation */
	long double get_latency_between_nodes(Node* n1,Node* n2, int size /* in KB */);
	long double get_next_block_time(Node* n);
};



#endif
