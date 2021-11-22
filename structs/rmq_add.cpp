template<class T, const T& f(const T&, const T&) = std::min<T>, class A = T >
struct rmq_add {
	constexpr static T neutral = [](T m,T M){return f(m,M)==m?M:m;}(numeric_limits<T>::min(),numeric_limits<T>::max());
	
	explicit rmq_add(size_t sz = 0): d(_p2(sz)), t(d*2, {neutral,{}}) {}
	
	rmq_add(size_t sz, const T &val): rmq_add(sz) {
		for(size_t i=0; i<sz; ++i) t[i+d].first = val;
		_build();
	}
	
	rmq_add(const vector<auto> &vals): rmq_add(size(vals)) {
		for(size_t i=0; i<size(vals); ++i) t[i+d].first = vals[i];
		_build();
	}
	
	T operator()(size_t l, size_t r) const {
		if(r > d) r = d;
		return l < r ? _calc(l,r,0,d,1) : neutral;
	}
	T operator()() const { return t[1].first; }
	T operator[](size_t i) const {
		T result = t[i+=d].first;
		while(i>>=1) result += t[i].second;
		return result;
	}
	
	void add(size_t l, size_t r, const A &val) {
		if(r > d) r = d;
		if(l < r) _add(l,r,val,0,d,1);
	}
	
	private:
	const size_t d;
	vector<pair<T,A>> t;
	
	static size_t _p2(size_t n) { return n > 1 ? (2<<__lg(n-1)) : 1; }
	void _build() { for(size_t i=d; i-->1; ) _build_node(i); }
	inline void _build_node(size_t v) { t[v].first = f(t[v*2].first, t[v*2+1].first) + t[v].second; }
	
	void _add(size_t i, size_t j, const A &val, size_t l, size_t r, size_t v) {
		if(i==l && j==r) {
			t[v].first += val;
			t[v].second += val;
			return ;
		}
		size_t m = (l+r)>>1;
		if(i<m) _add(i,min(j,m),val,l,m,v*2);
		if(m<j) _add(max(i,m),j,val,m,r,v*2+1);
		_build_node(v);
	}
	
	T _calc(size_t i, size_t j, size_t l, size_t r, size_t v) const {
		if(i==l && j==r) return t[v].first;
		size_t m = (l+r)>>1;
		return (j<=m ? _calc(i,j,l,m,v*2) : i>=m ? _calc(i,j,m,r,v*2+1)
		 : f(_calc(i,m,l,m,v*2), _calc(m,j,m,r,v*2+1))) + t[v].second;
	}
};
