#include <bits/stdc++.h>
using namespace std;

#include "../structs/rmq_static.cpp"

void creation_test() {
	//empty
	rmq<int> f0;
	
	//one
	rmq<int> f1(vector(1,0));
	
	//assign / coping
	auto f = f1;
	
	f1 = rmq<int>(vector(1<<15,-1));
	
	f = f1;
}

template<class T>
void all_range_queries(const vector<T> &vals) {
	const size_t n = size(vals);
	rmq<T,min> fmin(vals);
	rmq<T,max> fmax(vals);
	for(size_t l = 0; l < n; ++l) {
		T curmin = vals[l], curmax = vals[l];
		for(size_t r = l+1; r <= n; ++r) {
			curmin = min(curmin, vals[r-1]);
			assert(fmin(l, r) == curmin);
			curmax = max(curmax, vals[r-1]);
			assert(fmax(l, r) == curmax);
		}
	}
}

void queries_permutations(const size_t n) {
	vector<int> vals(n);
	iota(begin(vals), end(vals), 0);
	do {
		all_range_queries(vals);
	} while(next_permutation(begin(vals),end(vals)));
}

int main() {
	
	creation_test();
	
	for(size_t n = 1; n <= 8; ++n) {
		queries_permutations(n);
	}
	
	return 0;
}