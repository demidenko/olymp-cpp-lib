struct heavy_light_decomposition {
	heavy_light_decomposition(const graph &g) {
		size_t n = size(g);
		par.assign(n, -1);
		header.assign(n, -1);
		calc_sz(g, 0);
		tin.resize(n);
		tn = 0;
		go(g, 0, 0);
	}
	
	size_t index(size_t v) const { return tin[v]; }
	
	void query_path(size_t x, size_t y, auto proceed_range) const {
		if(tin[x] > tin[y]) swap(x, y);
		for(size_t v; tin[v=header[y]] > tin[x]; y = par[v]) 
			proceed_range(tin[v], tin[y]+1);
		for(size_t v; (v=header[x]) != header[y]; x = par[v])
			proceed_range(tin[v], tin[x]+1);
		if(tin[x] > tin[y]) swap(x, y);
		proceed_range(tin[x], tin[y]+1);
	}
	
	private:
	vector<size_t> tin, par, header;
	size_t tn;
	
	size_t calc_sz(const graph &g, size_t v) {
		size_t sz = 1, ms = 0;
		for(size_t i : g[v]) if(i!=par[v]) {
			par[i] = v;
			size_t s = calc_sz(g, i);
			if(s > ms) ms = s, header[v] = i;
			sz+=s;
		}
		return sz;
	}
	
	void go(const graph &g, size_t v, size_t f) {
		tin[v] = tn++;
		size_t mx = exchange(header[v], f);
		if(mx == -1) return ;
		go(g, mx, f);
		for(size_t i : g[v]) if(i!=par[v] && i!=mx) go(g, i, i);
	}
};
