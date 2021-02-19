/*
	vector<int> a;
	auto next_less = 
		nearest<less, 1>(a);
	auto prev_greater_or_equal = 
		nearest<greater_equal, -1>(a);
*/

template<template<typename> class C, int dir, typename T>
vector<int> nearest(const vector<T> &v){
	static constexpr C<T> cmp;
	static_assert(abs(dir) == 1);
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
