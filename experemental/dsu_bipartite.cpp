struct dsu_bipartite {
    vector<int> p, sz, o;
    dsu_bipartite(int n): sz(n,1), p(n), o(n,0) {
        iota(ALL(p),0);
    }
    int get(int i){
        if(i==p[i]) return i;
        int v = get(p[i]);
        o[i] ^= o[p[i]];
        return p[i]=v;
    }
    bool unite(int i, int j, int apart = true){
        int vi = get(i);
        int vj = get(j);
        if(vi==vj) return false;
        apart^=o[i]^o[j];
        i=vi; j=vj;
        if(sz[i]<sz[j]) swap(i,j);
        p[j] = i;
        o[j] = apart;
        sz[i]+=sz[j];
        return true;
    }
	bool is_safe_to_unite(int i, int j, int apart){
		if(get(i) != get(j)) return true;
		return (o[i]^o[j]) == apart;
	}
	int get_part(int i){
		get(i);
		return o[i];
	}
};