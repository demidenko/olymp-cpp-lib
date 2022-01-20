template<class... T>
void centriod_decomposition(
	const graph_t<T...> &g,
	auto action //use as [&](auto &g, size_t centroid, size_t sizeof_subtree)
) {
	const size_t n = size(g); if(n == 0) return ;
	vector<bool> used(n);
	vector<size_t> par(n), sub(n), q(n);
	
	auto bfs = [&](size_t start, size_t p) {
		if(start == n || used[start]) return ;
		par[start] = p;
		size_t qn = 0;
		q[qn++] = start;
		for(size_t k=0; k<qn; ++k) {
			size_t v = q[k];
			sub[v] = 1;
			for(size_t i : g[v]) if(!used[i] && i!=par[v]) {
				q[qn++] = i;
				par[i] = v;
			}
		}
		while(--qn) sub[par[q[qn]]] += sub[q[qn]];
	};
	
	graph_t<T...> tree(n);
	auto add_edge = [&tree](auto&&... args) { tree.add_edge(args...); };
	function<void(size_t)> go = [&](size_t c) {
		const size_t sz = sub[c];
		for(size_t nx=c; nx != n; ) {
			c = exchange(nx, n);
			for(size_t i : g[c]) if(!used[i] && i!=par[c] && sub[i]*2 > sz) nx = i;
		}
		used[c] = true;
		bfs(par[c], c);
		for(auto &e : g[c]) if(size_t i=e; !used[i]) {
			go(i);
			std::apply(add_edge, tuple_cat(tuple{c}, tuple<size_t,T...>{e}));
		}
		action(std::as_const(tree), c, sz);
	};
	
	bfs(0, n);
	go(0);
}
