auto strong_connected_components(const auto &g) {
	size_t n = size(g), tn = 0, cn = 0, sn = 0;
	vector<size_t> t(n), h(n), c(n), s(n);
	function<void(size_t)> css = [&](size_t v) {
		h[v] = t[v] = ++tn;
		s[sn++] = v;
		for(size_t i : g[v]) {
			if(!t[i]) css(i);
			if(!c[i] && h[i] < h[v]) h[v] = h[i];
		}
		if(h[v] == t[v]) for(++cn; !c[v];) c[s[--sn]] = cn;
	};
	for(size_t i=0; i<n; ++i) if(!t[i]) css(i);
	for(size_t &x : c) x = cn-x;
	return pair{cn, c};
};
