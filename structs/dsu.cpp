struct dsu {
	dsu(size_t n = 0): sz(n,1), p(n) {
		iota(ALL(p),0);
	}
	size_t get(size_t i){
		size_t v = i;
		while(v!=p[v]) v = p[v];
		while(i!=p[i]) i = exchange(p[i],v);
		return v;
	}
	bool unite(size_t i, size_t j){
		i = get(i);
		j = get(j);
		if(i==j) return false;
		if(sz[i]<sz[j]) swap(i,j);
		p[j] = i;
		sz[i]+=sz[j];
		return true;
	}
	size_t size(size_t i){
		return sz[get(i)];
	}
	
	private:
	vector<size_t> p, sz;
};