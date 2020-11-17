template<typename T, const T&(*f)(const T&, const T&) = std::min<T> >
struct rmq_add {
	constexpr static T inf = numeric_limits<T>::max()/2;
	constexpr static T neutral = -f(inf,-inf);
	
	rmq_add(size_t sz = 0){
		for(d=1;d<sz;d<<=1);
		t.assign(d*2, neutral);
		ta.assign(d*2, T(0));
	}
	
	rmq_add(size_t sz, const T &val): rmq_add(sz) {
		fill(begin(t)+d, begin(t)+d+sz, val);
		_build();
	}
	
	template<typename C>
	rmq_add(const vector<C> &vals): rmq_add(size(vals)) {
		copy(begin(vals), end(vals), begin(t)+d);
		_build();
	}
	
	void add(int l, int r, const T& val){
		if(l<0) l = 0;
		if(r>d) r = d;
		if(l>=r) return ;
		_add(l,r,val,0,d,1);
	}
	
	T operator()(int l, int r){
		assert(0<=l && l<r && r<=d);
		return _calc(l,r,0,d,1);
	}
	
	private:
	
	vector<T> t, ta;
	size_t d;
	
	void _build(){
		for(size_t i=d; i-->1; ) t[i] = max(t[i*2], t[i*2+1]);
	}
	
	void _add(size_t i, size_t j, const T &val, size_t l, size_t r, size_t v){
		if(i==l && j==r){
			ta[v]+=val;
			t[v]+=val;
			return ;
		}
		size_t m = (l+r)/2;
		if(i<m) _add(i,min(j,m),val,l,m,v*2);
		if(j>m) _add(max(i,m),j,val,m,r,v*2+1);
		t[v] = f(t[v*2], t[v*2+1]) + ta[v];
	}
	
	T _calc(size_t i, size_t j, size_t l, size_t r, size_t v){
		if(i==l && j==r) return t[v];
		size_t m = (l+r)/2;
		T res = neutral;
		if(i<m) res = f(res, _calc(i,min(j,m),l,m,v*2));
		if(j>m) res = f(res, _calc(max(i,m),j,m,r,v*2+1));
		return res + ta[v];
	}
	
};