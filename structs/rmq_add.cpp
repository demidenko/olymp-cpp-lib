template<typename T, const T&(*f)(const T&, const T&) = std::min<T> >
struct rmq_add {
	constexpr static T neutral = [](T m,T M){return f(m,M)==m?M:m;}(numeric_limits<T>::min(),numeric_limits<T>::max());
	
	rmq_add(size_t sz = 0) {
		for(d=1;d<sz;d<<=1);
		t.assign(d*2, pair(neutral, T()));
	}
	
	template<typename C>
	rmq_add(const vector<C> &vals): rmq_add(size(vals)) {
		for(size_t i=0; i<size(vals); ++i) t[i+d].first = vals[i];
		_build();
	}
	
	void add(size_t l, size_t r, const T& val) {
		if(r>d) r = d;
		if(l<r) _add(l,r,val,0,d,1);
	}
	
	T operator()(size_t l, size_t r) {
		if(r>d) r = d;
		if(l<r) return _calc(l,r,0,d,1);
		return neutral;
	}
	
	private:
	size_t d;
	vector<pair<T,T>> t;
	
	void _build() {
		for(size_t i=d; i-->1; ) t[i].first = f(t[i*2].first, t[i*2+1].first);
	}
	
	void _add(size_t i, size_t j, const T &val, size_t l, size_t r, size_t v){
		if(i==l && j==r){
			t[v].first+=val;
			t[v].second+=val;
			return ;
		}
		size_t m = (l+r)>>1;
		if(i<m) _add(i,min(j,m),val,l,m,v*2);
		if(m<j) _add(max(i,m),j,val,m,r,v*2+1);
		t[v].first = f(t[v*2].first, t[v*2+1].first) + t[v].second;
	}
	
	T _calc(size_t i, size_t j, size_t l, size_t r, size_t v){
		if(i==l && j==r) return t[v].first;
		size_t m = (l+r)>>1;
		return (j<=m ? _calc(i,j,l,m,v*2) : i>=m ? _calc(i,j,m,r,v*2+1)
		 : f(_calc(i,m,l,m,v*2), _calc(m,j,m,r,v*2+1))) + t[v].second;
	}
	
};