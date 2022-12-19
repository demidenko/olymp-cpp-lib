template<template<class> class C, int dir, class index_t = ptrdiff_t, class T>
vector<index_t> nearest(const vector<T> &v) {
	static_assert(abs(dir) == 1 && is_signed_v<index_t>);
	static constexpr C<T> cmp{};
	const index_t n = size(v), out = dir < 0 ? -1 : n;
	vector<index_t> f(n);
	for(index_t i = out-dir, k = 0; k < n; ++k, i-=dir)
		for(f[i] = i+dir; f[i]!=out && !cmp(v[f[i]], v[i]); f[i] = f[f[i]]) ;
	return f;
}
