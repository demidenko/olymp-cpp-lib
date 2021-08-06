auto parallel_binary_search(
	size_t qn, size_t n, auto init, auto update, auto ask
) {
	vector<size_t> res(qn);
	vector<tuple<size_t,size_t,size_t>> v(qn);
	for(size_t i=0; i<qn; ++i) v[i] = {0, n+1, i};
	while(!empty(v)) {
		init();
		size_t sz = 0;
		for(const auto &[l, r, qi] : v) {
			if(l < r) v[sz++] = {l, r, qi};
			else res[qi] = r;
		}
		v.resize(sz);
		sort(begin(v),end(v));
		size_t k = 0;
		for(auto &[l, r, qi] : v) {
			size_t m = (l + r) >> 1;
			while(k < m) update(++k);
			if(ask(qi)) r = m;
			else l = m+1;
		}
	}
	return res;
}
