struct heavy_light_decomposition {
	heavy_light_decomposition(const graph &g, size_t root = 0) {
		size_t n = size(g);
		par.assign(n, -1);
		header.assign(n, -1);
		calc(g, root);
		tin.resize(n);
		tn = 0;
		go(g, root, root);
		assert(tn == n);
	}
	
	size_t index(size_t v) const { return tin[v]; }
	
	size_t query_path(size_t x, size_t y, auto process_range, bool ignore_lca = false) const {
		if(tin[x] > tin[y]) swap(x, y);
		for(size_t v; tin[v=header[y]] > tin[x]; y = par[v]) 
			process_range(tin[v], tin[y]+1);
		for(size_t v; (v=header[x]) != header[y]; x = par[v])
			process_range(tin[v], tin[x]+1);
		if(tin[x] > tin[y]) swap(x, y);
		if(size_t l = tin[x]+ignore_lca, r = tin[y]+1; l<r)
			process_range(l, r);
		return x;
	}
	
	size_t lca(size_t x, size_t y) const {
		return query_path(x, y, [](auto...){});
	}
	
	private:
	vector<size_t> tin, par, header;
	size_t tn;
	
	size_t calc(const graph &g, size_t v) {
		size_t mh = 0;
		for(size_t i : g[v]) if(i!=par[v]) {
			par[i] = v;
			size_t h = calc(g, i);
			if(h > mh) mh = h, header[v] = i;
		}
		return mh + 1;
	}
	
	void go(const graph &g, size_t v, size_t f) {
		tin[v] = tn++;
		size_t mx = exchange(header[v], f);
		if(mx == -1) return ;
		go(g, mx, f);
		for(size_t i : g[v]) if(i!=par[v] && i!=mx) go(g, i, i);
	}
};
