template<class T, class A = T>
struct rsq_assign {
	explicit rsq_assign(size_t sz = 0): d(_p2(sz)), t(d*2, {{},nullopt}) {}
	
	rsq_assign(const vector<auto> &vals): rsq_assign(size(vals)) {
		for(size_t i=0; i<size(vals); ++i) t[i+d].first = vals[i];
		_build();
	}
	
	void assign(size_t l, size_t r, const A &value) {
		if(r>d) r = d;
		if(l<r) _assign(l,r,value,0,d,1);
	}
	
	T operator()(size_t l, size_t r) const {
		if(r>d) r = d;
		T buf{};
		if(l<r) _calc(l,r,0,d,1,buf);
		return buf;
	}
	
	private:
	const size_t d;
	vector<pair<T,optional<A>>> t;
	
	static size_t _p2(size_t n) { return n > 1 ? (2<<__lg(n-1)) : 1; }
	void _build() { for(size_t i=d; i-->1; ) _build_node(i); }
	inline void _build_node(size_t v) { t[v].first = t[v*2].first + t[v*2+1].first; }
	
	void _push(size_t v, size_t len) {
		if(auto &a = t[v].second) {
			t[v*2] = t[v*2+1] = {T(len>>1) * *a, a};
			a.reset();
		}
	}
	
	void _assign(size_t i, size_t j, const A &val, size_t l, size_t r, size_t v) {
		if(i==l && j==r) {
			t[v] = {T(r-l) * val, val};
			return ;
		}
		_push(v,r-l);
		size_t m = (l+r)>>1;
		if(i<m) _assign(i,min(j,m),val,l,m,v*2);
		if(m<j) _assign(max(i,m),j,val,m,r,v*2+1);
		_build_node(v);
	}
	
	void _calc(size_t i, size_t j, size_t l, size_t r, size_t v, T &buf) const {
		if(i==l && j==r) buf += t[v].first; else
		if(auto a = t[v].second) buf += T(j-i) * *a; else {
			size_t m = (l+r)>>1;
			if(i<m) _calc(i,min(j,m),l,m,v*2,buf);
			if(m<j) _calc(max(i,m),j,m,r,v*2+1,buf);
		}
	}
};
