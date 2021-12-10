template<template<class> class C, int dir, class T>
vector<int> nearest(const vector<T> &v) {
	static_assert(abs(dir) == 1);
	static constexpr C<T> cmp{};
	int n = size(v), out = dir < 0 ? -1 : n;
	vector<int> f(n);
	for(int i = out-dir, k = 0; k < n; ++k, i-=dir)
		for(f[i] = i+dir; f[i]!=out && !cmp(v[f[i]], v[i]); f[i] = f[f[i]]) ;
	return f;
}
