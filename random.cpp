#include "utils.h"

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


