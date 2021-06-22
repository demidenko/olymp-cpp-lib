template<typename S>
vector<S> tree_dp_root_each(
	const auto &g,
	function<S(size_t)> single,
	auto f //[](S dv, S di, auto &edge) -> S { }
) {
	size_t n = size(g);
	vector<S> res(n), up(n), excl(n);
	vector<typename decay_t<decltype(g)>::E> epar(n);
	
	using it = typename vector<size_t>::iterator;
	function<void(it,it,S)> exclude = [&](it l, it r, const S& val) {
		if(l==r) return ;
		if(l+1 == r) { excl[*l] = val; return ; }
		auto m = l + (r-l)/2;
		S x = val; for(it i=l; i!=m; ++i) x = f(x, res[*i], epar[*i]);
		exclude(m, r, x);
		x = val; for(it i=m; i!=r; ++i) x = f(x, res[*i], epar[*i]);
		exclude(l, m, x);
	};
	
	vector<size_t> par(n,-1), q(n), ar;
	size_t ql = 0, qr = 0;
	for(size_t root=0; root<n; ++root) if(par[root]==-1) {
		for(q[qr++] = root; ql<qr; ++ql) {
			size_t v = q[ql];
			res[v] = single(v);
			for(auto &edge : g[v]) if(size_t i=edge; i!=par[v]) {
				par[i] = v;
				epar[i] = edge;
				q[qr++] = i;
			}
		}
	}
	
	for(size_t k=qr; k--;)
		if(size_t v = q[k], p = par[v]; p!=-1)
			res[p] = f(res[p], res[v], epar[v]);
	
	for(size_t v : q) {
		ar.clear();
		for(auto &edge : g[v]) if(size_t i=edge; i!=par[v]) ar.push_back(i); else {
			up[v] = par[i]==-1 ? excl[v] : f(excl[v], up[i], epar[i]);
			res[v] = f(res[v], up[v], epar[v] = edge);
		}
		exclude(ALL(ar), single(v));
	}
	
	return res;
}
