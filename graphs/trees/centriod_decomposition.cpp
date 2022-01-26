template<class... T>
void centriod_decomposition(const graph_t<T...> &g, auto action) {
	const size_t n = size(g); if(n == 0) return ;
	vector<size_t> sub(n, 1), p(n, -1), q(n);
	for(size_t k=0, qn=1; k<qn; ++k) {
		size_t v = q[k];
		for(size_t i : g[v]) if(i!=p[v]) q[qn++] = i, p[i] = v;
	}
	for(size_t i=n-1; i; --i) sub[p[q[i]]] += sub[q[i]];
	graph_t<T...> tree(n);
	auto add_edge = [&tree](auto&&... args) { tree.add_edge(args...); };
	vector<bool> used(n);
	function<void(size_t)> go = [&](size_t c) {
		const size_t sz = sub[c];
		for(size_t pr = -1; exchange(pr,c) != c; )
			for(size_t i : g[c]) if(!used[i] && sub[i]*2 > sz) {
				sub[c] -= exchange(sub[i], sz);
				c = i;
				break;
			}
		used[c] = true;
		for(auto &e : g[c]) if(size_t i=e; !used[i]) {
			go(i);
			std::apply(add_edge, tuple_cat(tuple{c}, tuple<size_t,T...>{e}));
		}
		action(std::as_const(tree), c, sz);
	};
	go(0);
}
//use action as [&](auto &g, size_t centroid, size_t sizeof_subtree)
