template<typename T>
vector<T> tree_dp_root_each(
	const graph &g,
	function<T(size_t)> single,
	function<T(T, T)> f
) {
	size_t n = size(g);
	vector<T> res(n), up(n), excl(n);
	
	using it = typename vector<size_t>::iterator;
	function<void(it,it,T)> exclude = [&](it l, it r, const T& val) {
		if(l==r) return ;
		if(l+1 == r) { excl[*l] = val; return ; }
		auto m = l + (r-l)/2;
		T x = val; for(it i=l; i!=m; ++i) x = f(x, res[*i]);
		exclude(m, r, x);
		x = val; for(it i=m; i!=r; ++i) x = f(x, res[*i]);
		exclude(l, m, x);
	};
	
	vector<size_t> par(n, -1), q(n), ar;
	size_t ql = 0, qr = 0;
	for(size_t root=0; root<n; ++root) if(par[root]==-1) {
		for(q[qr++] = root; ql<qr; ++ql) {
			size_t v = q[ql];
			res[v] = single(v);
			for(size_t i : g[v]) if(i!=par[v]) {
				par[i] = v;
				q[qr++] = i;
			}
		}
	}
	
	for(size_t k=qr; k--;)
		if(size_t v = q[k], p = par[v]; p!=-1)
			res[p] = f(res[p], res[v]);
	
	for(size_t v : q) {
		ar.clear();
		for(size_t i : g[v]) if(i!=par[v]) ar.push_back(i);
		exclude(ALL(ar), single(v));
		for(size_t i : g[v]) if(i!=par[v]) {
			up[i] = par[v]==-1 ? excl[i] : f(excl[i], up[v]);
			res[i] = f(res[i], up[i]);
		}
	}
	
	return res;
}
