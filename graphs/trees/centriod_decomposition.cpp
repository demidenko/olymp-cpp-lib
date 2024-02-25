template<class... T>
void centriod_decomposition(const graph_t<T...> &g, auto &&action) {
	const size_t n = size(g); if(n == 0) return ;
	vector<size_t> sub(n, 1), p(n, -1), q(n);
	for(size_t qn=1; size_t v : q)
		for(size_t i : g[v]) if(i!=p[v]) q[qn++] = i, p[i] = v;
	for(size_t i=n-1; i; --i) sub[p[q[i]]] += sub[q[i]];
	graph_t<T...> tree(n);
	vector<bool> used(n);
	function<void(size_t)> go = [&](size_t c) {
		const size_t sz = sub[c];
		for(size_t pr = -1; exchange(pr,c) != c; )
			for(size_t i : g[c]) if(!used[i] && sub[i]*2 > sz)
				sub[c] -= sub[i], sub[i] = sz, c = i;
		used[c] = true;
		for(auto &e : g[c]) if(size_t i=e; !used[i]) {
			go(i);
			apply([&](auto&&...x){ tree.add_edge(c, x...); }, edge_t<T...>{e});
		}
		action(as_const(tree), c, sz);
	};
	go(0);
}
//use action as [&](auto &g, size_t centroid, size_t sizeof_subtree)
