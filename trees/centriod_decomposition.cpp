template<class...T>
void centriod_decomposition(
	const graph_t<T...> &g,
	auto action //use as [&](const auto &t, size_t centroid, size_t sizeof_subtree)
) {
	size_t n = size(g);
	vector<bool> used(n);
	vector<size_t> par(n), sub(n);
	
	auto bfs = [&](size_t start, size_t p) {
		if(start == -1 || used[start]) return ;
		par[start] = p;
		vector q = {start};
		for(size_t k=0; k<size(q); ++k) {
			size_t v = q[k];
			sub[v] = 1;
			for(size_t i : g[v]) if(!used[i] && i!=par[v]) {
				q.push_back(i);
				par[i] = v;
			}
		}
		for(size_t k=size(q)-1; k; --k){
			size_t v = q[k];
			sub[par[v]]+=sub[v];
		}
	};
	
	graph_t<T...> tree(n);
	auto add_edge = [&tree](auto&&... args) { tree.add_edge(args...); };
	function<void(size_t)> go = [&](size_t v) {
		const size_t sz = sub[v];
		for(size_t nx=v; nx!=-1;) {
			v = nx;
			nx = -1;
			for(size_t i : g[v]) 
				if(!used[i] && i!=par[v] && sub[i]*2 > sz) {
					nx = i;
					break;
				}
		}
		const size_t c = v;
		used[c] = true;
		bfs(par[c], c);
		for(auto &e : g[c]) if(size_t i=e; !used[i]) {
			go(i);
			std::apply(add_edge, tuple_cat(tuple{c}, tuple<size_t,T...>{e}));
		}
		action(tree, c, sz);
	};
	
	bfs(0, -1);
	go(0);
}
