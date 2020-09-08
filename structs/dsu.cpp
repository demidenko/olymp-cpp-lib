struct dsu {
	vector<int> p, sz;
	dsu(int n): sz(n,1), p(n) {
		iota(ALL(p),0);
	}
	int get(int i){
		int v = i;
		while(v!=p[v]) v = p[v];
		while(i!=p[i]) i = exchange(p[i],v);
		return v;
	}
	bool unite(int i, int j){
		i = get(i);
		j = get(j);
		if(i==j) return false;
		if(sz[i]<sz[j]) swap(i,j);
		p[j] = i;
		sz[i]+=sz[j];
		return true;
	}
};