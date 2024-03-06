template<template<class> class C, int dir, std::signed_integral index_t = ptrdiff_t, class T>
vector<index_t> nearest(const vector<T> &v) requires(std::abs(dir) == 1) {
	static constexpr C<T> cmp{};
	const index_t n = ssize(v), out = dir < 0 ? -1 : n;
	vector<index_t> f(n);
	for(index_t i = out-dir, k = 0; k < n; ++k, i-=dir)
		for(f[i] = i+dir; f[i]!=out && !cmp(v[f[i]], v[i]); f[i] = f[f[i]]) ;
	return f;
}
