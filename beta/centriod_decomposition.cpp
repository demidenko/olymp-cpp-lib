void centriod_decomposition(
	const auto &g,
	const function<void(const graph<>&, size_t, size_t)> &action
	//use as [](const auto &t, size_t centroid, size_t sizeof_subtree)
) {
	
	size_t n = size(g);
	vector<bool> used(n);
	vector<size_t> par(n), sub(n);
	
	auto bfs = [&](size_t start, size_t p) {
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
	
	graph tree(n);
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
		for(size_t i : g[c]) if(!used[i]) {
			go(i);
			tree.add_edge(c, i);
		}
		action(tree, c, sz);
	};
	
	bfs(0,-1);
	go(0);
}
