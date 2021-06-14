vector<size_t> strong_connected_components(const graph &g) {
	size_t n = size(g), tn = 0, cn = 0;
	vector<size_t> tin(n), h(n), comp(n), s;
	function<void(size_t)> css = [&](size_t v) {
		h[v] = tin[v] = ++tn;
		s.push_back(v);
		for(size_t i : g[v]){
			if(!tin[i]) css(i);
			if(tin[i] && !comp[i]) h[v] = min(h[v], h[i]);
		}
		if(h[v]==tin[v]) for(++cn; !comp[v]; s.pop_back()) comp[s.back()] = cn;
	};
	for(size_t i=0; i<n; ++i) if(!tin[i]) css(i);
	for(size_t &x : comp) x = cn-x;
	return comp;
};
