template<bool heavy_by_size = true>
struct heavy_light_decomposition {
	heavy_light_decomposition(const auto &g, size_t root = 0):
		par(size(g),-1), header(size(g),-1), tin(size(g)), tn(0)
	{
		calc(g, root);
		build(g, root, root);
		assert(tn == size(g));
	}
	
	size_t index(size_t v) const { return tin[v]; }
	
	size_t lca(size_t x, size_t y) const { return query_path(x, y, [](...){}); }
	
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
	
	size_t query_path_strict(size_t x, size_t y, auto process_range, bool ignore_lca = false) const {
		vector<pair<size_t,size_t>> sl, sr;
		size_t m = min(tin[x],tin[y]), z = query_path(x, y, [&](size_t l, size_t r) { (r-1 > m ? sr : sl).emplace_back(l, r); });
		if(tin[x] > tin[y]) sl.swap(sr);
		for(auto [l, r] : sl) process_range(l, r, true);
		for(size_t i=size(sr); i--; ) process_range(sr[i].first, sr[i].second, false);
		return z;
	}
	
	private:
	vector<size_t> par, header, tin;
	size_t tn;
	
	size_t calc(const auto &g, size_t v) {
		size_t mx = 0, sz = 1;
		for(size_t i : g[v]) if(i!=par[v]) {
			par[i] = v;
			size_t x = calc(g, i);
			if(x > mx) mx = x, header[v] = i;
			if constexpr (heavy_by_size) sz += x;
		}
		if constexpr (heavy_by_size) return sz;
		else return mx + 1;
	}
	
	void build(const auto &g, size_t v, size_t f) {
		tin[v] = tn++;
		size_t mx = exchange(header[v], f);
		if(mx == -1) return ;
		build(g, mx, f);
		for(size_t i : g[v]) if(i!=par[v] && i!=mx) build(g, i, i);
	}
};
