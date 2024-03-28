template<class T, class A = T>
struct rsq_range_assign {
	explicit rsq_range_assign(size_t sz = 0): d(_p2(sz)), t(d*2, {{},nullopt}) {}
	
	rsq_range_assign(const vector<auto> &vals): rsq_range_assign(size(vals)) {
		for(size_t i=0; i<size(vals); ++i) t[i+d].first = vals[i];
		for(size_t i=d; i-->1; ) t[i].first = t[i*2].first + t[i*2+1].first;
	}
	
	void assign(size_t l, size_t r, const A &value) {
		if(l < r) assign(l, r, value, 0, d, 1);
	}
	
	T operator()(size_t l, size_t r) const {
		T buf{};
		if(l < r) calc(l, r, 0, d, 1, buf);
		return buf;
	}
	
	private:
	size_t d;
	vector<pair<T,optional<A>>> t;
	
	static size_t _p2(size_t n) { return n > 1 ? (2<<__lg(n-1)) : 1; }
	
	void assign(size_t i, size_t j, const A &val, size_t l, size_t r, size_t v) {
		if(i == l && j == r) {
			t[v] = {T(r-l) * val, val};
			return ;
		}
		size_t m = (l+r)>>1;
		if(auto &a = t[v].second) {
			t[v*2] = t[v*2+1] = {T(m-l) * *a, a};
			a.reset();
		}
		if(i < m) assign(i, min(j,m), val, l, m, v*2);
		if(m < j) assign(max(i,m), j, val, m, r, v*2+1);
		t[v].first = t[v*2].first + t[v*2+1].first;
	}
	
	void calc(size_t i, size_t j, size_t l, size_t r, size_t v, T &buf) const {
		if(i == l && j == r) buf += t[v].first; else
		if(auto a = t[v].second) buf += T(j-i) * *a; else {
			size_t m = (l+r)>>1;
			if(i < m) calc(i, min(j,m), l, m, v*2, buf);
			if(m < j) calc(max(i,m), j, m, r, v*2+1, buf);
		}
	}
};
