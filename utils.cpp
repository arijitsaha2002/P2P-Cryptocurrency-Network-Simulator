#include "utils.h"

pair<int, int> getSortedPair(int a, int b) {
    return make_pair(min(a, b), max(a, b));
}


int TransactionAmount(uid_t sender){
    int low = 0;
    int high = 50;
    return rng.uniformNumber(low, high);
}

RandomNumber rng;
