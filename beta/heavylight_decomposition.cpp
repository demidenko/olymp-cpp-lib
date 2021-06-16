struct heavylight_decomposition {
    heavylight_decomposition(const graph &g, size_t root = 0) {
        size_t n = size(g);
        par.assign(n, -1);
        header.assign(n, -1);
        calc_sz(g, root);
        tin.resize(n);
        tn = 0;
        build(g, root, root);
    }
    
    size_t index(size_t v) { return tin[v]; }
    
    size_t query_path(size_t x, size_t y, auto proceed_range) {
        if(tin[x] > tin[y]) swap(x, y);
        while(tin[header[y]] > tin[x]) {
            size_t v = header[y];
            proceed_range(tin[v], tin[y]+1);
            y = par[v];
            assert(y!=-1);
        }
        while(header[x] != header[y]) {
            size_t v = header[x];
            proceed_range(tin[v], tin[x]+1);
            x = par[v];
            assert(x!=-1);
        }
        if(tin[x] > tin[y]) swap(x, y);
        proceed_range(tin[x], tin[y]+1);
        return x;
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
    
    void build(const graph &g, size_t v, size_t f) {
        tin[v] = tn++;
        size_t mx = header[v];
        header[v] = f;
        if(mx == -1) return ;
        build(g, mx, f);
        for(size_t i : g[v]) if(i!=par[v] && i!=mx) build(g, i, i);
    }
};