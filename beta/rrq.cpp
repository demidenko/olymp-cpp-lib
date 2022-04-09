template<class T> struct rrq {
	rrq(const vector<T> &vals, auto build): vals(vals), d(size(vals)), t(d*2) {
		for(size_t i=0; i<d; ++i) t[i+d] = {i};
		for(size_t i=d; i-->1; ) {
			t[i].resize(size(t[i*2])+size(t[i*2+1]));
			merge(begin(t[i*2]),end(t[i*2]),begin(t[i*2+1]),end(t[i*2+1]),begin(t[i]),[&](size_t i, size_t j) {
				return vals[i] < vals[j];
			});
		}
		for(size_t i=1; i<d*2; ++i) build(i-1, as_const(t[i]));
	}
	
	void operator()(size_t l, size_t r, const T &lower, const T &upper, auto&& process) const {
		if(lower < upper) for(l+=d, r+=d; l < r; l>>=1, r>>=1) {
			if(l&1) call(lower, upper, l, process), ++l;
			if(r&1) --r, call(lower, upper, r, process);
		}
	}
	
	void operator()(size_t i, auto &&process) const {
		for(size_t v=i+d; v; v>>=1) process(v-1, lb(t[v], vals[i]));
	}
	
	private:
	vector<T> vals;
	size_t d;
	vector<vector<size_t>> t;
	inline void call(const T &lower, const T &upper, size_t k, auto&& process) const {
		if(size_t l = lb(t[k], lower), r = lb(t[k], upper); l < r) process(k-1, l, r);
	}
	inline size_t lb(const vector<size_t> &v, const T &val) const {
		return partition_point(begin(v), end(v), [&](size_t i) { return vals[i] < val; }) - begin(v);
	}
};
