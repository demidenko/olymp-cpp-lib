struct dsu_bipartite {
	explicit dsu_bipartite(size_t n = 0): p(n), sz(n, 1), o(n) {
		iota(begin(p), end(p), 0);
	}
	size_t get(size_t i) const {
		if(i == p[i]) return i;
		size_t v = get(p[i]);
		o[i] ^= o[p[i]];
		return p[i] = v;
	}
	bool unite(size_t i, size_t j, bool apart = true) {
		size_t vi = get(i), vj = get(j);
		if(vi == vj) return false;
		apart ^= o[i]^o[j];
		i = vi; 
		j = vj;
		if(sz[i] < sz[j]) swap(i, j);
		p[j] = i;
		o[j] = apart;
		sz[i] += sz[j];
		return true;
	}
	bool is_safe_to_unite(size_t i, size_t j, bool apart) const {
		if(get(i) != get(j)) return true;
		return (o[i]^o[j]) == apart;
	}
	size_t get_part(size_t i) const {
		get(i);
		return o[i];
	}
	private: 
	mutable vector<size_t> p;
	vector<size_t> sz;
	mutable vector<uint8_t> o;
};
