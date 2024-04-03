template<class S, class ...T>
vector<S> tree_dp_root_each(
	const graph_t<T...> &g,
	auto &&single,
	auto &&link //[](S dv, S di, auto &edge) -> S { auto [v, i, ...] = edge }
) {
	const size_t n = size(g);
	vector<S> res(n), up(n);
	vector<edge_t<size_t,T...>> epar(n);
	
	auto expand_edge = [](size_t v, auto &e) { return tuple_cat(tuple{v}, tuple<size_t,T...>{e}); };
	
	vector<size_t> par(n,-1), q(n);
	for(size_t ql=0,qr=0,st=0; st<n; ++st) if(par[st] == -1)
		for(q[qr++] = st; ql<qr; ++ql) {
			size_t v = q[ql];
			res[v] = single(v);
			for(auto &edge : g[v]) if(size_t i=edge; i!=par[v]) {
				par[i] = v;
				epar[i] = expand_edge(v,edge);
				q[qr++] = i;
			}
		}
	
	for(size_t v : views::reverse(q)) if(size_t p=par[v]; p!=-1) res[p] = link(res[p], res[v], epar[v]);
	
	auto cf = [&](const S &x, size_t i) { return link(x, res[i], epar[i]); };
	using it = vector<size_t>::iterator;
	function<void(it, it, S)> exclude = [&](it l, it r, S val) {
		if(l+1 == r) { up[*l] = val; return ; }
		auto m = l + (r-l)/2;
		exclude(l, m, accumulate(m, r, val, cf));
		exclude(m, r, accumulate(l, m, val, cf));
	};
	
	for(vector<size_t> ar; size_t v : q) {
		ar.clear();
		for(auto &edge : g[v]) if(size_t i=edge; i!=par[v]) ar.push_back(i); else {
			if(par[i]!=-1) up[v] = link(up[v], up[i], epar[i]);
			res[v] = link(res[v], up[v], epar[v] = expand_edge(v,edge));
		}
		if(!empty(ar)) exclude(begin(ar), end(ar), single(v));
	}
	
	return res;
}
