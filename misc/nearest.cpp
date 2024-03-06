template<int dir, std::signed_integral index_t = ptrdiff_t, class T>
vector<index_t> nearest(const vector<T> &v, auto &&cmp) requires(std::abs(dir) == 1) {
	vector<index_t> f(size(v));
	const index_t n = ssize(v), out = dir < 0 ? -1 : n;
	for(index_t i = out-dir, k = 0; k < n; ++k, i-=dir)
		for(f[i] = i+dir; f[i]!=out && !cmp(v[f[i]], v[i]); f[i] = f[f[i]]) ;
	return f;
}
