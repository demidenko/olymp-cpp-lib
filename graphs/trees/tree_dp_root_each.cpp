template<class S, class ...T>
vector<S> tree_dp_root_each(
	const graph_t<T...> &g,
	function<S(size_t)> single,
	auto f //[](S dv, S di, auto &edge) -> S { auto [v, i, ...] = edge }
) {
	const size_t n = size(g);
	vector<S> res(n), up(n);
	vector<edge_t<size_t,T...>> epar(n);
	
	auto cf = [&f, &res, &epar](const S &x, size_t i) { return f(x, res[i], epar[i]); };
	auto expand_edge = [](size_t v, const auto &e) { return tuple_cat(tuple{v}, tuple<size_t,T...>{e}); };
	
	using it = vector<size_t>::iterator;
	function<void(it, it, const S&)> exclude = [&](it l, it r, const S &val) {
		if(l == r) return ;
		if(l+1 == r) { up[*l] = val; return ; }
		auto m = l + (r-l)/2;
		exclude(l, m, accumulate(m, r, val, cf));
		exclude(m, r, accumulate(l, m, val, cf));
	};
	
	vector<size_t> par(n,-1), q(n), ar;
	size_t ql = 0, qr = 0;
	for(size_t st=0; st<n; ++st) if(par[st] == -1)
		for(q[qr++] = st; ql<qr; ++ql) {
			size_t v = q[ql];
			res[v] = single(v);
			for(auto &edge : g[v]) if(size_t i=edge; i!=par[v]) {
				par[i] = v;
				epar[i] = expand_edge(v,edge);
				q[qr++] = i;
			}
		}
	
	while(qr--) if(size_t v=q[qr], p=par[v]; p!=-1) res[p] = f(res[p], res[v], epar[v]);
	
	for(size_t v : q) {
		ar.clear();
		for(auto &edge : g[v]) if(size_t i=edge; i!=par[v]) ar.push_back(i); else {
			if(par[i]!=-1) up[v] = f(up[v], up[i], epar[i]);
			res[v] = f(res[v], up[v], epar[v] = expand_edge(v,edge));
		}
		exclude(begin(ar), end(ar), single(v));
	}
	
	return res;
}
