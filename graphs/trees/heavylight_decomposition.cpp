struct heavy_light_decomposition {
	heavy_light_decomposition(const auto &g, size_t root = 0, bool use_longest_path = false):
		par(size(g),-1), header(size(g),-1), tin(size(g))
	{
		calc(g, root, use_longest_path);
		size_t tn = 0;
		build(g, root, root, tn);
		assert(tn == size(g));
	}
	
	size_t index(size_t v) const { return tin[v]; }
	
	size_t lca(size_t x, size_t y) const { return decompose(x, y, [](...){}); }
	
	size_t decompose(size_t x, size_t y, auto &&process_range, bool exclude_lca = false) const {
		for(size_t v;; process_range(tin[v], tin[y] + 1), y = par[v]) {
			if(tin[x] > tin[y]) swap(x, y);
			if((v = header[y]) == header[x]) break ;
		}
		if(size_t l = tin[x]+exclude_lca, r = tin[y]+1; l < r) process_range(l, r);
		return x;
	}
	
	size_t decompose_ordered(size_t x, size_t y, auto &&process_range, bool exclude_lca = false) const {
		vector<pair<size_t,size_t>> sl, sr;
		auto f = [&, m = min(tin[x], tin[y])](size_t l, size_t r) { (r-1 > m ? sr : sl).emplace_back(l, r); };
		size_t z = decompose(x, y, f, exclude_lca);
		if(tin[x] > tin[y]) sl.swap(sr);
		for(auto [l, r] : sl) process_range(l, r, true);
		for(auto [l, r] : views::reverse(sr)) process_range(l, r, false);
		return z;
	}
	
	private: vector<size_t> par, header, tin;
	
	size_t calc(const auto &g, size_t v, bool use_longest_path) {
		size_t mx = 0, sz = 1;
		for(size_t i : g[v]) if(i!=par[v]) {
			par[i] = v;
			size_t x = calc(g, i, use_longest_path);
			if(x > mx) mx = x, header[v] = i;
			sz += x;
		}
		return use_longest_path ? mx + 1 : sz;
	}
	
	void build(const auto &g, size_t v, size_t f, size_t &tn) {
		tin[v] = tn++;
		size_t mx = exchange(header[v], f);
		if(mx != -1) build(g, mx, f, tn);
		for(size_t i : g[v]) if(i!=par[v] && i!=mx) build(g, i, i, tn);
	}
};
