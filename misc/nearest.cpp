/*
	vector<int> a;
	auto next_less = 
		nearest<less>(a, 1);
	auto prev_greater_or_equal = 
		nearest<greater_equal>(a, -1);
*/

template<template<typename> class C, typename T>
vector<int> nearest(const vector<T> &v, int dir){
    static constexpr C<T> cmp;
	assert(abs(dir) == 1);
	int n = size(v), fin = dir>0 ? n : -1;
	vector<int> s, res(n, fin);
	for(int i = dir>0 ? 0 : n-1; i!=fin; i+=dir){
		while(!empty(s) && cmp(v[i],v[s.back()])){
			res[s.back()] = i;
			s.pop_back();
		}
		s.push_back(i);
	}
	return res;
}
