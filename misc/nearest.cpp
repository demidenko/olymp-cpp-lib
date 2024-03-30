template<std::signed_integral index_t>
vector<index_t> __nearest(const auto &v, auto &&cmp, index_t dir) {
	vector<index_t> f(size(v));
	const index_t n = ssize(v), out = dir < 0 ? -1 : n;
	for(index_t i = out-dir, k = 0; k < n; ++k, i-=dir)
		for(f[i] = i+dir; f[i]!=out && !cmp(v[f[i]], v[i]); f[i] = f[f[i]]) ;
	return f;
}
auto nearest_next(const auto &v, auto &&cmp) { return __nearest(v, cmp, +1); }
auto nearest_prev(const auto &v, auto &&cmp) { return __nearest(v, cmp, -1); }
